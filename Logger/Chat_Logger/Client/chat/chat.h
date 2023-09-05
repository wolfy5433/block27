#pragma once

#include <iostream>

#include "../connection/connection.h"

class Chat : public Connection
{
public:
	Chat();
	~Chat();

	// Case '1'
	void registration();

	// Case '2'
	void enterToAccount();

	// Case '2'::Case '1'::Case '1'
	void sendMessage();

	// Case '2'::Case '1'::Case '2'
	void groupChat(); // в доработке

	// Case '2'::Case '2'
	void printUsersMessages();

	// Case '2'::Case '3'
	void setNewPass();

	// Case '2'::Case '0'
	void readLogs();

	// Case '3'
	void printUsersFromServer(); 

private:
};