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
#endif

	_buffer.resize(1024);
}

void Connection::initializeServerSocket(const int& family, const int& type, const int& flag)
{
	_clientSocket = socket(family, type, flag); 
#ifdef __linux__
	if (_clientSocket == -1)
		{
			std::cout << "Error initialization socket" << std::endl;
			close(_clientSocket);
			return;
		}
#elif _WIN32 || _WIN64
	if (_clientSocket == INVALID_SOCKET)
	{
		std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
		closesocket(_clientSocket);
		WSACleanup();
		return;
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
#ifdef __linux__

#elif _WIN32 || _WIN64
	ZeroMemory(&_serverSockaddr, sizeof(_serverSockaddr));
	_serverSockaddr.sin_family = family;
	_serverSockaddr.sin_addr = _IP;
	_serverSockaddr.sin_port = htons(port);
#else
#error Not implemented!
#endif
}

void Connection::establishConnectToServer()
{
	_checkStatus = connect(_clientSocket, (sockaddr*)&_serverSockaddr, sizeof(_serverSockaddr));
#ifdef __linux__
	if (_checkStatus != 0)
	{
		std::cout << "Connection to Server is FAILED" << std::endl;
		close(_clientSocket);
		return;
	}
#elif _WIN32 || _WIN64
	if (_checkStatus != 0)
	{
		std::cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << std::endl;
		closesocket(_clientSocket);
		WSACleanup();
		return;
	}
#else
#error Not implemented!
#endif
}

const char Connection::setCharValueAndSendItToServer()
{
	fgets(_buffer.data(), _buffer.size(), stdin);
	_packet_size = send(GetClientSocket(), _buffer.data(), _buffer.size(), 0);
	char value = _buffer[0];
	clearBuffer();

	return value;
}

void Connection::sendDataFromClientToServer()
{
	fgets(_buffer.data(), _buffer.size(), stdin);
	_packet_size = send(GetClientSocket(), _buffer.data(), _buffer.size(), 0);
	clearBuffer();
}

void Connection::receiveDataFromServer()
{
	while (_packet_size = recv(GetClientSocket(), _buffer.data(), _buffer.size(), MSG_WAITALL))
	{
		if (_buffer[0] == '\0')
		{
			break;
		}
		else
		{
			for (int i = 0; i < 1024; ++i)
			{
				if (_buffer[i] == '\0')
				{
					_container.push_back(_value);
					_value.clear();
					break;
				}
				else
				{
					_value += _buffer[i];
				}
			}
			clearBuffer();
		}
	}
}

bool Connection::receiveResultFromServer()
{
	_packet_size = recv(GetClientSocket(), _buffer.data(), _buffer.size(), MSG_WAITALL);
	if (_buffer[0] == '0')
	{
		clearBuffer();
		return true;
	}
	else
	{
		clearBuffer();
		return false;
	}
}

void Connection::clearBuffer()
{
	_buffer.clear();
	_buffer.resize(1024);
}

Connection::~Connection()
{
#ifdef __linux__
	close(_clientSocket);
#elif _WIN32 || _WIN64
	closesocket(_clientSocket);
	WSACleanup();
#else
#error Not implemented!
#endif
}