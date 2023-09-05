#ifdef __linux
#include <sys/socket.h>
#elif _WIN32 || _WIN64
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <Windows.h>
#else
#error Not implemented!
#endif

#include <iostream>
#include <string>
#include <vector>

#include "../Client/chat/chat.h"

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
	SetConsoleCP(1251);		// для кирилицы
	SetConsoleOutputCP(1251);
#else
#error Not implemented!
#endif
	Chat chat;

	chat.Chat::Connection::initializeServerSocket(con_family, con_type, con_flag);
	chat.Chat::Connection::initializeIP(con_family, con_IP);
	chat.Chat::Connection::establishServerInfo(con_family, con_port);
	chat.Chat::Connection::establishConnectToServer();
	
	std::cout << "Добро пожаловать!" << std::endl;

	bool open = true;
	while (open)
	{
		std::cout << "Выберите, что вы хотите сделать:" << std::endl;
		std::cout << "1.[Создать аккаунт] 2.[Войти в аккаунт] 3.[Отобразить всех пользователей] 4.[Выйти из чата]" << std::endl;

		char choice = chat.Chat::Connection::setCharValueAndSendItToServer();

		switch (choice)
		{
		case '1':
		{
			chat.registration();

			break;
		}
		case '2':
		{
			chat.enterToAccount();
			if (chat.Chat::Connection::receiveResultFromServer() == true)
			{
				bool open2 = true;
				while (open2)
				{
					std::cout << "Выберите действие:" << std::endl << "1.[Написать сообщение] 2.[Показать сообщение] 3.[Сменить пароль] 4.[Выйти из аккаунта] 0.[Читать логи]" << std::endl;
					char choiceInsideCase2 = chat.Chat::Connection::setCharValueAndSendItToServer();
					switch (choiceInsideCase2)
					{
					case '1':
					{
						bool open3 = true;
						while (open3)
						{
							std::cout << "Кому вы хотите написать сообщение:\n1.[Определенному пользователю] 2.[Групповое сообщение] 3.[Вернутся назад]" << std::endl;
							char choiceInsideCase2InsideCase1 = chat.Chat::Connection::setCharValueAndSendItToServer();
							switch (choiceInsideCase2InsideCase1)
							{
							case '1': // сообщение пользователю
							{
								chat.sendMessage();

								break;
							}
							case '2': // групповой чат
							{
								chat.groupChat();

								break;
							}
							case '3':
								open3 = false;

								break;
							default:
							{
								std::cout << "Неверное значение\n" << std::endl;

								break;
							}
							}
						};

						break;
					}
					case '2': // показать сообщения
					{
						chat.printUsersMessages();
						std::cout << "Нажмите любую клавишу, чтобы продолжить..." << std::endl;
						system("pause>0"); // пауза, чтобы пользователь прочитал сообщение

						break;
					}
					case '3': //Смена пароля на новый
					{
						chat.setNewPass();

						break;
					}
					case '4':
						open2 = false;
						std::cout << "Вы вышли из аккаунта." << std::endl;

						break;
					case '0':
						chat.readLogs();

						break;
					default:
					{
						std::cout << "Неверное значение\n" << std::endl;

						break;
					}
					}
				};
			}
			else
			{
				std::cout << "Неверный логин или пароль\n" << std::endl;
			}
			break;
		}
		case '3':
		{
			chat.printUsersFromServer();

			break;
		}
		case '4':
			open = false;

			break;
		default:
		{
			std::cout << "Неверное значение\n" << std::endl;

			break;
		};
		}
	}

	return 0;
}