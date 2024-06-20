#include "Controller.h"
#include <exception>
#include <iostream>
void Controller::sendMessage(SOCKET sock, char* msg)
{
	try
	{
		send(sock, msg, strlen(msg), 0);
	}
	catch (std::exception e)
	{
		std::cerr << e.what() << "\n";
	}
}
