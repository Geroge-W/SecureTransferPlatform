/*************************************************************************
	> File Name: protobuf_test.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: 2021年08月05日 星期四 22时21分54秒
 ************************************************************************/

#include <iostream>
#include <string>
#include "Person.pb.h"
using namespace std;

int main()
{
	/* 创建Person对象，并初始化 */
	Person p;
	p.set_id(1001);
	p.set_name("lbw");
	p.set_sex("male");
	p.set_age(17);
	p.set_color(Blue);
	
	/* 将Person对象序列化，得到序列化后的二进制字节流 */	
	string output;
	p.SerializeToString(&output);
	cout << "序列化后的数据：" << output << endl;

	/* 进行数据传输 */

	/* 接收数据，对其进行解码操作，得到原始数据 */
	Person pp;
	pp.ParseFromString(output);

	/* 处理原始数据，打印数据信息 */
	cout << "id = " << pp.id() << endl;
	cout << "name = " << pp.name() << endl;
	cout << "sex = " << pp.sex() << endl;
	cout << "age = " << pp.age() << endl;
	cout << "color = " << pp.color() << endl;

	return 0;
}
