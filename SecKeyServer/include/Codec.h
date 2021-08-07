/*************************************************************************
	> File Name: Codec.h
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月06日 星期五 19时15分55秒
 ************************************************************************/

#pragma once
#include <iostream>
#include <string>

class Codec
{
public:
	virtual std::string encodeMsg() = 0;
	virtual void* decodeMsg() = 0;
	virtual ~Codec() = 0;
};
