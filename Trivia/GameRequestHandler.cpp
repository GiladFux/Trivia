#include "GameRequestHandler.h"
#include <unordered_map>
#include <algorithm>
GameRequestHandler::GameRequestHandler(LoggedUser user, RequestHandlerFactory& factory, Room& room) : m_user(user), m_handlerFactory(factory), m_room(room)
{
}

bool GameRequestHandler::isRequestRelevant(const RequestInfo& info)
{
    return info.requestID == LEAVE_GAME || info.requestID == GET_QUESTION || info.requestID == SUBMIT_ANSWER || info.requestID == GET_GAME_RESULT;
}

RequestResult GameRequestHandler::handleRequest(const RequestInfo& info)
{
    switch (info.requestID)
    {
    case LEAVE_GAME:
    {
        return leaveGame(info);
        break;
    }
    case GET_QUESTION:
    {
        return getQuestion(info);
        break;
    }
    case SUBMIT_ANSWER:
    {
        return submitAnswer(info);
        break;
    }
    case GET_GAME_RESULT:
    {
        return getGameResult(info);
        break;
    }
    }
    return RequestResult();
}

RequestResult GameRequestHandler::leaveGame(const RequestInfo& info)
{
    RequestResult result;
    LeaveGameResponse resp;
    try
    {
        m_handlerFactory.getGameManager().getGame(m_room.getData().id).removePlayer(m_user);
        resp.status = SUCCESS;
        Buffer buffer = JsonResponsePacketSerializer::serializeResponse(resp);
        char* msg = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
        result.response = msg;
        result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
    }
    catch (std::exception e)
    {
        ErrorResponse r;
        r.message = e.what();
        Buffer b = JsonResponsePacketSerializer::serializeResponse(r);
        char* msg = JsonResponsePacketSerializer::convertBufferToCstring(b);
        result.response = msg;
        result.newHandler = this;
    }
    return result;
}

RequestResult GameRequestHandler::getQuestion(const RequestInfo& info)
{
    GetQuestionResponse resp;
    unsigned int roomID = m_room.getData().id;
    RequestResult result;
    try
    {
        Question q = m_handlerFactory.getGameManager().getGame(roomID).getQuestionForUser(m_user);
        std::vector<std::string> possibleAns = q.getPossibleAnswers();
        resp.status = 1;
        std::unordered_map<unsigned int, std::string> answers;
        for (int i = 0; i < 4; i++)
        {
            answers[i] = possibleAns[i];
        }
        resp.answers = answers;
        resp.question = q.getQuestion();
        Buffer buffer = JsonResponsePacketSerializer::serializeResponse(resp);
        char* msg = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
        result.response = msg;
        result.newHandler = this;
    }
    catch (std::exception e)
    {
        resp.status = 0;
        ErrorResponse r;
        r.message = e.what();
        Buffer buffer = JsonResponsePacketSerializer::serializeResponse(r);
        char* msg = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
        result.response = msg;
        result.newHandler = this;
        
    }
    
    
    return result;
}

RequestResult GameRequestHandler::submitAnswer(const RequestInfo& info) 
{
    SubmitAnswerResponse resp;
    RequestResult result;
    SubmitAnswerRequest req = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(info.buffer);
    resp.correctAnswerId = m_handlerFactory.getGameManager().getGame(m_room.getData().id).submitAnswer(req.answerId, m_user, req.time);
    resp.status = SUCCESS;
    Buffer buffer = JsonResponsePacketSerializer::serializeResponse(resp);
    char* msg = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
    result.response = msg;
    result.newHandler = this;
    
    
    return result;
}

RequestResult GameRequestHandler::getGameResult(const RequestInfo& info) 
{
    RequestResult reqResult;
    GetGameResultsResponse resp;
    try 
    {   
        unsigned int roomID = m_room.getData().id;
        std::vector<PlayerResult> results = m_handlerFactory.getGameManager().getGame(roomID).getGameResults();
        std::sort(results.begin(), results.end(), [](const PlayerResult& a, const PlayerResult& b) {return b.score < a.score; }); // sort by score
        if (results[0].username == m_user.getName())
            resp.isWinner = true;
        resp.results = results;
        resp.status = SUCCESS;
        Buffer buffer = JsonResponsePacketSerializer::serializeResponse(resp);
        char* msg = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
        reqResult.response = msg;
        m_handlerFactory.getGameManager().getGame(m_room.getData().id).removePlayer(m_user);
        if (m_room.getUsers()[0] == m_user) // current user is admin
            reqResult.newHandler = m_handlerFactory.createRoomAdminRequestHandler(m_user, m_room);
        else // normal member
            reqResult.newHandler = m_handlerFactory.createRoomMemberRequestHandler(m_user, m_room);
    }
    catch (std::exception e)
    {
        resp.status = 0;
        ErrorResponse r;
        r.message = e.what();
        Buffer buffer = JsonResponsePacketSerializer::serializeResponse(r);
        char* msg = JsonResponsePacketSerializer::convertBufferToCstring(buffer);
        reqResult.response = msg;
        reqResult.newHandler = this;

    }
    return reqResult;
 
}
