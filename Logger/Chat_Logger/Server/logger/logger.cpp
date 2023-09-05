#include "logger.h"

Logger::Logger()
{
	log.open("log.txt", std::fstream::in | std::fstream::app);
	if (log.is_open())
	{
		std::cout << "Log file is opened" << std::endl;
	}
	else
	{
		std::cout << "Server can not open log file" << std::endl;
	}
}

std::string Logger::currentTime()
{
	std::time_t currentTime = std::time(nullptr);
	std::tm localTime;
	
#ifdef __linux__
	localtime_r(&currentTime, &localTime);
#elif _WIN32 || _WIN64
	localtime_s(&localTime, &currentTime);
#else
#error Not implemented!
#endif

	char timeString[20];
	std::strftime(timeString, sizeof(timeString), "%Y/%m/%d %H:%M:%S", &localTime);

	return timeString;
}

void Logger::badCharHeuristic(std::string str, int size, int badchar[NO_OF_CHARS])
{
    int i;
	for (i = 0; i < NO_OF_CHARS; i++)
	{
		badchar[i] = -1;
	}
	for (i = 0; i < size; i++)
	{
		badchar[(int)str[i]] = i;
	}    
}


bool Logger::search(std::string txt, std::string pat)
{
    int m = pat.size();
    int n = txt.size();

    int badchar[NO_OF_CHARS];

    badCharHeuristic(pat, m, badchar);

    int s = 0; 
	while (s <= (n - m))
	{
		int j = m - 1;

		while (j >= 0 && pat[j] == txt[s + j])
		{
			j--;
		}
		if (j < 0)
		{
			s += (s + m < n) ? m - badchar[txt[s + m]] : 1;

			return true;
		}
		else
		{
			s += max(1, j - badchar[txt[s + j]]);
		}
	}

	return false;
}

void Logger::setLogToFile(const std::string& value)
{
	shared_mutex.lock();
	log << currentTime() + " " + value << std::endl;
	shared_mutex.unlock();
}

void Logger::getLogsFromFile(const std::string& login)
{
	while (!log.eof())
	{
		shared_mutex.lock_shared();
		std::getline(log, str);
		shared_mutex.unlock_shared();
		if (search(str, login) == true)
		{
			for (int i = 0; i < str.size(); ++i)
			{
				_buffer[i] = str[i];
			}
			sendBufferToClient();
			str.clear();
		}
		else
		{
			str.clear();
		}
	}
	sendBufferToClient();
}

Logger::~Logger()
{
	log.close();
}