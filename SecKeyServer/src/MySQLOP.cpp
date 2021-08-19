/*************************************************************************
	> File Name: MySQLOP.cpp
	> Author: lbw
	> Mail: 296273803@qq.com 
	> Created Time: Thu 19 Aug 2021 11:13:43 AM CST
 ************************************************************************/

#include <iostream>
#include <ctime>
#include "MySQLOP.h"
using namespace std;

MySQLOP::MySQLOP()
{
}

MySQLOP::~MySQLOP()
{
	closeDB();
}

bool MySQLOP::connectDB(string host, string user, string pwd, string DBName)
{
	/* 初始化数据库连接变量 */
	m_connection = mysql_init(nullptr);
	if (m_connection == nullptr) {
		cout << "Error: " << mysql_error(m_connection) << endl;
		return false;
	}

	/* 函数mysql_real_connect建立一个数据库连接 */
	m_connection = mysql_real_connect(m_connection, host.c_str(),
		user.c_str(), pwd.c_str(), DBName.c_str(), 0, nullptr, 0);
	/* 成功返回MYSQL*连接句柄，失败返nullptr */
	if (m_connection == nullptr) {
		cout << "Error: " << mysql_error(m_connection) << endl;
		return false;
	}
	cout << "MySQL database init success..." << endl;
	return true;
}

int MySQLOP::getSecKeyID()
{
	/* 该表只有一条数据 */
	string sql = "SELECT ikeysn FROM keysn;";
	bool ret = executeQuery(sql);
	if (ret == false) {
		return -1;
	}
	/* 获取第一行数据 */
	m_row = mysql_fetch_row(m_result);
	/* 释放结果集的内存 */
	mysql_free_result(m_result);
	cout << "MySQL is OK..." << endl;
	return atoi(m_row[0]);
}

bool MySQLOP::updateSecKeyID(int secKeyID)
{
	/* 更新数据库 */
	string sql = "UPDATE keysn SET ikeysn = " + to_string(secKeyID);
	bool ret = executeQuery(sql);
	if (ret == false) {
		return false;
	}
	/* 释放结果集的内存 */
	mysql_free_result(m_result);
	cout << "MySQL is OK..." << endl;
	return true;
}

bool MySQLOP::writeSecKey(ShmNodeInfo* pNode)
{
	/* 组织待插入的sql语句 */
	char sql[1024] = { 0 };
	sprintf(sql, "INSERT INTO seckeyinfo(clientid, serverid, keyid, createtime, state, seckey)\
	values ('%s', '%s', %d, str_to_date('%s', '%%Y-%%m-%%d %%H:%%i:%%s'), %d, '%s');",
	pNode->clientID, pNode->serverID, pNode->secKeyID, getCurTime().data(), 1, pNode->secKey);
	bool ret = executeQuery(string(sql));
	if (ret == false) {
		return false;
	}
	mysql_free_result(m_result);
	cout << "MySQL is OK." << endl;
	return true;
}

void MySQLOP::closeDB()
{
	if (m_connection != nullptr) {
		mysql_close(m_connection);
		m_connection = nullptr;
	}
}

string MySQLOP::getCurTime()
{
	time_t timep;
	time(&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));		
	return string(tmp);
}

bool MySQLOP::executeQuery(string sql)
{
	/* mysql_query()执行成功返回0，失败返回非0值 */
	int ret = mysql_query(m_connection, sql.c_str());
	if (ret != 0) {
		cout << "Error: " << mysql_error(m_connection) << endl;
		return false;
	}
	cout << "MySQL query: " << sql << endl;
	/* 获取结果集 */
	m_result = mysql_store_result(m_connection);
	return true;
}