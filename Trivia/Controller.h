#pragma once
#include <WinSock2.h>
class Controller
{
public:
	static void sendMessage(SOCKET sock, char* msg);
};

