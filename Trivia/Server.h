#pragma once
#include "Communicator.h"
#include "WSAInitializer.h"
#include <iostream>
#include <thread>
#include <string>
#include <exception>
#include "IDataBase.h"
#include "SqliteDataBase.h"
#define PORT 6969
class Server
{
public:
	Server();
	void run();
private:
	IDataBase* m_database;
	RequestHandlerFactory* m_handlerFactory;
	Communicator& m_communicator;
	static void getAdminCommands();
};

