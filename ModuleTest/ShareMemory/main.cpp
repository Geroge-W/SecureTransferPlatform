/*************************************************************************
	> File Name: main.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: Wed 11 Aug 2021 10:13:19 AM CST
 ************************************************************************/

#include <iostream>
#include <cstring>
#include "BaseShm.h"
#include "SecKeyShm.h"
#include "ShmNodeInfo.h"
using namespace std;

static void writeShm()
{
	ShmNodeInfo info;
	strcpy(info.clientID, "levin");
	strcpy(info.serverID, "Alice");
	strcpy(info.secKey, "Hello, thank you");
	info.secKeyID = 666;
	info.status = 1;
	SecKeyShm shm("/home", 10);
	shm.shmInit();
	shm.shmWrite(&info);

	strcpy(info.clientID, "Bob");
	strcpy(info.serverID, "Caler");
	strcpy(info.secKey, "Hello, thank very much");
	info.secKeyID = 555;
	info.status = 1;
	shm.shmWrite(&info);
	shm.unmapShm();
}

static void readShm()
{
	SecKeyShm shm("/home", 10);
	ShmNodeInfo info = shm.shmRead("levin", "Alice");
	cout << "ClientID: " << info.clientID << endl;
	cout << "ServerID: " << info.serverID << endl;
	cout << "SecKey: " << info.secKey << endl;
	cout << "SecKeyID: " << info.secKeyID << endl;
	cout << "status: " << info.status << endl;
	shm.unmapShm();
	shm.delShm();
}

int main()
{
	cout << "Please input your operation: " << endl;
	cout << "1.write	2.read" << endl;
	int ret = 0;
	cin >> ret;
	switch (ret)
	{
	case 1:
		writeShm();
		break;
	case 2:
		readShm();
		break;
	default:
		break;
	}
	cout << "Bye!" << endl;
	exit(0);
}