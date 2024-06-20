#include "Server.h"

Server::Server() : m_database(new SqliteDatabase), m_handlerFactory(new RequestHandlerFactory(m_database)), m_communicator(Communicator::getInstance(m_handlerFactory))
{

}

void Server::run()
{
	try
	{
		std::thread adminInput(getAdminCommands);
		adminInput.detach();
		m_communicator.startHandleRequests(PORT);
	}
	catch (std::exception& e)
	{
		std::cout << "Error occured: " << e.what() << std::endl;
	}

}

void Server::getAdminCommands() // close the program when "EXIT" is entered
{
	std::string command;
	while (true)
	{
		try
		{
			std::cin >> command;
		}
		catch (std::exception& e)
		{
			std::cerr << e.what();
		}
		if (command == "EXIT")
		{
			exit(0);
		}
		
	}
}
