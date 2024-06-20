#include "MenuRequestHandler.h"
#include "InvalidTimePerQuestionException.h"
#include "InvalidPlayersAmountException.h"
#include "InvalidQuestionsAmountException.h"
#include "RoomIsActiveException.h"
MenuRequestHandler::MenuRequestHandler(LoggedUser user, RequestHandlerFactory& factory) : m_handlerFactory(factory), m_user(user)
{
}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& info)
{
    unsigned int id = info.requestID;
    return id == CREATE_ROOM || id == GET_ROOMS || id == GET_PLAYERS || id == JOIN_ROOM || id == GET_STATS  || id == GET_LEADERBOARD|| id == LOGOUT;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& info)
{
    switch (info.requestID)
    {
    case CREATE_ROOM:
    {
        return createRoom(info);
        break;
    }
    case GET_ROOMS:
    {
        return getRooms(info);
        break;
    }
    case GET_PLAYERS:
    {
        return getPlayersInRoom(info);
        break;
    }
    case JOIN_ROOM:
    {
        return joinRoom(info);
        break;
    }
    case GET_STATS:
    {
        return getPersonalStats(info);
        break;
    }
    case GET_LEADERBOARD:
        return getHighScore(info);
        break;
    case LOGOUT:
    {
        return logout(info);
        break;
    }
    }
}

RequestResult MenuRequestHandler::createRoom(const RequestInfo& info)
{
    CreateRoomRequst req = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(info.buffer);
    RoomData data;
    RequestResult result;
    ErrorResponse r;
    try
    {
        if (req.answerTimeout < 5 || req.answerTimeout > 30)
            throw InvalidTimePerQuestionException();
        if (req.maxUsers > 10 || req.maxUsers < 1)
            throw InvalidPlayersAmountException();
        if (req.questionCount > 50 || req.questionCount < 1)
            throw InvalidQuestionsAmountException();
        data.maxPlayers = req.maxUsers;
        data.name = req.roomName;
        data.timePerQuestion = req.answerTimeout;
        data.numOfQuestionsInGame = req.questionCount;
        data.isActive = false;
        data.id = m_handlerFactory.getRoomManager().getIdForNewRoom();
        m_handlerFactory.getRoomManager().createRoom(m_user, data);
        result.newHandler = m_handlerFactory.createRoomAdminRequestHandler(m_user, m_handlerFactory.getRoomManager().getRoom(data.id));
        CreateRoomResponse resp;
        resp.id = data.id;
        resp.status = SUCCESS;

        Buffer buffer = JsonResponsePacketSerializer::serializeResponse(resp);
        result.response = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
    }
    catch (const std::exception& e)
    {
        r.message = e.what();
        Buffer b = JsonResponsePacketSerializer::serializeResponse(r);
        result.response = JsonResponsePacketSerializer::convertBufferToCstring(b);
        result.newHandler = this; // unable to create a room
    }
    
    return result;
}

RequestResult MenuRequestHandler::getRooms(const RequestInfo& info)
{
    RequestResult result;
    GetRoomResponse resp;
    resp.rooms = m_handlerFactory.getRoomManager().getRooms();
    resp.status = SUCCESS;
    Buffer buffer = JsonResponsePacketSerializer::serializeResponse(resp);
    result.response = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
    result.newHandler = this;
   
    
    
    return result;
}


RequestResult MenuRequestHandler::joinRoom(const RequestInfo& info)
{
    RequestResult result;
    JoinRoomRequst req = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(info.buffer);
    JoinRoomResponse resp;
    try
    {
        if (m_handlerFactory.getRoomManager().getRoom(req.roomid).isActive())
            throw RoomIsActiveException();
        m_handlerFactory.getRoomManager().getRoom(req.roomid).addUser(m_user);
        resp.status = SUCCESS;
        result.newHandler = m_handlerFactory.createRoomMemberRequestHandler(m_user, m_handlerFactory.getRoomManager().getRoom(req.roomid)); 
        Buffer buffer = JsonResponsePacketSerializer::serializeResponse(resp);
        result.response = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
        
    }
    catch(const std::exception& e)
    {
        ErrorResponse r;
        r.message = e.what();
        Buffer buffer = JsonResponsePacketSerializer::serializeResponse(r);
        result.response = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
        result.newHandler = this;
    }
    
    
   
    return result;
}


RequestResult MenuRequestHandler::logout(const RequestInfo& info)
{
    
    bool resp = m_handlerFactory.getLoginManager().logout(m_user); // logout and get json response
    RequestResult result;
    LogoutResponse response;
    if (resp) // need to sign out
    {
        result.newHandler = m_handlerFactory.createLoginRequestHandler(m_user.getSocket());
        response.status = SUCCESS;
    }
    else // stay in menu handler
    {
        result.newHandler = this;
        response.status = FAILED;
    }
    Buffer buffer = JsonResponsePacketSerializer::serializeResponse(response);
    result.response = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
    return result;
}

RequestResult MenuRequestHandler::getHighScore(const RequestInfo& info)
{
    std::vector<UserScore> scores = m_handlerFactory.getStatisticsManager().getHighScore();
    RequestResult result;
    GetHighScoreResponse response;
    response.statistics = scores;
    if (!scores.empty()) 
    {
        response.status = SUCCESS;
        result.newHandler = this;
    }
    else // stay in menu handler
    {
        response.status = FAILED;
        result.newHandler = this;
    }
    Buffer scoresBuffer = JsonResponsePacketSerializer::serializeResponse(response);
    result.response = JsonResponsePacketSerializer::convertBufferToCstring(scoresBuffer);
    return result;
}

RequestResult MenuRequestHandler::getPersonalStats(const RequestInfo& info)
{
    RequestResult result;
    GetPersonalStatsResponse resp;
    resp.statistics = m_handlerFactory.getStatisticsManager().getUserStatistics(m_user.getName());
    resp.status = SUCCESS;
    Buffer buffer = JsonResponsePacketSerializer::serializeResponse(resp);
    result.response = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
    result.newHandler = this;
    return result;
}


RequestResult MenuRequestHandler::getPlayersInRoom(const RequestInfo& info)
{
    RequestResult result;
    GetPlayersInRoomRequst req = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(info.buffer);
    GetPlayersInRoomResponse resp;
    Room& currentRoom = m_handlerFactory.getRoomManager().getRoom(req.roomid);
    resp.players = currentRoom.getAllUsers();
    Buffer buffer = JsonResponsePacketSerializer::serializeResponse(resp);
    result.response = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
    result.newHandler = this;
    return result;
}
