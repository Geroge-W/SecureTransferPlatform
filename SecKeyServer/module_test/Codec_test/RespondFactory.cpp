/*************************************************************************
	> File Name: RespondFactory.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月07日 星期六 16时07分58秒
 ************************************************************************/

#include "RespondFactory.h"
#include "RespondCodec.h"
#include <string>
using namespace std;

RespondFactory::RespondFactory(string encStr)
{
	m_flag = false;
	m_encStr = encStr;
}

RespondFactory::RespondFactory(RespondInfo* info)
{
	m_flag = true;
	m_info = info;
}

Codec* RespondFactory::createCodec()
{
	Codec* codec = nullptr;
	if (m_flag) {
		codec = new RespondCodec(m_info);
	} else {
		codec = new RespondCodec(m_encStr);
	}
	return codec;	
}

RespondFactory::~RespondFactory()
{
}
