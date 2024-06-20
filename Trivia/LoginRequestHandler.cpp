#include "LoginRequestHandler.h"

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& factory, SOCKET sock) : m_handlerFactory(factory), m_socket(sock)
{
}

//checking if the request is relevant by checking if this is a LOGIN or SIGNUP request (since we are at the begginning stage of handling clients)
bool LoginRequestHandler::isRequestRelevant(const RequestInfo& info)
{
    return info.requestID == LOGIN || info.requestID == SIGNUP;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& info)
{
    switch (info.requestID)
    {
    case LOGIN:
        return login(info);
    case SIGNUP:
        return signup(info);
    default:
    {
        // TODO: throw exception and logout the user

    }
    }

}

// these two function will return request result with the response from the server and new handler of the user

RequestResult LoginRequestHandler::login(const RequestInfo& info)
{
    RequestResult result;
    LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer);
    try
    {
        bool resp = m_handlerFactory.getLoginManager().login(req.username, req.password, m_socket);
        LoginResponse r;
        r.status = SUCCESS;
        result.newHandler = m_handlerFactory.createMenuRequestHandler(m_handlerFactory.getLoginManager().getLoggedUser(req.username));
        Buffer b = JsonResponsePacketSerializer::serializeResponse(r);
        result.response = JsonResponsePacketSerializer::convertBufferToCstring(b);
    }
    catch(const std::exception& e)
    {
        ErrorResponse r;
        r.message = e.what();
        result.newHandler = this;
        Buffer b = JsonResponsePacketSerializer::serializeResponse(r);
        result.response = JsonResponsePacketSerializer::convertBufferToCstring(b);
    }

    return result;
}
RequestResult LoginRequestHandler::signup(const RequestInfo& info)
{
    RequestResult result;
    SignupResponse r;
    SignupRequest req = JsonRequestPacketDeserializer::deserializeSignupRequest(info.buffer);
    try
    {
        bool resp = m_handlerFactory.getLoginManager().signup(req.username, req.password, req.email, m_socket);
        r.status = SUCCESS;
        Buffer b = JsonResponsePacketSerializer::serializeResponse(r);
        result.response = JsonResponsePacketSerializer::convertBufferToCstring(b);
        result.newHandler = m_handlerFactory.createMenuRequestHandler(m_handlerFactory.getLoginManager().getLoggedUser(req.username));

    }
    catch (const std::exception& e)
    {
        ErrorResponse err;
        err.message = e.what();
        Buffer b = JsonResponsePacketSerializer::serializeResponse(err);
        result.response = JsonResponsePacketSerializer::convertBufferToCstring(b);
        result.newHandler = this;
    }
    return result;
}