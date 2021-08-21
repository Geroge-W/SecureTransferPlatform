/*************************************************************************
	> File Name: AesCrypto.h
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: Fri 13 Aug 2021 03:01:44 PM CST
 ************************************************************************/

#pragma once
#include <iostream>
#include <openssl/aes.h>
using namespace std;

class AesCrypto
{
public:
	/* 可以使用16bytes, 24bytes, 32bytes的秘钥 */
	AesCrypto(string key);
	~AesCrypto();

	/* 使用AES算法加密 */
	string cbcEncrypt(string oriStr);
	/* 对应解密算法 */
	string cbcDecrypt(string encStr);

private:
	void generateIvec(unsigned char* iVec);

private:
	AES_KEY m_encKey;
	AES_KEY m_decKey;
	/* 秘钥 */
	string m_key;
};