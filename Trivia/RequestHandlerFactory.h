#pragma once
#include <WinSock2.h>
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "LoginManager.h"
#include "StatisticsManager.h"
#include "RoomManager.h"
#include "GameRequestHandler.h"
#include "GameManager.h"

class LoginRequestHandler;
class MenuRequestHandler;
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;
class RoomManager;
class Room;
class GameRequestHandler;
class RequestHandlerFactory
{
public:
    // methods
    RequestHandlerFactory(IDataBase* db);
    LoginRequestHandler* createLoginRequestHandler(SOCKET sock);
    MenuRequestHandler* createMenuRequestHandler(LoggedUser& user);
    IRequestHandler* createRoomAdminRequestHandler(LoggedUser& user, Room room);
    IRequestHandler* createRoomMemberRequestHandler(LoggedUser& user, Room room);
    GameRequestHandler* createGameRequestHandler(LoggedUser& user, Room& room);
    LoginManager& getLoginManager();
    StatisticsManager& getStatisticsManager();
    RoomManager& getRoomManager();
    GameManager& getGameManager();

private:
    // fields
    IDataBase* m_database;
    LoginManager m_loginManager;
    StatisticsManager m_statisticsManager;
    RoomManager m_roomManager;
    GameManager m_gameManager;

};