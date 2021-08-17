/*************************************************************************
	> File Name: ClientOP.h
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: Sat 14 Aug 2021 02:42:01 PM CST
 ************************************************************************/

#pragma once
#include <iostream>
#include "SecKeyShm.h"
using namespace std;

struct ClientInfo
{
	string clientID;
	string serverID;
	string serverIP;
	unsigned short serverPort;
};

class ClientOP
{
private:
	ClientInfo m_info;
	SecKeyShm* m_shm;

public:
	/* 通过解析json文件配置客户端参数 */
	ClientOP(string jsonFile);
	~ClientOP();

	/* 显示菜单 */
	int showMenu();
	/* 秘钥协商 */
	bool secKeyConsult();
	/* 秘钥校验 */
	void secKeyCheck();
	/* 秘钥注销 */
	void secKeyCancel();

};