#pragma once
#include "IRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "RequestHandlerFactory.h"
#include "UserNotInRoomException.h"
#include "Communicator.h"
class RequestHandlerFactory;


class RoomPlayerRequestHandler :  public IRequestHandler
{
public:
	RoomPlayerRequestHandler(RequestHandlerFactory& factory, Room room, LoggedUser user, RoomManager& roomManager);
	virtual bool isRequestRelevant(const RequestInfo& info) override = 0;
	RequestResult handleRequest(const RequestInfo& info) override;
private:
	RequestHandlerFactory& m_handlerFactory;
	Room m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestResult closeRoom(const RequestInfo& info);
	RequestResult startRoom(const RequestInfo& info);
	RequestResult getRoomState(const RequestInfo& info);
	RequestResult leaveRoom(const RequestInfo& info, bool sendUpdates = true);	
};

