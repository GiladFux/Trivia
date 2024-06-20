#include "RoomPlayerRequestHandler.h"
#include "Communicator.h"
RoomPlayerRequestHandler::RoomPlayerRequestHandler(RequestHandlerFactory& factory, Room room, LoggedUser user, RoomManager& roomManager) : m_handlerFactory(factory), m_room(room), m_roomManager(roomManager), m_user(user)
{
}


RequestResult RoomPlayerRequestHandler::handleRequest(const RequestInfo& info)
{
    switch (info.requestID)
    {
    case CLOSE_ROOM:
        return closeRoom(info);
    case LEAVE_ROOM:
        return leaveRoom(info);
    case LEAVE_RESULTS:
        return leaveRoom(info, false);
    case EXIT_ROOM:
        return leaveRoom(info);
    case START_GAME:
        return startRoom(info);
    case GET_ROOM_STATE:
        return getRoomState(info);
    }
}

RequestResult RoomPlayerRequestHandler::closeRoom(const RequestInfo& info) 
{
    RequestResult result;
    LeaveRoomResponse resp;
    try
    {
        m_roomManager.deleteRoom(m_room.getData().id); // remove from manager
        LeaveRoomResponse resp;
        resp.status = CLOSE_ROOM;//changed it to close room to inform that the user left
        Buffer buffer = JsonResponsePacketSerializer::serializeResponse(resp);
        char* msg = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
        for (auto& user : m_room.getUsers())
        {
            try
            {
                m_room.removeUser(user);
            }
            catch (std::exception e)
            {
                std::cerr << e.what() << "\n";
            }
            if (!(user == m_user))
            {
                Communicator::getInstance().setHandler(user.getSocket(), m_handlerFactory.createMenuRequestHandler(user)); // set user's new handler back to menu
                Controller::sendMessage(user.getSocket(), msg);                
            }
        }
        m_handlerFactory.getGameManager().deleteGame(m_room.getData().id);
        m_roomManager.deleteRoom(m_room.getData().id);
        result.response = msg;
        result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
    }
    catch (std::exception e)
    {
        ErrorResponse resp;
        resp.message = e.what();
        Buffer buffer = JsonResponsePacketSerializer::serializeResponse(resp);
        char* msg = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
        result.newHandler = this;
        result.response = msg;
    }

    return result;
}

RequestResult RoomPlayerRequestHandler::startRoom(const RequestInfo& info) 
{
    
    RequestResult result;
    StartGameResponse resp;
    try
    {
        resp.status = START_GAME;
        Buffer buffer = JsonResponsePacketSerializer::serializeResponse(resp);
        char* msg = JsonResponsePacketSerializer::convertBufferToCstring(buffer);

        for (auto& user : m_room.getUsers())
        {
            if (!(user == m_user))
            {
                Controller::sendMessage(user.getSocket(), msg);
                Communicator::getInstance().setHandler(user.getSocket(), m_handlerFactory.createGameRequestHandler(user, m_room)); 
            }
            else
            {
                m_handlerFactory.getGameManager().createGame(m_room, m_room.getData().numOfQuestionsInGame);
            }

        }
        m_handlerFactory.getRoomManager().getRoom(m_room.getData().id).setIsActive(true);
        delete[] msg;
    }
    catch (std::exception e)
    {
        resp.status = FAILED;
    }
    Buffer buffer = JsonResponsePacketSerializer::serializeResponse(resp);
    result.response = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
    result.newHandler = m_handlerFactory.createGameRequestHandler(m_user, m_room); 

    return result;
}

RequestResult RoomPlayerRequestHandler::getRoomState(const RequestInfo& info)
{
    RequestResult result;
    result.newHandler = this;
    GetRoomStateResponse resp;
    m_room = m_handlerFactory.getRoomManager().getRoom(m_room.getData().id);
    try
    {
        resp.answerTimeout = m_room.getData().timePerQuestion;
        resp.hasGameBegun = m_room.getData().isActive;
        resp.players = m_room.getAllUsers();
        resp.questionCount = m_room.getData().numOfQuestionsInGame;
        resp.status = GET_ROOM_STATE;
    }
    catch(std::exception e)
    {
        ErrorResponse resp;
        resp.message = e.what();
        Buffer buffer = JsonResponsePacketSerializer::serializeResponse(resp);
        result.response = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
        return result;
    }
    
    
    Buffer buffer = JsonResponsePacketSerializer::serializeResponse(resp);
    result.response = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
    return result;
}

RequestResult RoomPlayerRequestHandler::leaveRoom(const RequestInfo& info, bool sendUpdates)
{
    RequestResult result;
    LeaveRoomResponse resp;
    try
    {
        m_handlerFactory.getRoomManager().getRoom(m_room.getData().id).removeUser(m_user);
        resp.status = LEAVE_ROOM;
        resp.username = m_user.getName();   
        Buffer buffer = JsonResponsePacketSerializer::serializeResponse(resp);
        char* msg = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
        if (sendUpdates) // check if should send messages to other users
        {
            for (auto& user : m_room.getUsers())
            {
                if (!(user == m_user))
                    Controller::sendMessage(user.getSocket(), msg);

            }
        }
        if (m_handlerFactory.getRoomManager().getRoom(m_room.getData().id).getUsers().size() == 0) // no more users left
            m_handlerFactory.getRoomManager().deleteRoom(m_room.getData().id);
        delete[] msg;

        resp.status = 45;

    }
    catch (std::exception e)
    {
        ErrorResponse errResp;
        errResp.message = e.what();
        Buffer buffer = JsonResponsePacketSerializer::serializeResponse(errResp);
        result.response = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
        return result;
    }
    if (info.requestID == EXIT_ROOM)
    {
        Communicator::getInstance().setHandler(m_user.getSocket(), m_handlerFactory.createLoginRequestHandler(m_user.getSocket()));
        result.newHandler = m_handlerFactory.createLoginRequestHandler(m_user.getSocket());
        m_handlerFactory.getLoginManager().logout(m_user);
    }
    else if(info.requestID == LEAVE_ROOM)
    {
        Communicator::getInstance().setHandler(m_user.getSocket(), m_handlerFactory.createMenuRequestHandler(m_user));

    }
    result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);

    Buffer buffer = JsonResponsePacketSerializer::serializeResponse(resp);
    result.response = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
    return result;
}
