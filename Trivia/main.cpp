#include <iostream>
#include "WSAInitializer.h"
#include "Server.h"
#include "SqliteDataBase.h"
int main()
{
    try
    {
        WSAInitializer wsaInit; 

        Server mainServer;
        mainServer.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
    return 0;
}
