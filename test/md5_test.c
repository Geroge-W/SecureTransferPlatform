/*************************************************************************
	> File Name: openssl_test.c
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月05日 星期四 15时38分07秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/md5.h>

void getMD5(const char* str, char* result)
{
	MD5_CTX ctx;
	// 初始化
	MD5_Init(&ctx);
	// 添加数据
	MD5_Update(&ctx, str, strlen(str));
	// 计算结果
	unsigned char md[16] = { 0 };
	MD5_Final(md, &ctx);
	for (int i = 0; i < 16; ++i) {
		sprintf(&result[i * 2], "%02x", md[i]);
	}
}

int main(void)
{
	char result[33] = { 0 };
	getMD5("hello world", result);
	printf("md5 value: %s\n", result);
	return 0;
}
