/*************************************************************************
	> File Name: AesCrypto.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: Fri 13 Aug 2021 03:07:10 PM CST
 ************************************************************************/

#include <iostream>
#include "AesCrypto.h"
using namespace std;

AesCrypto::AesCrypto(string key)
{
	int keyLen = key.length();
	if (keyLen == 16 || keyLen == 24 || keyLen == 32) {
		unsigned char* aesKey = (unsigned char*)key.c_str();
		AES_set_encrypt_key(aesKey, keyLen * 8, &m_encKey);
		AES_set_decrypt_key(aesKey, keyLen * 8, &m_decKey);
		m_key = key;
	}
}

AesCrypto::~AesCrypto()
{
}

string AesCrypto::cbcEncrypt(string oriStr)
{
	/* +1是添加\0的长度 */
	int length = oriStr.length() + 1;
	/* 传入的length需要转换为16的整数倍 */
	if (length % 16 != 0) {
		length = (length / 16 + 1) * 16;
	}
	char* out = new char[length];
	unsigned char iVec[AES_BLOCK_SIZE];
	generateIvec(iVec);
	AES_cbc_encrypt((unsigned char*)oriStr.c_str(), (unsigned char*)out,
		length, &m_encKey, iVec, AES_ENCRYPT);
	
	string result = string(out, length);
	delete [] out;
	return result;
}

string AesCrypto::cbcDecrypt(string encStr)
{
	int length = encStr.length();
	/* AES使用CBC模式加密后的密文长度一定是16的整数倍 */
	if (length % 16 != 0) {
		return string();
	}
	char* out = new char[length];
	unsigned char iVec[AES_BLOCK_SIZE];
	generateIvec(iVec);
	AES_cbc_encrypt((unsigned char*)encStr.c_str(), (unsigned char*)out,
		length, &m_decKey, iVec, AES_DECRYPT);
	string result = string(out);
	delete [] out;
	return result;
}

void AesCrypto::generateIvec(unsigned char* iVec)
{
	for (int i = 0; i < AES_BLOCK_SIZE; ++i) {
		iVec[i] = m_key.at(AES_BLOCK_SIZE - i - 1);
	}
}