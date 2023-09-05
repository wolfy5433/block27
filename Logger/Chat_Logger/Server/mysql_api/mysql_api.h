#pragma once

#include <iostream>

#ifdef __linux__
#include <mysql/mysql.h>
#elif _WIN32 || _WIN64
#include "mysql.h"
#else
#error Not implemented!
#endif

class MySQL_API
{
public:
	MySQL_API();
	~MySQL_API();

	void initialize();
	void connectDatabase(const std::string& host, const std::string& userName, const std::string& userPass, const std::string& dbName);
	
	bool addUser(const std::string& userName, const std::string& login, const std::string& password);
	bool addMessage(const std::string& login, const std::string& receiverLogin, const std::string& message);
	bool checkUser(const std::string& login, const std::string& password);
	bool checkLoginExist(const std::string& login);
	bool changePassword(const std::string& login, const std::string& password);

protected:
	MYSQL _mysql;
	MYSQL_RES* _res;
	MYSQL_ROW _row;
	std::string _query;
};