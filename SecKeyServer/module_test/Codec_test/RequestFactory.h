/*************************************************************************
	> File Name: RequestFactory.h
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月07日 星期六 15时36分26秒
 ************************************************************************/

#pragma once
#include <iostream>
#include "CodecFactory.h"
#include "Codec.h"
#include "RequestCodec.h"
using namespace std;

class RequestFactory : public CodecFactory
{
public:
	RequestFactory(string encStr);
	RequestFactory(RequestInfo* info);
	Codec* createCodec() override;
	~RequestFactory();

private:
	bool m_flag;
	string m_encStr;
	RequestInfo* m_info;
};
