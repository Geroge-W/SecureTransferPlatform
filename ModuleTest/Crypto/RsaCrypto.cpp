/*************************************************************************
	> File Name: RsaCrypto.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: Wed 11 Aug 2021 10:22:33 PM CST
 ************************************************************************/

#include <iostream>
#include <openssl/bio.h>
#include <openssl/err.h>
#include "RsaCrypto.h"
using namespace std;

RsaCrypto::RsaCrypto()
{
	m_publicKey = RSA_new();
	m_privateKey = RSA_new();
}

RsaCrypto::RsaCrypto(string fileName, bool isPrivate)
{
	m_publicKey = RSA_new();
	m_privateKey = RSA_new();
	if (isPrivate) {
		initPrivateKey(fileName);
	} else {
		initPublicKey(fileName);
	}
}

RsaCrypto::~RsaCrypto()
{
	RSA_free(m_publicKey);
	RSA_free(m_privateKey);
}

void RsaCrypto::generateKeyPair(int bits, string pub, string pri)
{
	RSA* r = RSA_new();
	/* 创建BIGNUM对象 */
	BIGNUM* e = BN_new();
	/* 初始化BIGNUM对象 */
	BN_set_word(e, 456789);
	/* 生成RSA密钥对 */
	RSA_generate_key_ex(r, bits, e, nullptr);
	/* 创建BIO文件对象 */
	BIO* pubBio = BIO_new_file(pub.c_str(), "w");
	/* 公钥以pem格式写入到文件中 */
	PEM_write_bio_RSAPublicKey(pubBio, r);
	/* 刷新缓冲区，保证写入到文件中 */
	BIO_flush(pubBio);
	BIO_free(pubBio);

	/* 创建BIO对象 */
	BIO* priBio = BIO_new_file(pri.c_str(), "w");
	/* 私钥以pem格式写入到文件中 */
	PEM_write_bio_RSAPrivateKey(priBio, r, nullptr, nullptr, 0, nullptr, nullptr);
	/* 刷新缓冲区，保证写入到文件中 */
	BIO_flush(priBio);
	BIO_free(priBio);

	/* 获取公钥和私钥 */
	m_privateKey = RSAPrivateKey_dup(r);
	m_publicKey = RSAPublicKey_dup(r);
	/* 释放资源 */
	BN_free(e);
	RSA_free(r);
}

string RsaCrypto::pubKeyEncrypt(string data)
{
	/* 计算公钥长度 */
	int keyLen = RSA_size(m_publicKey);
	cout << "public key length: " << keyLen << endl;
	/* 申请内存空间 */
	char* encode = new char[keyLen + 1];
	/* 使用公钥加密 */
	int ret = RSA_public_encrypt(data.size(), (unsigned char*)data.c_str(), 
		(unsigned char*)encode, m_publicKey, RSA_PKCS1_PADDING);
	string result = string();
	if (ret >= 0) {
		result = string(encode, ret);
	}
	/* 释放资源 */
	delete [] encode;
	return result;
}

string RsaCrypto::priKeyDecrypt(string encData)
{
	/* 计算私钥长度 */
	int keyLen = RSA_size(m_privateKey);
	cout << "private key length: " << keyLen << endl;
	/* 申请内存空间 */
	char* decode = new char[keyLen + 1];
	/* 使用私钥解密 */
	int ret = RSA_private_decrypt(encData.size(), (unsigned char*)encData.c_str(), 
	(unsigned char*)decode, m_privateKey, RSA_PKCS1_PADDING);
	string result = string();
	if (ret >= 0) {
		result = string(decode, ret);
	}
	/* 释放资源 */
	delete [] decode;
	return result;
}

string RsaCrypto::sign(string data, SignLevel level)
{
	unsigned int len;
	char* signBuf = new char[RSA_size(m_privateKey) + 1];
	RSA_sign(level, (unsigned char*)data.c_str(), data.size(), (unsigned char*)signBuf, 
		&len, m_privateKey);
	cout << "sign length: " << len << endl;
	string result = string(signBuf, len);
	delete [] signBuf;
	return result;
}

bool RsaCrypto::signVerify(string data, string signData, SignLevel level)
{
	int ret = RSA_verify(level, (unsigned char*)data.c_str(), data.size(), 
		(unsigned char*)signData.c_str(), signData.size(), m_publicKey);
	if (ret != 1) {
		return false;
	}
	return true;
}

bool RsaCrypto::initPublicKey(string pubFile)
{
	/* 通过BIO读取文件 */
	BIO* pubBio = BIO_new_file(pubFile.c_str(), "r");
	/* 将BIO中的pem数据读出 */
	if (!PEM_read_bio_RSAPublicKey(pubBio, &m_publicKey, nullptr, nullptr)) {
		ERR_print_errors_fp(stdout);
		return false;
	}
	BIO_free(pubBio);
	return true;
}

bool RsaCrypto::initPrivateKey(string priFile)
{
	/* 通过BIO读取文件 */
	BIO* priBio = BIO_new_file(priFile.c_str(), "r");
	/* 将BIO中的pem数据读出 */
	if (!PEM_read_bio_RSAPrivateKey(priBio, &m_privateKey, nullptr, nullptr)) {
		ERR_print_errors_fp(stdout);
		return false;
	}
	BIO_free(priBio);
	return true;
}