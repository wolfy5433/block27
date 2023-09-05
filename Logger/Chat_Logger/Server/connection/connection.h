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

#include "../mysql_api/mysql_api.h"

class Connection : public MySQL_API
{
public:
	Connection();
	~Connection();

	void initializeServerSocket(const int& family, const int& type, const int& flag);
	void initializeIP(const int& family, const std::string&IPv4);
	void establishServerInfo(const int& family, const int& port);
	void bindServerSocket();
	void serverIsListeningToConnections();
	void createClientSocketAndEstablishHisInfo();

	void sendSuccessToCleint();
	void sendFailureToClient();
	void sendDataToClient(const std::string& query);
	void sendBufferToClient();
	void clearBuffer();
	const char GetCharValueFromClient();
	void receiveStringValueFromClient(std::string& value);

#ifdef __linux__
	int GetServerSocket() { return _serverSocket; };
	int GetClientSocket() { return _clientSocket; };
#elif _WIN32 || _WIN64
	SOCKET GetServerSocket() { return _serverSocket; };
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
	SOCKET _serverSocket;
	SOCKET _clientSocket;
	in_addr _IP;
	sockaddr_in _serverSockaddr;
	sockaddr_in _clientSockaddr;
#else
#error Not implemented!
#endif
	std::vector <char> _buffer;
	std::string _value;
	int _checkStatus;
	short _packet_size = 0;
};