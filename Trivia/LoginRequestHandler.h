#pragma once
#include "IRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "RequestHandlerFactory.h"
#include <WinSock2.h>
class RequestHandlerFactory;
class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(RequestHandlerFactory& factory, SOCKET sock);
	bool isRequestRelevant(const RequestInfo& info) override;
	RequestResult handleRequest(const RequestInfo& info) override;
private:
	SOCKET m_socket;
	RequestHandlerFactory& m_handlerFactory;
	RequestResult login(const RequestInfo& info);
	RequestResult signup(const RequestInfo& info);
};

