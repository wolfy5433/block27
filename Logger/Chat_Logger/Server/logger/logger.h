#pragma once

#include <iostream>
#include <fstream>
#include <ctime>
#include <shared_mutex>
#include <string>

# define NO_OF_CHARS 256

#include "../connection/connection.h" 

class Logger : public Connection
{
public:
	Logger();
	~Logger();

	std::string currentTime();
	void badCharHeuristic(std::string str, int size, int badchar[NO_OF_CHARS]);
	bool search(std::string txt, std::string pat);

	void setLogToFile(const std::string& value);
	void getLogsFromFile(const std::string& login);

protected:
	std::fstream log;
	std::shared_mutex shared_mutex;
	std::string str;
};