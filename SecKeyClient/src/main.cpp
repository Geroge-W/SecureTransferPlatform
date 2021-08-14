/*************************************************************************
	> File Name: main.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: Sat 14 Aug 2021 04:38:27 PM CST
 ************************************************************************/

#include <iostream>
#include <ClientOP.h>
using namespace std;

int main()
{
	ClientOP op("config.json");
	while (true)
	{
		int ret = op.showMenu();
		switch (ret)
		{
		case 1:
			op.secKeyConsult();
			break;
		case 2:
			op.secKeyCheck();
			break;
		case 3:
			op.secKeyCancel();
			break;
		default:
			break;
		}
	}
	cout << "Bye!" << endl;
	exit(0);
}