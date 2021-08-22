/*************************************************************************
	> File Name: main.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月09日 星期一 10时45分45秒
 ************************************************************************/

#include <iostream>
#include <pthread.h>
#include "TcpServer.h"
#include "Interface.h"
using namespace std;

static Interface inter("server.json");

static void* callback(void* arg)
{
	TcpSocket* sockTcp = (TcpSocket*)arg;
	/* 通信 */
	while (1) {
		string msg = sockTcp->recvMsg();
		if (msg.empty()) {
			break;
		}
		msg = inter.decryptData(msg);
		cout << "recvive message: " << msg << endl;
		/* 发送数据 */
		string sendMsg = "Are you OK?";
		sendMsg = inter.encryptData(sendMsg);
		sockTcp->sendMsg(sendMsg);
	}
	sockTcp->disConnect();
	delete sockTcp;
	pthread_exit(nullptr);
}

int main()
{
	/* 创建对象 */
	TcpServer* server = new TcpServer;
	/* 设置监听 */
	server->setListen(8888);
	/* 等待并接受连接请求 */
	while (true) {
		cout << "accept wait..." << endl;
		TcpSocket* sockTcp = server->acceptConn();
		cout << "accept a request..." << endl;
		if (sockTcp != nullptr) {
			pthread_t tid;
			pthread_create(&tid, nullptr, callback, sockTcp);
			pthread_detach(tid);
		}
	}
	delete server;
	exit(0);
}