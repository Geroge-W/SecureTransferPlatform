/*************************************************************************
	> File Name: RequestCodec.h
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月06日 星期五 19时32分01秒
 ************************************************************************/

#include <iostream>
#include <string>
#include "Codec.h"
#include "RequestMsg.pb.h"
using namespace std;

struct RequestInfo
{
	int cmdType;
	string clientID;
	string serverID;
	string sign;
	string data;
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
