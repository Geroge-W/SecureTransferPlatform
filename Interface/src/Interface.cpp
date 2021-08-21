/*************************************************************************
	> File Name: Interface.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: Thu 19 Aug 2021 11:07:37 PM CST
 ************************************************************************/

#include <iostream>
#include <fstream>
#include <json/json.h>
#include "Interface.h"
#include "SecKeyShm.h"
#include "AesCrypto.h"
using namespace std;

Interface::Interface(string json)
{
	/* 解析json配置文件 */
	/* 1.得到流对象 -> 读文件 */
	ifstream ifs(json);
	/* 2.创建json Reader */
	Json::Reader rd;
	/* 3.调用Reader对象parse，初始化一个Value对象 */
	Json::Value root;
	rd.parse(ifs, root);
	/* 4.提取出Value对象中的数据 */
	string shmKey = root["ShmKey"].asString();
	string serverID = root["ServerID"].asString();
	string clientID = root["ClientID"].asString();
	int maxNode = root["MaxNode"].asUInt();

	/* 从共享内存中读取出密钥 */
	SecKeyShm shm(shmKey, maxNode);
	ShmNodeInfo node = shm.shmRead(clientID, serverID);
	m_key = string(node.secKey);
}

Interface::~Interface()
{
}

string Interface::encryptData(string data)
{
	AesCrypto aes(m_key);
	string encData = aes.cbcEncrypt(data);
	return encData;
}

string Interface::decryptData(string data)
{
	AesCrypto aes(m_key);
	return aes.cbcDecrypt(data);
}