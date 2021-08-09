/*************************************************************************
	> File Name: RespondCodec.h
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月07日 星期六 11时51分49秒
 ************************************************************************/

#pragma once
#include <iostream>
#include <string>
#include "Codec.h"
#include "RespondMsg.pb.h"
using namespace std;

struct RespondInfo
{
	int status;
	int secKeyID;
	string clientID;
	string serverID;
	string data;
};

class RespondCodec : public Codec
{
public:
	RespondCodec();
	RespondCodec(string encStr);
	RespondCodec(RespondInfo* info);
	void initMessage(string encStr);
	void initMessage(RespondInfo* info);
	string encodeMsg() override;
	void* decodeMsg() override;
	~RespondCodec();

private:
	string m_encStr;
	RespondMsg m_msg;
};
