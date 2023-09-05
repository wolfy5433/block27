#include "mysql_api.h"

MySQL_API::MySQL_API()
{

}

MySQL_API::~MySQL_API()
{
	mysql_close(&_mysql);
}

void MySQL_API::initialize()
{
	mysql_init(&_mysql);
	if (&_mysql == NULL)
	{
		std::cout << "Error: can't creat MySQL - descriptor" << std::endl;
		return;
	}
	else
	{
		std::cout << "MySQL descriptor successfully initialized" << std::endl;
	}

	mysql_set_character_set(&_mysql, "utf8mb4"); 
	std::cout << "Connection characterest: " << mysql_character_set_name(&_mysql) << std::endl;
}

void MySQL_API::connectDatabase(const std::string& host, const std::string& userName, const std::string& userPass, const std::string& dbName)
{
	if (mysql_real_connect(&_mysql, host.c_str(), userName.c_str(), userPass.c_str(), NULL, 0, NULL, 0))
	{
		_query = "SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = \'" + dbName + "\'";
		mysql_query(&_mysql, _query.c_str());
		_query.clear();
		_res = mysql_store_result(&_mysql);
		if (!(_row = mysql_fetch_row(_res)))
		{
			_query = "CREATE DATABASE IF NOT EXISTS " + dbName;
			mysql_query(&_mysql, _query.c_str());
			_query.clear();
			std::cout << "Database was created" << std::endl;

			_query = "USE " + dbName;
			mysql_query(&_mysql, _query.c_str());
			_query.clear();

			_query = "CREATE TABLE users (login VARCHAR(20) PRIMARY KEY CHECK (login <> ''), pass VARCHAR(40), user_name VARCHAR(30) CHECK (user_name <> ''))";
			mysql_query(&_mysql, _query.c_str());
			_query.clear();
			std::cout << "Table \'users\' was created" << std::endl;

			_query = "CREATE TABLE messages (id INT PRIMARY KEY AUTO_INCREMENT, login_of_sender VARCHAR(20), login_of_receiver VARCHAR(20), message TEXT(200) CHECK (message <> ''), FOREIGN KEY(login_of_sender) REFERENCES users(login), FOREIGN KEY(login_of_receiver) REFERENCES users(login))";
			mysql_query(&_mysql, _query.c_str());
			_query.clear();
			std::cout << "Table \'messages\' was created" << std::endl;
		}
		else
		{
			_query = "USE " + dbName;
			mysql_query(&_mysql, _query.c_str());
			_query.clear();
			std::cout << "Server successfully connected to database" << std::endl;
		}
		mysql_free_result(_res);
	}
	else
	{
		std::cout << "Error occured" << std::endl;
		return;
	}
}

bool MySQL_API::addUser(const std::string& userName, const std::string& login, const std::string& password)
{
	_query = "INSERT INTO users (login, pass, user_name) VALUES (\'" + login + "\', \'" + password + "\', \'" + userName + "\')";
	if (mysql_query(&_mysql, _query.c_str()) == 0)
	{
		_query.clear();
		return true;
	}
	else
	{
		_query.clear();
		return false;
	}
}

bool MySQL_API::addMessage(const std::string& login, const std::string& receiverLogin, const std::string& message)
{
	_query = "INSERT INTO messages (login_of_sender, login_of_receiver, message) VALUES (\'" + login + "\', \'" + receiverLogin + "\', \'" + message + "\')";
	if (mysql_query(&_mysql, _query.c_str()) == 0)
	{
		_query.clear();
		return true;
	}
	else
	{
		_query.clear();
		return false;
	}
}

bool MySQL_API::checkUser(const std::string& login, const std::string& password)
{
	_query = "SELECT login, pass FROM users WHERE login = \'" + login + "\' and pass = \'" + password + "\'";
	mysql_query(&_mysql, _query.c_str());
	_res = mysql_store_result(&_mysql);
	if (_row = mysql_fetch_row(_res))
	{
		_query.clear();
		mysql_free_result(_res);
		return true;
	}
	else
	{
		_query.clear();
		mysql_free_result(_res);
		return false;
	}
}

bool MySQL_API::checkLoginExist(const std::string& login)
{
	_query = "SELECT login FROM users WHERE login = \'" + login + "\'";
	mysql_query(&_mysql, _query.c_str());
	_res = mysql_store_result(&_mysql);
	if (_row = mysql_fetch_row(_res))
	{
		_query.clear();
		mysql_free_result(_res);
		return true;
	}
	else
	{
		_query.clear();
		mysql_free_result(_res);
		return false;
	}
}

bool MySQL_API::changePassword(const std::string& login, const std::string& password)
{
	_query = "UPDATE users SET pass = \'" + password + "\' where login = \'" + login + "\'";
	if (mysql_query(&_mysql, _query.c_str()) == 0)
	{
		_query.clear();
		return true;
	}
	else
	{
		_query.clear();
		return false;
	}
}