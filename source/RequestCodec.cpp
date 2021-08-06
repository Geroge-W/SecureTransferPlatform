/*************************************************************************
	> File Name: RequestCodec.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月06日 星期五 20时19分42秒
 ************************************************************************/

#include <iostream>
#include "RequestCodec.h"
#include "RequestMsg.pb.h"
using namespace std;

RequestCodec::RequestCodec()
{
}

RequestCodec::RequestCodec(string encStr)
{
	initMessage(encStr);
}

RequestCodec::RequestCodec(RequestInfo* info)
{
	initMessage(info);
}

void RequestCodec::initMessage(string encStr)
{
	m_encStr = encStr;
}

void RequestCodec::initMessage(RequestInfo* info)
{
	m_msg.set_cmdtype(info->cmdType);
	m_msg.set_clientid(info->clientID);
	m_msg.set_serverid(info->serverID);
	m_msg.set_sign(info->sign);
	m_msg.set_data(info->data);
}

string RequestCodec::encodeMsg()
{
	string output;
	m_msg.SerializeToString(&output);
	return output;
}

void* RequestCodec::decodeMsg()
{
	m_msg.ParseFromString(m_encStr);
	return &m_msg;
}

RequestCodec::~RequestCodec()
{
}
