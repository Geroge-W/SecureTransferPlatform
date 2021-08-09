/*************************************************************************
	> File Name: main.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月09日 星期一 11时01分45秒
 ************************************************************************/

#include <iostream>
#include <unistd.h>
#include "TcpSocket.h"
using namespace std;

int main()
{
	/* 创建套接字 */
	TcpSocket socket;
	cout << "start connection..." << endl;
	socket.connectToHost("127.0.0.1", 9999);
	cout << "connect success..." << endl;

	/* 通信 */
	while (1) {
		string sendMsg = "hello server...";
		cout << "send data: " << sendMsg << endl;
		socket.sendMsg(sendMsg);
		/* 接受数据 */
		int recvLen = -1;
		string recvMsg = socket.recvMsg(2);
		cout << "receice message: " << recvMsg << endl;
		sleep(1);
	}
	return 0;
}
