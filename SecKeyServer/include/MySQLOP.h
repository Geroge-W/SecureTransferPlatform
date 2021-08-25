/*************************************************************************
	> File Name: MySQLOP.h
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: Wed 18 Aug 2021 08:50:37 PM CST
 ************************************************************************/

#pragma once
#include <iostream>
#include <mysql/mysql.h>
#include "ShmNodeInfo.h"
using namespace std;

/* MySQL数据库操作的类 */
class MySQLOP
{
public:
	MySQLOP();
	~MySQLOP();

	/* 初始化环境连接数据库 */
	bool connectDB(string host, string user, string pwd, string DBName);
	/* 得到keyID -> 根据业务需求封装的小函数 */
	int getSecKeyID();
	/* 更新秘钥ID */
	bool updateSecKeyID(int secKeyID);
	/* 向数据库中写入秘钥的相关信息 */
	bool writeSecKey(ShmNodeInfo* pNode);
	/* 根据keyID查找密钥，将密钥标记为不可用 */
	bool cancelSecKey(string secKeyID);
	/* 查询最近N天的密钥信息 */
	bool getLastNDaysInfo(int n, string& output);
	/* 关闭与数据库的连接 */
	void closeDB();

private:
	/* 获取当前时间，并格式化为字符串 */
	string getCurTime();
	/* 执行SQL语句 */
	bool executeQuery(string sql);

private:
	MYSQL* m_connection;
	MYSQL_RES* m_result;
	MYSQL_ROW m_row;
};