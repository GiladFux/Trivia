#pragma once
#include "RequestHandlerFactory.h"
#include "RoomPlayerRequestHandler.h"


class RoomMemberRequestHandler : public RoomPlayerRequestHandler
{
public:
	RoomMemberRequestHandler(RequestHandlerFactory& factory, Room room, LoggedUser user, RoomManager& roomManager);
	virtual bool isRequestRelevant(const RequestInfo& info) override;
};

