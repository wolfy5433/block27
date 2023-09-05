#ifdef __linux__
#include <arpa/inet.h> // for inet_pton
#include <unistd.h> // for close socket
#include <sys/types.h>
#include <sys/socket.h> // for sockets and IPv4
#include <netinet/in.h> // for sockaddr_in
#elif _WIN32 || _WIN64
#include <WinSock2.h>
#include <WS2tcpip.h>
#else
#error Not implemented!
#endif

#include "connection.h"

Connection::Connection()
{
#ifdef __linux__

#elif _WIN32 || _WIN64
	_checkStatus = WSAStartup(MAKEWORD(2, 2), &_wsData);
	if (_checkStatus != 0)
	{
		std::cout << "Error WinSock version initializaion #" << WSAGetLastError() << std::endl;
		return;
	}
	else
	{
		std::cout << "WinSock successfully initialized" << std::endl;
	}
#endif
	_buffer.resize(1024);
}

void Connection::initializeServerSocket(const int& family, const int& type, const int& flag)
{
	_serverSocket = socket(family, type, flag); 
#ifdef __linux__
	if (_serverSocket == -1)
		{
			std::cout << "Error initialization socket" << std::endl;
			close(_serverSocket);
			return;
		}
		else
		{
			std::cout << "Server socket successfully initialized" << std::endl;
		}
#elif _WIN32 || _WIN64
	if (_serverSocket == INVALID_SOCKET)
		{
			std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
			closesocket(_serverSocket);
			WSACleanup();
			return;
		}
		else
		{
			std::cout << "Server socket successfully initialized" << std::endl;
		}
#else
#error Not implemented!
#endif
}

void Connection::initializeIP(const int& family, const std::string& IPv4)
{

	_checkStatus = inet_pton(family, IPv4.c_str(), &_IP);
	if (_checkStatus <= 0)
	{
		std::cout << "Error in IP translation to special numeric format" << std::endl;
		return;
	}
}

void Connection::establishServerInfo(const int& family, const int& port)
{
	_serverSockaddr.sin_family = family;
	_serverSockaddr.sin_addr = _IP;
	_serverSockaddr.sin_port = htons(port);
}

void Connection::bindServerSocket()
{
	_checkStatus = bind(_serverSocket, (sockaddr*)&_serverSockaddr, sizeof(_serverSockaddr)); // bind
	
#ifdef __linux__
	if (_checkStatus != 0)
		{
			std::cout << "Error Socket binding to server info" << std::endl;
			close(_serverSocket);
			return;
		}
		else
		{
			std::cout << "Binding socket successfully completed" << std::endl;
		}
#elif _WIN32 || _WIN64
	if (_checkStatus != 0)
		{
			std::cout << "Error Socket binding to server info. Error # " << WSAGetLastError() << std::endl;
			closesocket(_serverSocket);
			WSACleanup();
			return;
		}
		else
		{
			std::cout << "Binding socket successfully completed" << std::endl;
		}
#else
#error Not implemented!
#endif
}

void Connection::serverIsListeningToConnections()
{
	_checkStatus = listen(_serverSocket, SOMAXCONN); 
#ifdef __linux__
	if (_checkStatus == -1) 
	{
		std::cout << "Can't start to listen to" << std::endl;
		close(_serverSocket);
		return;
	}
	else
	{
		std::cout << "Listening..." << std::endl;
	}
#elif _WIN32 || _WIN64
	if (_checkStatus != 0) 
	{
		std::cout << "Can't start to listen to. Error # " << WSAGetLastError() << std::endl;
		closesocket(_serverSocket);
		WSACleanup();
		return;
	}
	else
	{
		std::cout << "Listening..." << std::endl;
	}
#else
#error Not implemented!
#endif
}

void Connection::createClientSocketAndEstablishHisInfo()
{
#ifdef __linux__
	socklen_t client_size = sizeof(_clientSockaddr);
	_clientSocket = accept(_serverSocket, (sockaddr*)&_clientSockaddr, &client_size); 
	if (_clientSocket == -1)
	{
		std::cout << "Client detected, but can't connect to a client." << std::endl;
		close(_serverSocket);
		close(_clientSocket);
		return;
	}
	else
	{
		std::cout << "Connection to a client established successfully" << std::endl;
	}
#elif _WIN32 || _WIN64
	ZeroMemory(&_clientSockaddr, sizeof(_clientSockaddr));
	int client_size = sizeof(_clientSockaddr);
	_clientSocket = accept(_serverSocket, (sockaddr*)&_clientSockaddr, &client_size); 
	if (_clientSocket == INVALID_SOCKET)
	{
		std::cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << std::endl;
		closesocket(_serverSocket);
		closesocket(_clientSocket);
		WSACleanup();
		return;
	}
	else
	{
		std::cout << "Connection to a client established successfully" << std::endl;
	}
#else
#error Not implemented!
#endif
}

void Connection::sendSuccessToCleint()
{
	_buffer[0] = '0';
	_packet_size = send(GetClientSocket(), _buffer.data(), _buffer.size(), 0);
	clearBuffer();
}

void Connection::sendFailureToClient()
{
	_buffer[0] = '1';
	_packet_size = send(GetClientSocket(), _buffer.data(), _buffer.size(), 0);
	clearBuffer();
}

void Connection::sendDataToClient(const std::string& query)
{
	int i = 0;
	mysql_query(&_mysql, query.c_str());
	if (_res = mysql_store_result(&_mysql))
	{
		while (_row = mysql_fetch_row(_res))
		{
			for (int j = 0; j < mysql_num_fields(_res); ++j)
			{
				_value = _row[j];
				for (int k = 0; k < _value.size(); ++k)
				{
					_buffer[i] = _value[k];
					++i;
				}
				sendBufferToClient();
				_value.clear();
				i = 0;
			}
		}
		sendBufferToClient();
	}
	else
	{
		sendBufferToClient();
		std::cout << "Error " << mysql_error(&_mysql) << std::endl;
	}
	mysql_free_result(_res);
}

void Connection::sendBufferToClient()
{
	_packet_size = send(GetClientSocket(), _buffer.data(), _buffer.size(), 0);
	clearBuffer();
}

void Connection::clearBuffer()
{
	_buffer.clear();
	_buffer.resize(1024);
}

const char Connection::GetCharValueFromClient()
{
	_packet_size = recv(GetClientSocket(), _buffer.data(), _buffer.size(), MSG_WAITALL);
	char value = _buffer[0];
	clearBuffer();

	return value;
}

void Connection::receiveStringValueFromClient(std::string& value)
{
	_packet_size = recv(GetClientSocket(), _buffer.data(), _buffer.size(), MSG_WAITALL);
	for (int i = 0; i < 1024; ++i)
	{
		if (_buffer[i] == '\n')
		{
			break;
		}
		else
		{
			value += _buffer[i];
		}
	}
	clearBuffer();
}

Connection::~Connection()
{
#ifdef __linux__
	close(_serverSocket);
	close(_clientSocket);
#elif _WIN32 || _WIN64
	closesocket(_serverSocket);
	closesocket(_clientSocket);
	WSACleanup();
#else
#error Not implemented!
#endif
}