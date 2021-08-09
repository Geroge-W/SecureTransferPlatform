/*************************************************************************
	> File Name: SecKeyShm.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月09日 星期一 20时20分32秒
 ************************************************************************/

#include <iostream>
#include <cstring>
#include "SecKeyShm.h"
using namespace std;

SecKeyShm::SecKeyShm(int key, int maxNode)
	: BaseShm(key, maxNode * sizeof(ShmNodeInfo))
	, m_maxNode(maxNode)
{
}

SecKeyShm::SecKeyShm(string pathName, int maxNode)
	: BaseShm(pathName, maxNode * sizeof(ShmNodeInfo))
	, m_maxNode(maxNode)
{
}

SecKeyShm::~SecKeyShm()
{
}

void SecKeyShm::shmInit()
{
	if (m_shmAddr != nullptr) {
		memset(m_shmAddr, 0, m_maxNode * sizeof(ShmNodeInfo));	
	}
}

int SecKeyShm::shmWrite(ShmNodeInfo* pNodeInfo)
{
	int ret = -1;
	/* 关联至共享内存 */
	ShmNodeInfo* pAddr = static_cast<ShmNodeInfo*>(mapShm());
	if (pAddr == nullptr) {
		return ret;
	}
	/* 判断传入的网点秘钥是否已经存在 */
	ShmNodeInfo* pNode = nullptr;
	for (int i = 0; i < m_maxNode; ++i) {
		/* pNode依次指向共享内存中每个节点的首地址 */
		pNode = pAddr + i;
		/* Debug 输出 */
		cout << i << endl;
		cout << "clientID compare: " << pNode->clientID << " - " << pNodeInfo->clientID << endl;
		cout << "serverID compare: " << pNode->serverID << " - " << pNodeInfo->serverID << endl;
		cout << endl;
		/* 如果原网点秘钥已经存在，则使用新秘钥覆盖旧值 */
		if (strcmp(pNode->clientID, pNodeInfo->clientID) == 0 &&
			strcmp(pNode->serverID, pNodeInfo->serverID) == 0)
		{
			memcpy(pNode, pNodeInfo, sizeof(ShmNodeInfo));
			unmapShm();
			cout << "data write success, original data is overwritten" << endl;
			return 0;
		}
	}
	/* 若没有找到对应信息，则找一个空节点将秘钥信息写入 */
	int i = 0;
	ShmNodeInfo tmpNodeInfo;
	for (i = 0; i < m_maxNode; ++i) {
		pNode = pAddr + i;
		if (memcmp(&tmpNodeInfo, pNode, sizeof(ShmNodeInfo)) == 0) {
			ret = 0;
			memcpy(pNode, pNodeInfo, sizeof(ShmNodeInfo));
			cout << "data write success, add data on the new node" << endl;
			break;
		}
	}
	if (i == m_maxNode) {
		ret = -1;
	}
	unmapShm();
	return ret;
}

ShmNodeInfo SecKeyShm::shmRead(string clientID, string serverID)
{

}