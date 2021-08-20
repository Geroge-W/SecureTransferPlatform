/*************************************************************************
	> File Name: BaseShm.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月09日 星期一 19时28分50秒
 ************************************************************************/

#include "BaseShm.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <cstdio>
#include <string>
using namespace std;

const char RandX = 'x';
BaseShm::BaseShm(int key)
{
	getShmID(key, 0, 0);
}

BaseShm::BaseShm(int key, int size)
{
	getShmID(key, size, IPC_CREAT | 0664);
}

BaseShm::BaseShm(string name)
{
	key_t key = ftok(name.c_str(), RandX);
	getShmID(key, 0, 0);
}

BaseShm::BaseShm(string name, int size)
{
	key_t key = ftok(name.c_str(), RandX);
	getShmID(key, size, IPC_CREAT | 0664);
}

void* BaseShm::mapShm()
{
	m_shmAddr = shmat(m_shmID, nullptr, 0);
	if (m_shmAddr == (void*)-1) {
		return nullptr;
	}
	return m_shmAddr;
}

int BaseShm::unmapShm()
{
	int ret = shmdt(m_shmAddr);
	return ret;
}

int BaseShm::delShm()
{
	int ret = shmctl(m_shmID, IPC_RMID, nullptr);
	return ret;
}

BaseShm::~BaseShm()
{
}

int BaseShm::getShmID(key_t key, int shmSize, int flag)
{
	cout << "share memory size: " << shmSize << endl;
	m_shmID = shmget(key, shmSize, flag);
	cout << "m_shmID = " << m_shmID << endl;
	if (m_shmID == -1 && errno != EEXIST) {
		perror("shmget error");
	}	
	return m_shmID;
}