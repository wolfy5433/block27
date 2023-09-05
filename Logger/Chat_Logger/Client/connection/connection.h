#pragma once

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

#include <iostream>
#include <vector>

class Connection
{
public:
	Connection();
	~Connection();

	void initializeServerSocket(const int& family, const int& type, const int& flag);
	void initializeIP(const int& family, const std::string& IPv4);
	void establishServerInfo(const int& family, const int& port);
	void establishConnectToServer();

	const char setCharValueAndSendItToServer();

	void sendDataFromClientToServer();
	void receiveDataFromServer(); // удаление
	bool receiveResultFromServer();

	void clearBuffer();

#ifdef __linux__
	int GetClientSocket() { return _clientSocket; };
#elif _WIN32 || _WIN64
	SOCKET GetClientSocket() { return _clientSocket; };
#else
#error Not implemented!
#endif

protected:
#ifdef __linux__
	int _serverSocket;
	int _clientSocket;
	in_addr _IP;
	struct sockaddr_in _serverSockaddr;
	struct sockaddr_in _clientSockaddr;
#elif _WIN32 || _WIN64
	WSAData _wsData;
	SOCKET _clientSocket;
	in_addr _IP;
	sockaddr_in _serverSockaddr;
	sockaddr_in _clientSockaddr;
#else
#error Not implemented!
#endif
	std::vector<char> _buffer;
	std::vector<std::string> _container;
	std::string _value;
	int _checkStatus;
	short _packet_size = 0;
};