#pragma once

#include <exception>
#include <string>
#include <string.h>
#include <algorithm>
#include <list>
#include <sstream>

#include "../logger/logger.h"		

class Chat : public Logger
{
public:
	Chat();
	~Chat();

	// Case '1'
	void registration();

	// Case '2'
	bool enterToAccount();
	std::string GetNameByLogin(const std::string& login); 

	// Case '2'::Case '1'::Case '1'
	void sendMessage();
	void sendToClientAllUsersExceptCurrentUser();

	// Case '2'::Case '1'::Case '2'
	void groupChat(); // в доработке

	// Case '2'::Case '2'
	void sendToClientAllHisMessages();

	// Case '2'::Case '3'
	void setNewPass();

	// Case '2'::Case '4'
	void exitFromAccount();

	// Case '2'::Case '0'
	void sendLogsToClient();

	// Case '3'
	void sendToClientAllUsers();

private:
	std::string _userName;
	std::string _login;
	std::string _password;
	std::string _message;
};