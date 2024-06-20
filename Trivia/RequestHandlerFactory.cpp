#include "RequestHandlerFactory.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
RequestHandlerFactory::RequestHandlerFactory(IDataBase* db) : m_database(db), m_loginManager(LoginManager(db)), m_statisticsManager(StatisticsManager(db)), m_gameManager(db)
{   
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler(SOCKET sock)
{
    LoginRequestHandler* newLogin = new LoginRequestHandler(*this, sock);
    return newLogin;
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser& user)
{
    MenuRequestHandler* newMenu = new MenuRequestHandler(user, *this);
    return newMenu;
}

IRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser& user, Room room)
{
    return new RoomAdminRequestHandler(*this, room, user, m_roomManager);
}

IRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser& user, Room room)
{
    return new RoomMemberRequestHandler(*this, room, user, m_roomManager);
}

GameRequestHandler* RequestHandlerFactory::createGameRequestHandler(LoggedUser& user, Room& room)
{
    return new GameRequestHandler(user, *this, room);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
    return m_loginManager;
}

StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
    return m_statisticsManager;
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
    return m_roomManager;
}

GameManager& RequestHandlerFactory::getGameManager()
{
    return m_gameManager;
}
