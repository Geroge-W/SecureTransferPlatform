/*************************************************************************
	> File Name: BaseShm.h
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月09日 星期一 19时28分44秒
 ************************************************************************/

#pragma once
#include <iostream>
using namespace std;

class BaseShm
{
public:
	/* 通过key打开共享内存 */
	BaseShm(int key);
	/* 通过传递进来的key创建或打开共享内存 */
	BaseShm(int key, int size);
	/* 通过文件路径打开共享内存 */
	BaseShm(string name);
	/* 通过文件路径创建或打开共享内存 */
	BaseShm(string name, int size);
	/* 关联当前进程与共享内存 */
	void* mapShm();
	/* 取消关联共享内存 */
	int unmapShm();
	/* 销毁共享内存 */
	int delShm();
	/* 析构函数 */
	~BaseShm();

private:
	int getShmID(key_t key, int shmSize, int flag);

private:
	int m_shmID;

protected:
	void* m_shmAddr = nullptr;
};