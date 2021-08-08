/*************************************************************************
	> File Name: TcpSocket.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月08日 星期日 19时34分38秒
 ************************************************************************/

#include <iostream>
#include <sys/socket.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "TcpSocket.h"
using namespace std;

TcpSocket::TcpSocket()
{
}

TcpSocket::TcpSocket(int connfd)
{
	m_socket = connfd;
}

TcpSocket::~TcpSocket()
{
}

int TcpSocket::connectToHost(string ip, unsigned short port, int timeout)
{
	int ret = 0;
	/* 参数检查 */
	if (port <= 0 || port > 65535 || timeout < 0) {
		ret = ParamError;
		return ret;
	}
	/* 建立套接字 */
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket < 0) {
		ret = errno;
		return ret;
	}
	/* 初始化地址结构 */
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) == 0) {
		ret = ParamError;
		return ret;
	}
	/* 向对方进程发起连接 */
	ret = connectTimeout(&serverAddr, timeout);
	if (ret < 0) {
		if (ret == -1 && errno == ETIMEDOUT) {
			ret = TimeoutError;
			return ret;
		} else {
			return errno;
		}
	}
	return ret;
}

int TcpSocket::sendMsg(string sendData, int timeout)
{
	int ret = writeTimeout(timeout);
	/* 没超时 */
	if (ret == 0) {
		int writed = 0;
		int dataLen = sendData.size() + sizeof(int);
		/* 添加的4字节作为数据头部信息，存储分组长度 */
		unsigned char* netData = (unsigned char*)malloc(dataLen);
		if (netData == nullptr) {
			ret = MallocError;
			return ret;
		}
		/* 主机字节序 -> 网络字节序 */
		int netLen = htonl(sendData.size());
		memcpy(netData, &netLen, sizeof(int));
		memcpy(netData + sizeof(int), sendData.c_str(), sendData.size());

		writed = writen(netData, dataLen);
		if (writed < dataLen) {		/* 发送失败 */
			if (netData != nullptr) {
				free(netData);
				netData = nullptr;
			}
			return writed;
		}
		if (netData != nullptr) {
			free(netData);
			netData = nullptr;
		}
	}
	/* 发生超时 */
	else
	{
		if (ret == -1 && errno == ETIMEDOUT) {
			ret = TimeoutError;
		}
	}
	return ret;
}

string TcpSocket::recvMsg(int timeout)
{
	int ret = readTimeout(timeout);
	/* 超时或有异常 */
	if (ret != 0) {
		if (ret == -1 && errno == ETIMEDOUT) {
			fprintf(stderr, "readTimeout error: TimeoutError\n");
			return string();
		}
		else {
			fprintf(stderr, "readTimeout error: %d\n", ret);
			return string();
		}
	}
	int netDataLen = 0;
	/* 读取报文首部字段 */
	ret = readn(&netDataLen, sizeof(int));
	if (ret == -1) {
		fprintf(stderr, "readn error: %d\n", ret);
		return string();
	}
	else if (ret < 4) {
		fprintf(stderr, "readn error peer closed: %d\n", ret);
		return string();
	}
	/* 网络字节序 -> 主机字节序 */
	int n = ntohl(netDataLen);
	/* 根据报文首部中记录的数据长度申请内存，接收数据 */
	char* tmpBuf = (char*)malloc(n + 1);
	if (tmpBuf == nullptr) {
		ret = MallocError;
		fprintf(stderr, "recvMsg maloc error\n");
		return string();
	}
	/* 接收报文数据字段 */
	ret = readn(tmpBuf, n);
	if (ret == -1) {
		fprintf(stderr, "readn error: %d\n", ret);
		return string();
	} else if (ret < n) {
		fprintf(stderr, "readn error peer closed: %d\n", ret);
		return string();
	}
	tmpBuf[n] = '\0';
	string data = string(tmpBuf);
	free(tmpBuf);

	return data;
}

/*
* setNonBlock - 设置I/O为非阻塞模式
* @fd: 文件描符符
*/
int TcpSocket::setNonBlock(int fd)
{
	int oldOption = fcntl(fd, F_GETFL);
	if (oldOption == -1) {
		return -1;
	}
	int newOption = oldOption | O_NONBLOCK;
	int ret = fcntl(fd, F_SETFL, newOption);
	if (ret == -1) {
		return -1;
	}
	return oldOption;
}

/*
* setBlock - 设置I/O为阻塞模式
* @fd: 文件描符符
*/
int TcpSocket::setBlock(int fd)
{
	int oldOption = fcntl(fd, F_GETFL);
	if (oldOption == -1) {
		return -1;
	}
	int newOption = oldOption & ~O_NONBLOCK;
	int ret = fcntl(fd, F_SETFL, newOption);
	if (ret == -1) {
		return -1;
	}
	return oldOption;
}

/*
* readTimeout - 读超时检测函数，不含读操作
* @waitSeconds: 等待超时秒数，如果为0表示不检测超时
* 成功（未超时）返回0，失败返回-1，超时返回-1并且errno = ETIMEDOUT
*/
int TcpSocket::readTimeout(unsigned int waitSeconds)
{

}

/*
* writeTimeout - 写超时检测函数，不含写操作
* @waitSeconds: 等待超时秒数，如果为0表示不检测超时
* 成功（未超时）返回0，失败返回-1，超时返回-1并且errno = ETIMEDOUT
*/
int TcpSocket::writeTimeout(unsigned int waitSeconds)
{
	int ret = 0;
	if (waitSeconds > 0) {
		fd_set write_fdset;
		struct timeval timeout;

		FD_ZERO(&write_fdset);
		FD_SET(m_socket, &write_fdset);
		timeout.tv_sec = waitSeconds;
		timeout.tv_usec = 0;
		do {
			ret = select(m_socket + 1, nullptr, &write_fdset, nullptr, &timeout);
		} while (ret < 0 && errno == EINTR);
		if (ret == 0) {
			ret = -1;
			errno = ETIMEDOUT;
		}	
		else if (ret == 1) {
			ret = 0;
		}
	}
	return ret;
}

/*
* connectTimeout - connect
* @addr: 要连接的对方地址
* @waitSeconds: 等待超时秒数，如果为0表示阻塞连接
* 成功（未超时）返回0，失败返回-1，超时返回-1并且errno = ETIMEDOUT
*/
int TcpSocket::connectTimeout(struct sockaddr_in* addr, unsigned int waitSeconds)
{
	int ret = 0;
	socklen_t addrLen = sizeof(struct sockaddr_in);
	if (waitSeconds > 0) {
		setNonBlock(m_socket);
	}
	ret = connect(m_socket, (struct sockaddr*)addr, addrLen);
	if (ret < 0 && errno == EINPROGRESS)
	{
		/* 以非阻塞方式连接返回EINPROGRESS, 表示连接还在进行中 */
		fd_set connect_fdset;
		struct timeval timeout;
		FD_ZERO(&connect_fdset);
		FD_SET(m_socket, &connect_fdset);
		timeout.tv_sec = waitSeconds;
		timeout.tv_usec = 0;
		/* 通过select判断m_socket是否可写，若可写，说明连接完成了 */
		do {
			ret = select(m_socket + 1, nullptr, &connect_fdset, nullptr, &timeout);
		} while (ret < 0 && errno == EINTR);
		/* 若到了超时时间没有可写事件发生，返回超时错误 */
		if (ret == 0)
		{
			ret = -1;
			errno = ETIMEDOUT;
		}
		/* select函数调用出错 */
		else if (ret < 0)
		{
			setBlock(m_socket);
			return -1;
		}
		/* ret返回为1（表示套接字可写），可能有两种情况，一种是连接建立成功，一种是套接字产生错误 */
		/* 此时错误信息不会保存至errno变量中，因此，需要调用getsockopt来获取。 */
		else if (ret == 1)
		{
			int err;
			socklen_t sockLen = sizeof(err);
			int sockoptRet = getsockopt(m_socket, SOL_SOCKET, SO_ERROR, &err, &sockLen);
			if (sockoptRet == -1)
			{
				setBlock(m_socket);
				return -1;
			}
			if (err == 0)
			{
				ret = 0; /* 成功建立连接 */
			}
			else
			{
				errno = err;
				ret = -1;
			}
		}
	}
	if (waitSeconds > 0) {
		setBlock(m_socket);
	}
	return ret;
}

/*
* readn - 读取固定字节数
* @fd: 文件描述符
* @buf: 接收缓冲区
* @count: 要读取的字节数
* 成功返回count，失败返回-1，读到EOF返回<count
*/
int TcpSocket::readn(void* buf, int count)
{
	
}

/*
* writen - 发送固定字节数
* @buf: 发送缓冲区
* @count: 要读取的字节数
* 成功返回count，失败返回-1
*/
int TcpSocket::writen(const void* buf, int count)
{
	size_t nleft = count;
	ssize_t nwritten;
	char* bufp = (char*)buf;

	while (nleft > 0) {
		if ((nwritten = write(m_socket, bufp, nleft)) < 0) {
			if (errno == EINTR) {	/* 被信号打断 */
				continue;
			}
			return -1;
		}
		else if (nwritten == 0) {
			continue;
		}
		bufp += nwritten;
		nleft -= nwritten;
	}
	return count;
}