/*************************************************************************
	> File Name: ServerOP.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: Sun 15 Aug 2021 05:04:27 PM CST
 ************************************************************************/

#include <iostream>
#include <json/json.h>
#include <fstream>
#include <unistd.h>
#include <exception>
#include <cstdio>
#include "Hash.h"
#include "RsaCrypto.h"
#include "ServerOP.h"
#include "TcpServer.h"
#include "RespondCodec.h"
#include "RequestCodec.h"
#include "RespondMsg.pb.h"
#include "RespondFactory.h"
#include "RequestFactory.h"
#include "ShmNodeInfo.h"
using namespace std;
using namespace Json;

ServerOP::ServerOP(string json)
{
	/* 解析json配置文件 */
	ifstream ifs(json);
	Reader r;
	Value root;
	if (r.parse(ifs, root) == false)
	{
		cout << "Json file init error" << endl;
		exit(1);
	}
	m_serverID = root["ServerID"].asString();
	m_port = root["Port"].asUInt();
	m_dbUser = root["UserDB"].asString();
	m_dbPwd = root["PwdDB"].asString();
	m_dbName = root["NameDB"].asString();

	/* 初始化与数据库的连接 */
	bool ret = m_mysql.connectDB("127.0.0.1", m_dbUser, m_dbPwd, m_dbName);
	if (ret == false) {
		cout << "database init error" << endl;
		exit(1);
	}

	/* 初始化共享内存 */
	string shmKey = root["ShmKey"].asString();
	int maxNode = root["ShmMaxNode"].asUInt();
	m_shm = new SecKeyShm(shmKey, maxNode);
	if (m_shm == nullptr) {
		cout << "share memory init error" << endl;
		exit(1);
	}

}

ServerOP::~ServerOP()
{
	m_mysql.closeDB();
	if (m_shm != nullptr) {
		delete m_shm;
		m_shm = nullptr;
	}
}

void ServerOP::startServer()
{
	TcpServer tcpServer;
	if (tcpServer.setListen(m_port) != 0)
	{
		perror("SecKeyServer start error");
		throw exception();
	}
	while (true)
	{
		cout << "Wait for the SecKeyClient connects..." << endl;
		TcpSocket *socket = tcpServer.acceptConn();
		if (socket == nullptr)
		{
			continue;
		}
		cout << "SecKeyClient connection succeeds..." << endl;
		/* 建立连接后，业务逻辑交给工作线程处理 */
		ThreadArg* arg = new ThreadArg(this, socket);
		pthread_t tid;
		pthread_create(&tid, nullptr, workHard, arg);
		pthread_detach(tid);
	}
}

string ServerOP::secKeyConsult(RequestMsg *reqMsg)
{
	cout << "The RSA_public_key from client is: " << endl;
	cout << reqMsg->data() << endl;

	/* 将从客户端发来的RSA公钥写入本地磁盘 */
	ofstream ofs("public.pem");
	ofs << reqMsg->data();
	ofs.close();

	RespondInfo info;
	RsaCrypto rsa("public.pem", false);
	/* 校验数字签名 */
	Hash hash(T_SHA1);
	hash.addData(reqMsg->data());
	bool checkRet = rsa.signVerify(hash.getResult(), reqMsg->sign());
	if (checkRet == false)
	{
		info.status = 0;
		cout << "Sign verify failed" << endl;
	}
	else
	{
		cout << "Sign verify succeed" << endl;
		/* 生成随机字符串作为AES加密算法的秘钥, 秘钥可选长度为16, 24, 32bytes */
		string AesKey = generateAesKey(Len32);
		cout << "the AES_KEY is: " << AesKey << endl;

		/* 通过客户端发来的RSA公钥对AES秘钥进行加密 */
		string secAesKey = rsa.pubKeyEncrypt(AesKey);

		/* 将生成的AES秘钥信息写入到数据库中 */
		ShmNodeInfo node;
		strcpy(node.clientID, reqMsg->clientid().c_str());
		strcpy(node.serverID, reqMsg->serverid().c_str());
		strcpy(node.secKey, AesKey.c_str());	
		node.secKeyID = m_mysql.getSecKeyID();
		node.status = 1;
		bool ret = m_mysql.writeSecKey(&node);
		if (ret == true) {
			/* 节点可用 */
			info.status = 1;
			/* 更新密钥ID */
			m_mysql.updateSecKeyID(node.secKeyID + 1);
			/* 写入共享内存 */
			m_shm->shmWrite(&node);	
		}
		else {
			/* 节点不可用 */
			info.status = 0;
		}

		/* 填充服务器的应答数据 */
		info.clientID = reqMsg->clientid();
		info.serverID = reqMsg->serverid();
		info.data = secAesKey;
		info.secKeyID = node.secKeyID;
	}
	/* 将要向客户端发送的数据序列化, 然后发送之 */
	CodecFactory *factory = new RespondFactory(&info);
	Codec *codec = factory->createCodec();
	string encMsg = codec->encodeMsg();
	delete factory;
	delete codec;

	return encMsg;
}

string ServerOP::secKeyCheck(RequestMsg* reqMsg)
{
	/* 组织向客户端发送的数据 */
	RespondInfo info;
	info.clientID = reqMsg->clientid();
	info.serverID = m_serverID;
	info.secKeyID = -1;
	info.data = string();

	/* 获取客户端发送的数据—>秘钥的哈希值 */
	string hashData = reqMsg->data();

	/* 从本地共享内存中读取出秘钥 */
	ShmNodeInfo nodeInfo = m_shm->shmRead(reqMsg->clientid(), m_serverID);
	if (nodeInfo.status == 0) {
		info.status = 0;
		cout << "No SecKeyInfo in share memory" << endl;
	}
	else {
		cout << "The SecKey read from share memory is: " << endl;
		cout << nodeInfo.secKey << endl;

		/* 对本地秘钥同样进行哈希运算 */
		Hash hash(T_SHA1);
		hash.addData(string(nodeInfo.secKey));
		string localData = hash.getResult();

		/* 比较两个哈希值 */
		if (localData == hashData) {
			info.status = 1;
		}
		else {
			info.status = 0;
		}
	}
	/* 将要向客户端发送的数据序列化, 然后发送之 */
	CodecFactory *factory = new RespondFactory(&info);
	Codec *codec = factory->createCodec();
	string encMsg = codec->encodeMsg();
	delete factory;
	delete codec;
	return encMsg;
}

string ServerOP::secKeyCancel(RequestMsg* reqMsg)
{
	/* 组织向客户端发送的数据 */
	RespondInfo info;
	info.clientID = reqMsg->clientid();
	info.serverID = m_serverID;
	info.secKeyID = stoi(reqMsg->data());
	info.data = string();

	/* 修改共享内存 */
	string keyID = reqMsg->data();
	ShmNodeInfo nodeInfo = m_shm->shmRead(reqMsg->clientid(), m_serverID);
	nodeInfo.status = 0;	/* 状态标记为不可用 */
	m_shm->shmWrite(&nodeInfo);

	/* 修改数据库中的信息 */
	bool ret = m_mysql.cancelSecKey(keyID);
	if (ret == false) {
		info.status = 0;
	} else {
		info.status = 1;
	}

	/* 将要向客户端发送的数据序列化, 然后发送之 */
	CodecFactory *factory = new RespondFactory(&info);
	Codec *codec = factory->createCodec();
	string encMsg = codec->encodeMsg();
	delete factory;
	delete codec;

	return encMsg;
}

string ServerOP::secKeyView(RequestMsg* reqMsg)
{
	/* 组织向客户端发送的数据 */
	RespondInfo info;
	info.clientID = reqMsg->clientid();
	info.serverID = m_serverID;

	/* 查找最近七天的密钥信息 */
	bool ret = m_mysql.getLastNDaysInfo(7, info.data);	
	if (ret == false) {
		info.status = 0;
	} else {
		info.status = 1;
	}
	cout << "The SecKey information of the last seven days is as follows: " << endl;
	cout << info.data << endl;

	/* 将要向客户端发送的数据序列化, 然后发送之 */
	CodecFactory *factory = new RespondFactory(&info);
	Codec *codec = factory->createCodec();
	string encMsg = codec->encodeMsg();
	delete factory;
	delete codec;

	return encMsg;
}

string ServerOP::generateAesKey(KeyLen keyLen)
{ 
	srand((unsigned int)(time(nullptr)));
	string randString = string();
	char randBuf[] = "~!@#$%^&*()_+}{|;[]";
	for (int i = 0; i < keyLen; ++i) {
		int idx = rand() % 4;
		switch (idx)
		{
		case 0:
			randString.push_back(rand() % 10 + '0');
			break;
		case 1:
			randString.push_back(rand() % 26 + 'a');
			break;
		case 2:
			randString.push_back(rand() % 26 + 'A');
			break;
		case 3:
			randString.push_back(randBuf[rand() % strlen(randBuf)]);
			break;
		default:
			break;
		}
	}
	return randString;
}

void *ServerOP::workHard(void *arg)
{
	/* 通过参数将传递的ThreadArg转换 */
	ThreadArg* targ = (ThreadArg*)arg;
	ServerOP *op = targ->sop;
	/* 接收客户端数据 */
	TcpSocket *socket = targ->socket;
	string msg = socket->recvMsg();
	/* 将接收到的数据反序列化 */
	CodecFactory *factory = new RequestFactory(msg);
	Codec *codec = factory->createCodec();
	RequestMsg *reqMsg = (RequestMsg *)codec->decodeMsg();
	/* 根据接收数据中cmdType的值，作出相应动作 */
	string data = string();
	switch (reqMsg->cmdtype())
	{
	case 1:
		/* 秘钥协商 */
		data = op->secKeyConsult(reqMsg);
		break;
	case 2:
		/* 秘钥校验 */
		data = op->secKeyCheck(reqMsg);
		break;
	case 3:
		/* 秘钥注销 */
		data = op->secKeyCancel(reqMsg);
		break;
	case 4:
		/* 秘钥查看 */
		data = op->secKeyView(reqMsg);
		break;
	default:
		break;
	}

	socket->sendMsg(data);
	socket->disConnect();
	/* 释放资源 */
	delete factory;
	delete codec;
	delete socket;
	delete targ;

	pthread_exit(0);
}