/*************************************************************************
	> File Name: RequestCodec.h
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月06日 星期五 19时32分01秒
 ************************************************************************/

#pragma once
#include <iostream>
#include <string>
#include "Codec.h"
#include "RequestMsg.pb.h"
using namespace std;

/* 客户端与服务端进行通信的数据结构 */
struct RequestInfo
{
	/* 客户端的请求类型（1->秘钥协商 2->秘钥检验 3->秘钥注销 4->秘钥查看）*/
	int cmdType;
	/* 所有进行通信的客户端都会被分配一个有效的ID */
	string clientID;
	/* 客户端要进行通信的服务器的ID */
	string serverID;
	/* 业务数据，根据cmdType的不同而不同 */
	string data;
	/* 对业务数据的数字签名，供服务端验证数据是否被篡改 */
	string sign;
};

class RequestCodec : public Codec
{
public:	
	RequestCodec();
	RequestCodec(string encStr);
	RequestCodec(RequestInfo* info);
	void initMessage(string encStr);
	void initMessage(RequestInfo* info);
	string encodeMsg() override;
	void* decodeMsg() override;
	~RequestCodec();

private:
	string m_encStr;
	RequestMsg m_msg;
};