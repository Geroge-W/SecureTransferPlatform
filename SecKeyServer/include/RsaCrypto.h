/*************************************************************************
	> File Name: RsaCrypto.h
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: Wed 11 Aug 2021 09:55:55 PM CST
 ************************************************************************/

#pragma once
#include <iostream>
#include <openssl/rsa.h>
#include <openssl/pem.h>
using namespace std;

/* 数字签名强度的枚举类 */
enum SignLevel
{
	Level1 = NID_md5, 
	Level2 = NID_sha1, 
	Level3 = NID_sha224, 
	Level4 = NID_sha256, 
	Level5 = NID_sha384, 
	Level6 = NID_sha512
};

/* 使用RSA算法进行数据的加解密和数字签名 */
class RsaCrypto
{
public:
	RsaCrypto();
	/* 根据已有的私钥和公钥文件来初始化 */
	RsaCrypto(string fileName, bool isPrivate = true);
	~RsaCrypto();

	/* 将公钥/私钥字符串数据解析到RSA对象中 */
	void parseKeyString(string keyStr, bool isPrivate = true);
	/* 使用RSA算法生成秘钥对 */
	void generateKeyPair(int bits, string pub = "public.pem", string pri = "private.pem");
	/* 使用公钥加密 */
	string pubKeyEncrypt(string data);
	/* 使用私钥解密 */
	string priKeyDecrypt(string encData);
	/* 对数据进行数字签名 */
	string sign(string data, SignLevel level = Level3);
	/* 签名校验 */
	bool signVerify(string data, string signData, SignLevel level = Level3);

private:
	/* 获取公钥 */
	bool initPublicKey(string pubFile);
	/* 获取私钥 */
	bool initPrivateKey(string priFile);
	/* Base64编码 */
	string toBase64(const char* str, int len);
	/* Base64解码 */
	char* fromBase64(string str);

private:
	/* 公钥 */
	RSA* m_publicKey;
	/* 私钥 */
	RSA* m_privateKey;
};