/*************************************************************************
	> File Name: test.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月07日 星期六 16时14分45秒
 ************************************************************************/

#include <iostream>
#include "Codec.h"
#include "RequestCodec.h"
#include "RespondCodec.h"
#include "CodecFactory.h"
#include "RequestFactory.h"
#include "RespondFactory.h"
using namespace std;

int main(void)
{
	/* 数据编码 */
	RequestInfo info { 1, "client", "server", "x00911", "hello world" };
	CodecFactory* factory = new RequestFactory(&info);
	Codec* codec = factory->createCodec();
	string str = codec->encodeMsg();
	cout << "序列化数据为：" << str << endl;
	delete factory;
	delete codec;

	/* 数据解码 */
	factory = new RequestFactory(str);
	codec = factory->createCodec();
	RequestMsg* r1 = (RequestMsg*)codec->decodeMsg();
	cout << "cmdType: " << r1->cmdtype() << endl;
	cout << "clientID: " << r1->clientid()<< endl;
	cout << "serverID: " << r1->serverid() << endl;
	cout << "sign: " << r1->sign() << endl;
	cout << "data: " << r1->data() << endl;
	delete factory;
	delete codec;

	cout << "---------------------------------" << endl;
	/* 数据编码 */
	RespondInfo resInfo { 1, 999, "luffy", "zoro", "Lakers go!" };
	factory = new RespondFactory(&resInfo);
	codec = factory->createCodec();
	str = codec->encodeMsg();
	cout << "序列化数据为：" << str << endl;
	delete factory;
	delete codec;

	/* 数据解码 */
	factory = new RespondFactory(str);
	codec = factory->createCodec();
	RespondMsg* r2 = (RespondMsg*)codec->decodeMsg();
	cout << "status: " << r2->status() << endl;
	cout << "secKeyID: " << r2->seckeyid()<< endl;
	cout << "clientID: " << r2->clientid() << endl;
	cout << "serverID: " << r2->serverid() << endl;
	cout << "data: " << r2->data() << endl;
	delete factory;
	delete codec;

	exit(0);
}
