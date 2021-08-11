/*************************************************************************
	> File Name: main.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: Wed 11 Aug 2021 09:45:05 PM CST
 ************************************************************************/

#include <iostream>
#include "Hash.h"
using namespace std;

int main()
{
	Hash hash(T_SHA384);
	hash.addData("lbw is the best!");
	string ret = hash.getResult();
	cout << ret << endl;
	return 0;
}
