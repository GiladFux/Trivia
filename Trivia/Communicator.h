#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <thread>
#include <unordered_map>
#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"
#define BUFFER_SIZE 1025
class RequestHandlerFactory;
class Communicator
{
public:
	static Communicator& getInstance(RequestHandlerFactory* factory = nullptr);
	void startHandleRequests(int port);
	void setHandler(SOCKET sock, IRequestHandler* newHandler);

private:
	static bool m_instanceExists;
	Communicator(RequestHandlerFactory& factory);
	~Communicator();
	static Communicator* m_instance; // store the only instance
	// members
	std::unordered_map<SOCKET, IRequestHandler*> m_clients;
	SOCKET m_serverSocket;
	RequestHandlerFactory& m_handlerfactory;
	// methods
	void bindAndListen();
	void handleNewClient(SOCKET clientSocket);

};

