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
#include "MySQLOP.h"
using namespace std;

class ServerOP
{
public:
	/* 生成AES公钥长度枚举类 */
	enum KeyLen { Len16 = 16, Len24 = 24, Len32 = 32 };
	/* 通过json配置文件初始化服务器参数 */
	ServerOP(string json);
	~ServerOP();
	/* 启动服务器 */
	void startServer();

private:
	/* 服务器端生成用于AES对称加密的密钥 */
	string generateAesKey(KeyLen len);
	/* 秘钥协商 */
	string secKeyConsult(RequestMsg* reqMsg);
	/* 工作线程回调函数 */
	static void* workHard(void* arg);

private:
	/* 服务器ID */
	string m_serverID;
	/* 服务器监听端口号 */
	unsigned short m_port;
	/* 数据库用户名 */
	string m_dbUser;
	/* 数据库密码 */
	string m_dbPwd;
	/* 数据库名称 */
	string m_dbName;
	/* 数据库实例对象 */
	MySQLOP m_mysql;


};

/* 主线程向工作线程传递的参数 */
struct ThreadArg
{
	ThreadArg(ServerOP* op, TcpSocket* sock): sop(op), socket(sock){};
	ServerOP* sop;
	TcpSocket* socket;
};