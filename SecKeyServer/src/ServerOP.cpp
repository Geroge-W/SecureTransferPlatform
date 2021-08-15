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
using namespace std;
using namespace Json;

ServerOP::ServerOP(string json)
{
	/* 解析json配置文件 */
	ifstream ifs(json);
	Reader r;
	Value root;
	if (!r.parse(ifs, root))
	{
		cout << "Json file init error" << endl;
		exit(1);
	}
	m_serverID = root["ServerID"].asString();
	m_port = root["Port"].asUInt();
}

ServerOP::~ServerOP()
{
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
		pthread_t tid;
		pthread_create(&tid, nullptr, workHard, this);
		m_list.emplace(tid, socket);
	}
}

string ServerOP::secKeyConsult(RequestMsg *reqMsg)
{
	/* 将从客户端发来的RSA公钥写入本地磁盘 */
	ofstream ofs("public.pem");
	cout << "11111111\n" << reqMsg->data() << endl;
	ofs << reqMsg->data();
	cout << "22222222" << endl;
	ofs.close();

	RespondInfo info;
	RsaCrypto rsa("public.pem", false);
	/* 校验数字签名 */
	Hash hash(T_SHA224);
	hash.addData(reqMsg->data());
	bool checkRet = rsa.signVerify(hash.getResult(), reqMsg->data());
	if (!checkRet)
	{
		info.status = 0;
		cout << "Sign verify failed" << endl;
	}
	else
	{
		cout << "Sign verify succeed" << endl;
		/* 生成随机字符串作为AES加密算法的秘钥, 秘钥可选长度为16, 24, 32bytes */
		string AesKey = generateAesKey(16);
		cout << "the AES_KEY is: " << AesKey << endl;

		/* 通过客户端发来的RSA公钥对AES秘钥进行加密 */
		string secAesKey = rsa.pubKeyEncrypt(AesKey);
		cout << "the AES_KEY after Encrypted is: " << secAesKey << endl;

		/* 填充服务器的应答数据 */
		info.clientID = reqMsg->clientid();
		info.serverID = reqMsg->serverid();
		info.data = secAesKey;
		info.status = 1;
	}
	/* 将要向客户端发送的数据序列化, 然后发送之 */
	CodecFactory *factory = new RespondFactory(&info);
	Codec *codec = factory->createCodec();
	string encMsg = codec->encodeMsg();
	return encMsg;
}

string ServerOP::generateAesKey(int keyLen)
{ 
	srand((unsigned int)(time(nullptr)));
	string randString(keyLen, '0');
	char randBuf[] = "~!@#$&*()+-=?";
	for (int i = 0; i < randString.length(); ++i) {
		int idx = rand() % 4;
		switch (idx)
		{
		case 0:
			randString[i] = rand() % 10 + '0';
			break;
		case 1:
			randString[i] = rand() % 26 + 'a';
			break;
		case 2:
			randString[i] = rand() % 26 + 'A';
			break;
		case 3:
			randString[i] = randBuf[rand() % strlen(randBuf)];
			break;
		default:
			break;
		}
	}
	return randString;
}

void *ServerOP::workHard(void *arg)
{
	sleep(1);
	/* 通过参数将传递的this指针转换 */
	ServerOP *op = (ServerOP *)arg;
	/* 接收客户端数据 */
	TcpSocket *socket = op->m_list[pthread_self()];
	string msg = socket->recvMsg();
	/* 将接收到的数据反序列化 */
	CodecFactory *factory = new RequestFactory(msg);
	Codec *codec = factory->createCodec();
	RequestMsg *reqMsg = (RequestMsg *)codec->decodeMsg();
	cout << "The RSA_public_key from client is: " << endl;
	cout << reqMsg->data() << endl;
	/* 释放资源 */
	delete factory;
	delete codec;
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
		break;
	case 3:
		/* 秘钥注销 */
		break;
	default:
		break;
	}
	socket->sendMsg(data);
	socket->disConnect();
	op->m_list.erase(pthread_self());

	pthread_exit(0);
}