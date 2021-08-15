/*************************************************************************
	> File Name: TcpServer.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月09日 星期一 09时59分35秒
 ************************************************************************/

#include <iostream>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <sys/select.h>
#include <errno.h>
#include "TcpServer.h"
using namespace std;

TcpServer::TcpServer()
{
}

TcpServer::~TcpServer()
{
}

int TcpServer::setListen(unsigned short port)
{
	int ret = 0;
	struct sockaddr_in serverAddr;
	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);	

	/* 创建监听的套接字 */
	m_lfd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_lfd == -1) {
		ret = errno;
		return ret;
	}
	/* 设置端口复用 */
	int on = 1;
	ret = setsockopt(m_lfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (ret == -1) {
		ret = errno;
		return ret;
	}
	/* 绑定地址结构 */
	ret = bind(m_lfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if (ret == -1) {
		ret = errno;
		return ret;
	}
	/* 设置监听 */
	ret = listen(m_lfd, 128);
	if (ret == -1) {
		ret = errno;
		return ret;
	}
	return ret;
}

TcpSocket* TcpServer::acceptConn(int waitSeconds)
{
	int ret;
	/* 若设置等待超时时间,则通过select监听读事件 */
	if (waitSeconds > 0) {
		fd_set accept_fdset;
		struct timeval timeout;

		FD_ZERO(&accept_fdset);
		FD_SET(m_lfd, &accept_fdset);
		timeout.tv_sec = waitSeconds;
		timeout.tv_usec = 0;
		do {
			ret = select(m_lfd + 1, &accept_fdset, nullptr, nullptr, &timeout);
		} while (ret < 0 && errno == EINTR);
		if (ret <= 0) {
			return nullptr;
		}
	}
	/* 若select检测到m_lfd上有读事件发生，说明有新的连接到来 */
	struct sockaddr_in clientAddr;
	socklen_t addrLen = sizeof(clientAddr);
	int connfd = accept(m_lfd, (struct sockaddr*)&clientAddr, &addrLen);
	if (connfd == -1) {
		return nullptr;
	}
	return new TcpSocket(connfd);
}

void TcpServer::closefd()
{
	close(m_lfd);
}