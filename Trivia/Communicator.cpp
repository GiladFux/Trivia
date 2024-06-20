#pragma comment(lib, "Ws2_32.lib")
#include "Communicator.h"
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include <cstring>
Communicator* Communicator::m_instance = nullptr;

Communicator::Communicator(RequestHandlerFactory& factory) : m_handlerfactory(factory)
{

	// this server use TCP. that why SOCK_STREAM & IPPROTO_TCP
	// if the server use UDP we will use: SOCK_DGRAM & IPPROTO_UDP
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");
}

Communicator::~Communicator()
{
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		closesocket(m_serverSocket);
	}
	catch (...) {}
}

Communicator& Communicator::getInstance(RequestHandlerFactory* factory)
{
	if (m_instance == nullptr)
	{
		if (factory == nullptr)
		{
			throw std::invalid_argument("First call to getInstance requires a valid RequestHandlerFactory");
		}
		m_instance = new Communicator(*factory);
	}
	return *m_instance;
	
}

void Communicator::startHandleRequests(int port)
{

	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(port); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// Connects between the socket and the configuration (port and etc..)
	if (bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		throw std::runtime_error(std::string("Failed to bind socket: ") + std::to_string(error));
	}
	// Start listening for incoming requests of clients
	if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << port << std::endl;

	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		std::cout << "Waiting for client connection request" << std::endl;
		bindAndListen();
	}
}

void Communicator::setHandler(SOCKET sock, IRequestHandler* newHandler)
{
	m_clients[sock] = newHandler;
}


void Communicator::bindAndListen()
{
	SOCKET client_socket = accept(m_serverSocket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);

	std::cout << "Client accepted. Server and client can speak" << std::endl;

	LoginRequestHandler* loginHandler = new LoginRequestHandler(m_handlerfactory, client_socket); // Create a new LoginRequestHandler object
	m_clients[client_socket] = loginHandler; // Store the pointer in the map

	std::thread clientThread(&Communicator::handleNewClient, this, client_socket);

	// Detach the thread, but ensure it's safe to delete the LoginRequestHandler later
	clientThread.detach();
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
	while (true)
	{
		try
		{
			char codeBuffer = 0;
			int received = recv(clientSocket, &codeBuffer, 1, 0);
			if (received == 0 || received == -1)
			{
				throw std::runtime_error("Connection closed by the client\n");
			}
			//check if got new msg
			if (codeBuffer == 0)
				continue;
			
			//casting the first byte to the code
			unsigned int code = JsonRequestPacketDeserializer::deserializeCode(codeBuffer);
			//getting the next 4 bytes (the length) 
			char lengthBuffer[4];
			unsigned int length = 0;
			int bytes = recv(clientSocket, lengthBuffer, 4, 0);
			if (bytes == 0 || bytes == -1)
			{
				throw std::runtime_error("Connection closed by the client\n");
			}
			length = JsonRequestPacketDeserializer::deserializeLength(lengthBuffer);
			char* buffer = new char[length];
			if (length != 0)
			{
				int receivedBytes = recv(clientSocket, buffer, length, 0);
				if (receivedBytes == 0 || receivedBytes == -1)
				{
					throw std::runtime_error("Connection closed by the client\n");
				}
			}

			//creating an requestInfo object and setting up his properties
			RequestInfo req;
			req.requestID = code;
			req.receivalTime = time(0);
			req.buffer = JsonResponsePacketSerializer::stringToBuffer(buffer);
			char* string;
			//checking if the current's user request is relevant
			if (m_clients[clientSocket]->isRequestRelevant(req))
			{
				//handling the current's user request
				RequestResult result = m_clients[clientSocket]->handleRequest(req);
				m_clients[clientSocket] = result.newHandler;
				string = result.response;
				result.response = nullptr;
				delete[] result.response;
			}
			else
			{
				ErrorResponse err;
				err.message = "Irrelevant message";
				Buffer errBuffer = JsonResponsePacketSerializer::serializeResponse(err);
				string = JsonResponsePacketSerializer::convertBufferToCstring(errBuffer);
			}
			send(clientSocket, string, strlen(string), 0);
		
			delete[] string;
			delete[] buffer;

			}
		catch (const std::exception& e)
		{
			//if there is an exception it will close the socket (end the connection) , delete the client from the clients and end the function
			try
			{
				try
				{
					LoggedUser user = m_handlerfactory.getLoginManager().getUserBySocket(clientSocket);
					m_handlerfactory.getLoginManager().logout(user);
				}
				catch (SOCKET sock)
				{
					std::cerr << "No user with socket " << sock << std::endl;
				}
				
			}
			catch (const std::exception& innerEx)
			{
				std::cerr << "Error during logout: " << innerEx.what() << std::endl;
			}
			closesocket(clientSocket);
			std::cout << e.what();
			delete m_clients[clientSocket];
			m_clients.erase(clientSocket);
			break;
		}
	}


}

