/*************************************************************************
	> File Name: Hash.h
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: Wed 11 Aug 2021 07:42:54 PM CST
 ************************************************************************/

#pragma once
#include <iostream>
#include <cstring>
#include <openssl/md5.h>
#include <openssl/sha.h>
using namespace std;

enum HashType
{
	T_MD5 = 1, 
	T_SHA1, 
	T_SHA224, 
	T_SHA256, 
	T_SHA384, 
	T_SHA512
};

class Hash
{
public:
	/* 根据传入的的type构造Hash对象 */
	Hash(HashType type);
	/* 析构函数 */
	~Hash();
	/* 传入需要哈希的数据 */
	void addData(string data);
	/* 计算哈希结果 */
	string getResult();

private:
	/* MD5 */
	inline void md5Init() {
		MD5_Init(&m_md5);
	}
	inline void md5AddData(const char* data) {
		MD5_Update(&m_md5, data, strlen(data));
	}
	string md5Result();

	/* SHA1*/
	inline void sha1Init() {
		SHA1_Init(&m_sha1);
	}
	inline void sha1AddData(const char* data) {
		SHA1_Update(&m_sha1, data, strlen(data));
	}
	string sha1Result();

	/* SHA224 */ 
	inline void sha224Init() {
		SHA224_Init(&m_sha224);
	}
	inline void sha224AddData(const char* data) {
		SHA224_Update(&m_sha224, data, strlen(data));
	}
	string sha224Result();

	/* SHA256 */
	inline void sha256Init() {
		SHA256_Init(&m_sha256);
	}
	inline void sha256AddData(const char* data) {
		SHA256_Update(&m_sha256, data, strlen(data));
	}
	string sha256Result();

	/* SHA384 */
	inline void sha384Init() {
		SHA384_Init(&m_sha384);
	}
	inline void sha384AddData(const char* data) {
		SHA384_Update(&m_sha384, data, strlen(data));
	}
	string sha384Result();

	/* SHA512 */
	inline void sha512Init() {
		SHA512_Init(&m_sha512);
	}
	inline void sha512AddData(const char* data) {
		SHA512_Update(&m_sha512, data, strlen(data));
	}
	string sha512Result();

private:
	HashType m_type;
	MD5_CTX m_md5;
	SHA_CTX m_sha1;
	SHA256_CTX m_sha224;
	SHA256_CTX m_sha256;
	SHA512_CTX m_sha384;
	SHA512_CTX m_sha512;
};