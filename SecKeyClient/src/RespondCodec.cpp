/*************************************************************************
	> File Name: RespondCodec.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月07日 星期六 12时11分04秒
 ************************************************************************/

#include <iostream>
#include <string>
#include "RespondCodec.h"
using namespace std;

RespondCodec::RespondCodec()
{
}

RespondCodec::RespondCodec(string encStr)
{
	initMessage(encStr);
}

RespondCodec::RespondCodec(RespondInfo* info)
{
	initMessage(info);
}

void RespondCodec::initMessage(string encStr)
{
	m_encStr = encStr;
}

void RespondCodec::initMessage(RespondInfo* info)
{
	m_msg.set_status(info->status);
	m_msg.set_seckeyid(info->secKeyID);
	m_msg.set_clientid(info->clientID);
	m_msg.set_serverid(info->serverID);
	m_msg.set_data(info->data);
}

string RespondCodec::encodeMsg()
{
	string output;
	m_msg.SerializeToString(&output);
	return output;
}

void* RespondCodec::decodeMsg()
{
	m_msg.ParseFromString(m_encStr);
	return &m_msg;
}

RespondCodec::~RespondCodec()
{
}
