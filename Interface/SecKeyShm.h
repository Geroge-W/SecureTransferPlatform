/*************************************************************************
	> File Name: SecKeyShm.h
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月09日 星期一 20时14分01秒
 ************************************************************************/

#pragma once
#include <iostream>
#include "BaseShm.h"
#include "ShmNodeInfo.h"
using namespace std;

class SecKeyShm : public BaseShm
{
public:
	/* 根据key值，创建具有maxNode个节点信息的共享内存 */
	SecKeyShm(int key, int maxNode);
	/* 根据路径，创建具有maxNode个节点信息的共享内存 */
	SecKeyShm(string pathName, int maxNode);
	/* 析构函数 */
	~SecKeyShm();
	/* 将共享内存所在缓冲区数据初始化为0 */
	void shmInit();
	/* 向共享内存中写入节点信息 */
	int shmWrite(ShmNodeInfo* pNodeInfo);
	/* 读取对应的节点信息 */
	ShmNodeInfo shmRead(string clientID, string serverID);

private:
	int m_maxNode;
};