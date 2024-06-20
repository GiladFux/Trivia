#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "Room.h"
#include "NoMoreQuestionsException.h"
class RequestHandlerFactory;
class GameRequestHandler :	public IRequestHandler
{
public:
	GameRequestHandler(LoggedUser user, RequestHandlerFactory& factory, Room& room);
	bool isRequestRelevant(const RequestInfo& info) override;
	RequestResult handleRequest(const RequestInfo& info) override;
private:
	Room m_room;
	LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;
	RequestResult leaveGame(const RequestInfo& info);
	RequestResult getQuestion(const RequestInfo& info);
	RequestResult submitAnswer(const RequestInfo& info);
	RequestResult getGameResult(const RequestInfo& info);





};

