/*************************************************************************
	> File Name: ServerOP.h
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: Sun 15 Aug 2021 04:55:08 PM CST
 ************************************************************************/

#pragma once
#include <iostream>
#include <unordered_map>
#include <pthread.h>
#include "RequestMsg.pb.h"
#include "TcpSocket.h"
using namespace std;

class ServerOP
{
public:
	/* 通过json配置文件初始化服务器参数 */
	ServerOP(string json);
	~ServerOP();
	/* 启动服务器 */
	void startServer();

private:
	/* 服务器端生成用于AES对称加密的密钥 */
	string generateAesKey(int keyLen);
	/* 秘钥协商 */
	string secKeyConsult(RequestMsg* reqMsg);
	/* 工作线程回调函数 */
	static void* workHard(void* arg);

private:
	/* 服务器ID */
	string m_serverID;
	/* 服务器监听端口号 */
	unsigned short m_port;
	/* 维护每个工作线程所对应的通信对象 */
	unordered_map<pthread_t, TcpSocket*> m_list;
};