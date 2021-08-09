/*************************************************************************
	> File Name: TcpSocket.h
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月08日 星期日 19时34分28秒
 ************************************************************************/

#pragma once
#include <iostream>
#include <netinet/in.h>
using namespace std;

/* 超时时间 */
static const int TIME_OUT = 10000;

/* 用于通信的套接字类 */
class TcpSocket
{
public:
	/* 错误类型枚举类 */
	enum ErrorType {
		ParamError = 3001, 
		TimeoutError, 
		PeerCloseError, 
		MallocError
	};
	/* 创建一个套接字 */
	TcpSocket();
	/* 使用一个已经建立连接的套接字文件描述符进行初始化 */
	TcpSocket(int connfd);
	/* 析构函数 */
	~TcpSocket();
	/* 向指定IP和端口的进程发起连接 */
	int connectToHost(string ip, unsigned short port, int timeout = TIME_OUT);
	/* 向对方进程发送数据 */
	int sendMsg(string sendData, int timeout = TIME_OUT);
	/* 从对方进程接收数据 */
	string recvMsg(int timeout = TIME_OUT);
	/* 与对方断开连接 */
	void disConnect();

private:
	/* 设置非阻塞I/O */
	int setNonBlock(int fd);
	/* 设置阻塞I/O */
	int setBlock(int fd);
	/* 读超时检测函数，不含读操作 */
	int readTimeout(unsigned int waitSeconds);
	/* 写超时检测函数，不含写操作 */
	int writeTimeout(unsigned int waitSeconds);
	/* 带连接超时的connect函数 */
	int connectTimeout(struct sockaddr_in* addr, unsigned int waitSeconds);
	/* 从读缓冲区中连续读出n个字符 */
	int readn(void* buf, int count);
	/* 向写缓冲区中连续写入n个字符 */
	int writen(const void* buf, int count);

private:
	/* 用于通信的文件描述符 */
	int m_socket;
};