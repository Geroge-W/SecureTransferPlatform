/*************************************************************************
	> File Name: main.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: Sun 15 Aug 2021 08:19:21 PM CST
 ************************************************************************/

#include <iostream>
#include "ServerOP.h"
using namespace std;

int main()
{
	ServerOP op("server.json");
	op.startServer();
	return 0;
}