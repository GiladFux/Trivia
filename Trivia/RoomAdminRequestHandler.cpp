#include "RoomAdminRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(RequestHandlerFactory& factory, Room room, LoggedUser user, RoomManager& roomManager) : RoomPlayerRequestHandler(factory, room, user, roomManager)
{
}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& info)
{
    return info.requestID == CLOSE_ROOM || info.requestID == START_GAME || info.requestID == GET_ROOM_STATE || info.requestID == LEAVE_RESULTS;
}

