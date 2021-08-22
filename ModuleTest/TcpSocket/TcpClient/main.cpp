/*************************************************************************
	> File Name: main.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月09日 星期一 11时01分45秒
 ************************************************************************/

#include <iostream>
#include <unistd.h>
#include "TcpSocket.h"
#include "Interface.h"
using namespace std;

int main()
{
	/* 创建套接字 */
	TcpSocket socket;
	cout << "start connection..." << endl;
	socket.connectToHost("127.0.0.1", 8888);
	cout << "connect success..." << endl;

	/* 创建加解密接口对象 */
	Interface inter("client.json");	

	/* 通信 */
	while (1) {
		string sendMsg = "hello server...";
		sendMsg = inter.encryptData(sendMsg);
		cout << "send data: " << sendMsg << endl;
		socket.sendMsg(sendMsg);
		/* 接受数据 */
		string recvMsg = socket.recvMsg(2);
		recvMsg = inter.decryptData(recvMsg);
		cout << "receice message: " << recvMsg << endl;
		sleep(1);
	}
	return 0;
}
