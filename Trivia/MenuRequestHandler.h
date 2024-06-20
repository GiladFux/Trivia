#pragma once
#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "RequestHandlerFactory.h"
class RequestHandlerFactory;
class MenuRequestHandler : public IRequestHandler
{
public:
	MenuRequestHandler(LoggedUser user, RequestHandlerFactory& factory);
	bool isRequestRelevant(const RequestInfo& info) override;
	RequestResult handleRequest(const RequestInfo& info) override;
private:
	LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;
	RequestResult createRoom(const RequestInfo& info);
	RequestResult getRooms(const RequestInfo& info);
	RequestResult joinRoom(const RequestInfo& info);
	RequestResult logout(const RequestInfo& info);
	RequestResult getPersonalStats(const RequestInfo& info);
	RequestResult getPlayersInRoom(const RequestInfo& info);
	RequestResult getHighScore(const RequestInfo& info);

};
