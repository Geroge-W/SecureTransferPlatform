/*************************************************************************
	> File Name: RequestFactory.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月07日 星期六 15时54分54秒
 ************************************************************************/

#include "RequestFactory.h"
#include <string>
using namespace std;

RequestFactory::RequestFactory(string encStr)
{
	m_flag = false;
	m_encStr = encStr;
}

RequestFactory::RequestFactory(RequestInfo* info)
{
	m_flag = true;
	m_info = info;
}

Codec* RequestFactory::createCodec()
{
	Codec* codec = nullptr;
	if (m_flag) {
		codec = new RequestCodec(m_info);
	} else {
		codec = new RequestCodec(m_encStr);
	}
	return codec;
}

RequestFactory::~RequestFactory()
{
}
