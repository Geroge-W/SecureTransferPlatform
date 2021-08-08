/*************************************************************************
	> File Name: RespondFactory.h
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月07日 星期六 16时03分06秒
 ************************************************************************/

#pragma once
#include <iostream>
#include "CodecFactory.h"
#include "RespondCodec.h"
using namespace std;

class RespondFactory : public CodecFactory
{
public:
	RespondFactory(string encStr);
	RespondFactory(RespondInfo* info);
	Codec* createCodec() override;
	~RespondFactory();

private:
	bool m_flag;
	string m_encStr;
	RespondInfo* m_info;
};