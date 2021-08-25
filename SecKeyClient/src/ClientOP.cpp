/*************************************************************************
	> File Name: ClientOP.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: Sat 14 Aug 2021 02:48:47 PM CST
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <json/json.h>
#include "ClientOP.h"
#include "Hash.h"
#include "RequestCodec.h"
#include "RequestFactory.h"
#include "RequestMsg.pb.h"
#include "RsaCrypto.h"
#include "SecKeyShm.h"
#include "TcpSocket.h"
#include "RespondCodec.h"
#include "RespondFactory.h"
#include "RespondMsg.pb.h"
using namespace std;
using namespace Json;

ClientOP::ClientOP(string jsonFile)
{
	/* 打开json文件，解析到Value对象中 */
	ifstream ifs(jsonFile);
	Reader r;
	Value root;
	if (r.parse(ifs, root) == false) {
		cout << "Json file init error" << endl;
		throw exception();
	}

	/* 根据解析出的Value对象，配置客户端参数 */
	m_info.serverID = root["ServerID"].asString();
	m_info.clientID = root["ClientID"].asString();
	m_info.serverIP = root["ServerIP"].asString();
	m_info.serverPort = root["ServerPort"].asUInt();

	/* 实例化出共享内存对象 */
	string shmKey = root["ShmKey"].asString();
	int maxNode = root["ShmMaxNode"].asInt();
	m_shm = new SecKeyShm(shmKey, maxNode);
	if (m_shm == nullptr) {
		cout << "SecKeyShm init error" << endl;
		throw exception();
	}
}

ClientOP::~ClientOP()
{
	if (m_shm != nullptr) {
		delete m_shm;
		m_shm = nullptr;
	}
}

int ClientOP::showMenu()
{
	int nSel = -1;
	printf("\n  /*************************欢迎登陆密钥协商安全传输平台*************************/");
	printf("\n  /******************************************************************************/");
	printf("\n  /*                                 1.密钥协商                                 */");
	printf("\n  /*                                 2.密钥校验                                 */");
	printf("\n  /*                                 3.密钥注销                                 */");
	printf("\n  /*                                 4.密钥查看                                 */");
	printf("\n  /*                                 0.退出系统                                 */");
	printf("\n  /******************************************************************************/");
	printf("\n  /******************************************************************************/");
	printf("\n\n  请输入您的选择:");

	/* 获取用户输入 */
	scanf("%d", &nSel);
	/* 略去用户输入的多余字符 */
	while (getchar() != '\n');

	return nSel;
}

bool ClientOP::secKeyConsult()
{
	/* 使用RSA算法生成秘钥对，将公钥字符串读出 */
	RsaCrypto rsa;
	rsa.generateKeyPair(1024);
	ifstream ifs("public.pem");
	stringstream ss;
	ss << ifs.rdbuf();		/* 将文件流重定向至输入输出流 */

	/* 准备客户端向服务端发送的数据 */
	RequestInfo reqInfo;
	reqInfo.clientID = m_info.clientID;
	reqInfo.serverID = m_info.serverID;
	reqInfo.cmdType = 1;	/* 秘钥协商 */
	reqInfo.data = ss.str();

	cout << "The RSA public key is:\n" << reqInfo.data << endl;

	/* 对原始数据进行数字签名 */
	Hash hash(T_SHA1);
	hash.addData(reqInfo.data);
	reqInfo.sign = rsa.sign(hash.getResult());	/* 对公钥的哈希值进行数字签名 */

	/* 将数据序列化 */
	CodecFactory* factory = new RequestFactory(&reqInfo);
	Codec* codec = factory->createCodec();
	string encStr = codec->encodeMsg();
	delete factory;
	delete codec;

	/* 与服务器进行通信，发送序列化后的数据 */
	TcpSocket tcp;
	cout << "ServerIP: " << m_info.serverIP << endl;
	cout << "ServerPort: " << m_info.serverPort << endl;
	int ret = tcp.connectToHost(m_info.serverIP, m_info.serverPort);
	if (ret != 0) {
		cout << "Connect to SecKeyServer error" << endl;
		return false;
	}
	cout << "Connect to SecKeyServer success" << endl;
	tcp.sendMsg(encStr);

	/* 等待接收服务器的回复数据，并解析之 */
	string recvMsg = tcp.recvMsg();
	tcp.disConnect();
	factory = new RespondFactory(recvMsg);
	codec = factory->createCodec();
	RespondMsg* resData = (RespondMsg*)codec->decodeMsg();	/* 反序列化 */
	if (resData->status() == 0) {
		cout << "Secret Key consult failed" << endl;
		delete factory;
		delete codec;
		return false;
	}

	/* 将得到的服务器发来的密文解密 */
	string aesPubKey = rsa.priKeyDecrypt(resData->data());
	cout << "The public key for AES encryption is: " << endl;
	cout << aesPubKey << endl;

	/* 将秘钥写入到共享内存中 */
	ShmNodeInfo shmInfo;
	strcpy(shmInfo.clientID, m_info.clientID.c_str());
	strcpy(shmInfo.serverID, m_info.serverID.c_str());
	strcpy(shmInfo.secKey, aesPubKey.c_str());
	shmInfo.secKeyID = resData->seckeyid();
	shmInfo.status = 1;
	m_shm->shmWrite(&shmInfo);

	delete factory;
	delete codec;

	return true; 
}

void ClientOP::secKeyCheck()
{
	/* 从共享内存中读出密钥信息 */
	ShmNodeInfo nodeInfo = m_shm->shmRead(m_info.clientID, m_info.serverID);
	if (nodeInfo.status == 0) {
		cout << "No SecKeyInfo in share memory" << endl;
		return;
	}
	cout << "The SecKey read from share memory is: " << endl;
	cout << nodeInfo.secKey << endl;

	/* 对密钥进行哈希运算 */
	Hash hash(T_SHA1);
	hash.addData(string(nodeInfo.secKey));
	string hashData = hash.getResult();

	/* 准备客户端向服务端发送的数据 */
	RequestInfo reqInfo;
	reqInfo.cmdType = 2;	/* 秘钥校验 */
	reqInfo.clientID = m_info.clientID;
	reqInfo.serverID = m_info.serverID;
	reqInfo.data = hashData;
	reqInfo.sign = string();

	/* 将数据序列化 */
	CodecFactory* factory = new RequestFactory(&reqInfo);
	Codec* codec = factory->createCodec();
	string encStr = codec->encodeMsg();
	delete factory;
	delete codec;

	/* 与服务器进行通信，发送序列化后的数据 */
	TcpSocket tcp;
	cout << "ServerIP: " << m_info.serverIP << endl;
	cout << "ServerPort: " << m_info.serverPort << endl;
	int ret = tcp.connectToHost(m_info.serverIP, m_info.serverPort);
	if (ret != 0) {
		cout << "Connect to SecKeyServer error" << endl;
		return;
	}
	cout << "Connect to SecKeyServer success" << endl;
	tcp.sendMsg(encStr);

	/* 等待接收服务器的回复数据，并解析之 */
	string recvMsg = tcp.recvMsg();
	tcp.disConnect();
	factory = new RespondFactory(recvMsg);
	codec = factory->createCodec();
	RespondMsg* resData = (RespondMsg*)codec->decodeMsg();	/* 反序列化 */
	if (resData->status() == 0) {
		cout << "SecKey check fail" << endl;
	}
	else {
		cout << "SecKey check succeed" << endl;
	}
	delete factory;
	delete codec;
}

void ClientOP::secKeyCancel()
{
	/* 从共享内存中读出密钥信息 */
	ShmNodeInfo nodeInfo = m_shm->shmRead(m_info.clientID, m_info.serverID);
	if (nodeInfo.status == 0) {
		cout << "No SecKeyInfo in share memory" << endl;
		return;
	}
	cout << "The SecKey read from share memory is: " << endl;
	cout << nodeInfo.secKey << endl;
	cout << "Are you sure to log off the SecKey?(Yes->1, No->0)" << endl;

	/* 获取用户输入 */
	int nSel = -1;
	scanf("%d", &nSel);
	/* 略去用户输入的多余字符 */
	while (getchar() != '\n');

	if (nSel != 1) {
		return;
	}

	/* 准备客户端向服务器发送的数据 */
	RequestInfo reqInfo;
	reqInfo.cmdType = 3;	/* 密钥注销 */
	reqInfo.clientID = m_info.clientID;
	reqInfo.serverID = m_info.serverID;
	reqInfo.data = to_string(nodeInfo.secKeyID);	/* 向服务器发送该密钥的编号 */
	reqInfo.sign = string();

	/* 将数据序列化 */
	CodecFactory* factory = new RequestFactory(&reqInfo);
	Codec* codec = factory->createCodec();
	string encStr = codec->encodeMsg();
	delete factory;
	delete codec;

	/* 与服务器进行通信，发送序列化后的数据 */
	TcpSocket tcp;
	cout << "ServerIP: " << m_info.serverIP << endl;
	cout << "ServerPort: " << m_info.serverPort << endl;
	int ret = tcp.connectToHost(m_info.serverIP, m_info.serverPort);
	if (ret != 0) {
		cout << "Connect to SecKeyServer error" << endl;
		return;
	}
	cout << "Connect to SecKeyServer success" << endl;
	tcp.sendMsg(encStr);

	/* 等待接收服务器的回复数据，并解析之 */
	string recvMsg = tcp.recvMsg();
	tcp.disConnect();
	factory = new RespondFactory(recvMsg);
	codec = factory->createCodec();
	RespondMsg* resData = (RespondMsg*)codec->decodeMsg();	/* 反序列化 */
	if (resData->status() == 0) {
		cout << "SecKey cancel fail" << endl;
	}
	else {
		m_shm->delShm();
		cout << "SecKey cancel succeed" << endl;
	}

	delete factory;
	delete codec;
}

void ClientOP::secKeyView()
{

}