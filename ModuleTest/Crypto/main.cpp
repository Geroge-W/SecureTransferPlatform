/*************************************************************************
	> File Name: main.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: Wed 11 Aug 2021 09:45:05 PM CST
 ************************************************************************/

#include <iostream>
#include <functional>
#include "Hash.h"
#include "RsaCrypto.h"
#include "AesCrypto.h"
using namespace std;

static void hashTest()
{
	function<void(HashType, string)> test = [](HashType t, string s)
	{
		Hash hash(t);
		hash.addData("lbw is the best!");
		hash.addData("nice to meet you!");
		string ret = hash.getResult();
		cout << s << ret << endl;
	};
	cout << "hash function test: " << endl;
	test(T_MD5, "MD5: ");
	test(T_SHA1, "SHA1: ");
	test(T_SHA224, "SHA224: ");
	test(T_SHA256, "SHA256: ");
	test(T_SHA384, "SHA384: ");
	test(T_SHA512, "sha512: ");
	cout << endl;
}

static void rsaTest()
{
	cout << "RSA encrypto test: " << endl;
	RsaCrypto rc;
	rc.generateKeyPair(2048);
	string oriData = "good good study, day day up!";
	cout << "orignal data: " << oriData << endl;
	string encStr = rc.pubKeyEncrypt(oriData);
	cout << "encoded data: " << encStr << endl;
	string decStr = rc.priKeyDecrypt(encStr);
	cout << "decoded data: " << decStr << endl;

	string sign = rc.sign(oriData);
	bool bl = rc.signVerify(oriData, sign);
	cout << "verify: " << bl << endl;
	cout << endl;
}

static void aesTest()
{
	cout << "AES test: " << endl;
	string key = "1234567887654321";
	cout << "key length: " << key.length() << endl;
	cout << "please input the data to encode: ";
	string oriStr;
	getline(cin, oriStr);
	try {
		AesCrypto aes(key);
		cout << "original data: " << oriStr << endl;
		cout << "original data length: " << oriStr.length() << endl;
		string res = aes.cbcEncrypt(oriStr);
		cout << "encoded data length: " << res.length() << endl;
		res = aes.cbcDecrypt(res);
		cout << "decoded data: " << res << endl;
		cout << "decoded data length: " << res.length() << endl;
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << '\n';
	}
	
}

int main()
{
	// hashTest();
	// rsaTest();
	aesTest();

	return 0;
}
