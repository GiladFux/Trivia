#include "RoomMemberRequestHandler.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(RequestHandlerFactory& factory, Room room, LoggedUser user, RoomManager& roomManager) : RoomPlayerRequestHandler(factory, room, user, roomManager)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& info)
{
    return info.requestID == LEAVE_ROOM ||  info.requestID == GET_ROOM_STATE || info.requestID == EXIT_ROOM || info.requestID == LEAVE_RESULTS;
}
