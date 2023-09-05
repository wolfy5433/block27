#ifdef __linux__
#include <sys/socket.h>
#elif _WIN32 || _WIN64
#include <WinSock2.h>
#else
#error Not implemented!
#endif

#include "chat.h"

Chat::Chat()
{

}

Chat::~Chat()
{
	
}

void Chat::sendToClientAllHisMessages()
{
	int i = 0;
	std::cout << "Sending all messages of current users to client" << std::endl;
	_query = "SELECT user_name, login_of_sender, message FROM users JOIN messages ON users.login = messages.login_of_sender WHERE login_of_receiver = \'" + _login + "\'";
	sendDataToClient(_query);
	_query.clear();
}

void Chat::setNewPass()
{
	receiveStringValueFromClient(_password);
	while (checkUser(_login, _password) == false)
	{
		sendFailureToClient();
		_password.clear();
		receiveStringValueFromClient(_password);
	}
	sendSuccessToCleint();
	std::string newPassword;
	receiveStringValueFromClient(newPassword);
	while (newPassword == _password)
	{
		sendFailureToClient();
		newPassword.clear();
		receiveStringValueFromClient(newPassword);
	}
	sendSuccessToCleint();
	if (changePassword(_login, newPassword) == true)
	{
		sendSuccessToCleint();
		std::cout << "Password has been updated" << std::endl;
		setLogToFile("User " + _login + "(" + GetNameByLogin(_login) + ")" + " has changed his password successfully");
	}
	else
	{
		sendFailureToClient();
		std::cout << "Password has not been updated" << std::endl;
		setLogToFile("User " + _login + "(" + GetNameByLogin(_login) + ")" + " has not changed his password");
	}
}

void Chat::exitFromAccount()
{
	_login.clear();
}

void Chat::registration()
{
	std::cout << "Case 1 method registration has been opened" << std::endl;
	receiveStringValueFromClient(_userName);
	std::cout << _userName << std::endl;
	receiveStringValueFromClient(_login);
	std::cout << _login << std::endl;
	receiveStringValueFromClient(_password);
	std::cout << _password << std::endl;

	if (addUser(_userName, _login, _password) == true)
	{
		sendSuccessToCleint();
		std::cout << "User has been added to database successfully." << std::endl;
		setLogToFile("User " + _userName + " " + _login + " has been added");
	}
	else
	{
		sendFailureToClient();
		std::cout << "User has not been added to database" << std::endl;
		setLogToFile("User " + _userName + " " + _login + " has not been added");
	}
	_userName.clear();
	_login.clear();
	_password.clear();
}

bool Chat::enterToAccount()
{
	std::cout << "Case 2 method enterToAccount has been opened" << std::endl;
	receiveStringValueFromClient(_login);
	receiveStringValueFromClient(_password);

	if (checkUser(_login, _password) == true)
	{
		_password.clear();
		return true;
	}
	else
	{
		_login.clear();
		_password.clear();
		return false;
	}
}

void Chat::sendMessage()
{
	sendToClientAllUsersExceptCurrentUser();
	std::string receiverLogin;
	receiveStringValueFromClient(receiverLogin);
	if (checkLoginExist(receiverLogin) == true)
	{
		sendSuccessToCleint();
		receiveStringValueFromClient(_message);
		if (addMessage(_login, receiverLogin, _message) == true)
		{
			sendSuccessToCleint();
			std::cout << "Message was added to database successfully" << std::endl;
			setLogToFile("User " + GetNameByLogin(_login) + "(" + _login + ")" + " sent a message to " + GetNameByLogin(receiverLogin) + "(" + receiverLogin + ")");
		}
		else
		{
			sendFailureToClient();
			std::cout << "Message was not added to database" << std::endl;
			setLogToFile("User " + GetNameByLogin(_login) + "(" + _login + ")" + " did not send a message to " + GetNameByLogin(receiverLogin) + "(" + receiverLogin + ")");
		}
		_message.clear();
	}
	else
	{
		sendFailureToClient();
	}
}

void Chat::groupChat()
{
	std::cout << "Group chat still does not exist. Only sending messages between server and client is available." << std::endl;
	while (true)
	{
		std::string currentUser = GetNameByLogin(_login);;
		_packet_size = recv(GetClientSocket(), _buffer.data(), _buffer.size(), MSG_WAITALL);
		if (_buffer[0] == 'q')
		{
			break;
		}
		for (int i = 0; i < 1024; ++i)
		{
			if (_buffer[i] == '\n')
			{
				break;
			}
			else
			{
				_message += _buffer[i];
			}
		}
		std::cout << currentUser << ": " << _message << std::endl;
		_message.clear();
		fgets(_buffer.data(), _buffer.size(), stdin);
		_packet_size = send(GetClientSocket(), _buffer.data(), _buffer.size(), 0);
	}
	clearBuffer();
	_message.clear();
} 

std::string Chat::GetNameByLogin(const std::string& login)
{
	std::string name;
	_query = "SELECT user_name FROM users WHERE login = \'" + login + "\'";
	mysql_query(&_mysql, _query.c_str());
	_res = mysql_store_result(&_mysql);
	_row = mysql_fetch_row(_res);
	name = _row[0];
	mysql_free_result(_res);

	return name;
}

void Chat::sendToClientAllUsersExceptCurrentUser()
{ 
	
	std::cout << "Sending all users to client" << std::endl;
	_query = "SELECT user_name, login FROM users WHERE login NOT IN (\'" + _login + "\')";
	sendDataToClient(_query);
	_query.clear();
}

void Chat::sendToClientAllUsers()
{
	_query = "SELECT user_name, login FROM users";
	sendDataToClient(_query);
	_query.clear();
}

void Chat::sendLogsToClient()
{
	getLogsFromFile(_login);
}