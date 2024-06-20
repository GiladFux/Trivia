#pragma once
#include "RequestHandlerFactory.h"
#include "RoomPlayerRequestHandler.h"
class RequestHandlerFactory;


class RoomAdminRequestHandler :	public RoomPlayerRequestHandler
{
public:
	RoomAdminRequestHandler(RequestHandlerFactory& factory, Room room, LoggedUser user, RoomManager& roomManager);
	bool isRequestRelevant(const RequestInfo& info) override;
};

