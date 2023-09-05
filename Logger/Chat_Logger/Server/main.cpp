#ifdef __linux
#include <sys/socket.h>
#elif _WIN32 || _WIN64
#pragma comment(lib, "libmysql.lib")
#pragma comment(lib, "ws2_32.lib")
#else
#error Not implemented!
#endif

#include "../Server/chat/chat.h"

//	Параметры базы данных
constexpr const char* db_host = "127.0.0.1";
constexpr const char* db_user = "root";
constexpr const char* db_pass = "pass";
constexpr const char* db_name = "chat";

//	Параметры соединения
constexpr const int con_port = 55555;
constexpr const int con_family = AF_INET;
constexpr const int con_type = SOCK_STREAM;
constexpr const int con_flag = 0;
constexpr const char* con_IP = "127.0.0.1";

int main()
{
#ifdef __linux__
	setlocale(LC_ALL, "ru-RU");
#elif defined(_WIN32) || defined(_WIN64)
	SetConsoleCP(1251);		// Ability to use cyrillic
	SetConsoleOutputCP(1251);
#else
#error Not implemented!
#endif
	
	Chat chat;

	chat.Chat::Logger::Connection::MySQL_API::initialize();
	chat.Chat::Logger::Connection::MySQL_API::connectDatabase(db_host, db_user, db_pass, db_name);

	chat.Chat::Logger::Connection::initializeServerSocket(con_family, con_type, con_flag);
	chat.Chat::Logger::Connection::initializeIP(con_family, con_IP);
	chat.Chat::Logger::Connection::establishServerInfo(con_family, con_port);
	chat.Chat::Logger::Connection::bindServerSocket();
	chat.Chat::Logger::Connection::serverIsListeningToConnections();
	chat.Chat::Logger::Connection::createClientSocketAndEstablishHisInfo(); 

	bool open = true;
	while (open)
	{
		char choice = chat.Chat::Logger::Connection::GetCharValueFromClient();
		switch (choice)
		{
		case '1':
		{
			chat.registration();

			break;
		}
		case '2':
		{
			if (chat.enterToAccount() == true)
			{
				chat.Chat::Logger::Connection::sendSuccessToCleint();
		
				bool open2 = true;
				while (open2)
				{
					char choiceInsideCase2 = chat.Chat::Logger::Connection::GetCharValueFromClient();
					switch (choiceInsideCase2)
					{
					case '1':
					{
						bool open3 = true;
						while (open3)
						{
							char choiceInsideCase2InsideCase1 = chat.Chat::Logger::Connection::GetCharValueFromClient();
							switch (choiceInsideCase2InsideCase1)
							{
							case '1':	// сообщение пользователю
							{
								chat.sendMessage();

								break;
							}
							case '2':	// групповой чат
							{
								
								chat.groupChat();

								break;
							}
							case '3':
								open3 = false;

								break;
							default:
							{
								break;
							}
							}
						};

						break;
					}
					case '2':	// просмотр сообщений
					{
						chat.sendToClientAllHisMessages();

						break;
					}
					case '3':	// новый пароль
					{
						chat.setNewPass();

						break;
					}
					case '4':
						open2 = false;
						chat.exitFromAccount();

						break;
					case '0':
						chat.sendLogsToClient();

						break;
					default:
					{
						break;
					}
					}
				};
			}
			else
			{
				chat.Chat::Logger::Connection::sendFailureToClient();
			}
			break;
		}
		case '3':
		{
			chat.sendToClientAllUsers();

			break;
		}
		case '4':
			open = false;

			break;
		default:
		{
			break;
		};
		}
	}

	return 0;
}