/*************************************************************************
	> File Name: CodecFactory.h
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月07日 星期六 15时27分59秒
 ************************************************************************/

#pragma once
#include "Codec.h"

class CodecFactory
{
public:
	virtual Codec* createCodec() = 0;
	virtual ~CodecFactory() {};
};