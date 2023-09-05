#include "chat.h"

#ifdef __linux__
#include <sys/socket.h>
#elif _WIN32 || _WIN64
#include <WinSock2.h>
#else
#error Not implemented!
#endif


Chat::Chat()
{
	
}

Chat::~Chat()
{

}

void Chat::registration()
{
	std::cout << "Enter name: ";
	sendDataFromClientToServer();

	std::cout << "Enter login: ";
	sendDataFromClientToServer();

	std::cout << "Enter password: ";
	sendDataFromClientToServer();

	if (receiveResultFromServer() == true)
	{
		std::cout << "Account has been created successfully\n" << std::endl;
	}
	else
	{
		std::cout << "Something went wrong! Account has not been registered.\n" << std::endl;
	}
}

void Chat::enterToAccount()
{
	std::cout << "Enter login: ";
	sendDataFromClientToServer();

	std::cout << "Enter password: ";
	sendDataFromClientToServer();
}

void Chat::sendMessage()
{
	std::cout << "Users:" << std::endl;
	printUsersFromServer();
	
	std::cout << "Enter user's login to whom you want to send a message: ";
	sendDataFromClientToServer();

	if (receiveResultFromServer() == true)
	{
		std::cout << "Write a message: ";
		sendDataFromClientToServer();
		if (receiveResultFromServer() == true)
		{
			std::cout << "Message has been sent successfully!" << std::endl;
		}
		else
		{
			std::cout << "Something went wrong! Message has not been sent." << std::endl;
		}
	}
	else
	{
		std::cout << "Wrong login" << std::endl;
	}
}

void Chat::printUsersFromServer()
{
	receiveDataFromServer();
	int i = 0;
	for (auto s : _container)
	{
		if (i % 2 == 0)
		{
			std::cout << "----\n" << "- User's name: " << s << '\n';
			++i;
		}
		else
		{
			std::cout << "- Login: " << s << std::endl;
			++i;
		}
	}
	_container.clear();
	std::cout << std::endl;
}

void Chat::groupChat()
{
	std::cout << "Group chat still does not created. Messages can only be sent between server and client. For exit enter \'q\'" << std::endl;
	std::string message;
	while (true)
	{
		fgets(_buffer.data(), _buffer.size(), stdin);
		_packet_size = send(GetClientSocket(), _buffer.data(), _buffer.size(), 0);
		if (_buffer[0] == 'q')
		{
			break;
		}
		_packet_size = recv(GetClientSocket(), _buffer.data(), _buffer.size(), MSG_WAITALL);
		for (int i = 0; i < 1024; ++i)
		{
			if (_buffer[i] == '\n')
			{
				break;
			}
			else
			{
				message += _buffer[i];
			}
		}
		std::cout << "Server: " << message << std::endl;
		message.clear();
	}
	message.clear();
	clearBuffer();
}

void Chat::printUsersMessages()
{
	receiveDataFromServer();
	int i = 1;
	for (auto& s : _container)
	{
		if (i == 1)
		{
			std::cout << "--- Message from " << s;
			++i;
		}
		else if (i == 2)
		{
			std::cout << "(" << s << "): ";
			++i;
		}
		else
		{
			std::cout << s << std::endl;
			i = 1;
		}
	}
	_container.clear();
	std::cout << std::endl;
}

void Chat::setNewPass()
{
	std::cout << "Enter current password: ";
	sendDataFromClientToServer();
	while (receiveResultFromServer() == false)
	{
		std::cout << "Wrong password\nTry again: ";
		sendDataFromClientToServer();
	}

	std::cout << "Enter new password: ";
	sendDataFromClientToServer();
	while (receiveResultFromServer() == false)
	{
		std::cout << "This is your current password! Enter new : ";
		sendDataFromClientToServer();
	}

	if (receiveResultFromServer() == true)
	{
		std::cout << "Password has been changed successfully!\n" << std::endl;
	}
	else
	{
		std::cout << "Something went wrong! Password has not been changed." << std::endl;
	}
}

void Chat::readLogs()
{
	receiveDataFromServer();
	for (auto& s : _container)
	{
		std::cout << s << std::endl;
	}
	_container.clear();
	std::cout << std::endl;
}