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
	/* 对客户端所发来请求的处理状态 */
	int status;
	/* 只有在秘钥协商生成新秘钥时才有用 */
	int secKeyID;
	/* 秘钥协商客户端的ID */
	string clientID;
	/* 秘钥协商服务器的ID */
	string serverID;
	/* 针对业务逻辑处理得到的数据 */
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
