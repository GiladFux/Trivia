#pragma once
#include "LoggedUser.h"
#include "UserNotInRoomException.h"
#include "JsonResponsePacketSerializer.h"
#include "Controller.h"
#include <iostream>

class LoggedUser;
class Room
{
public:
	Room() = default;
	Room(RoomData data);
	void addUser(const LoggedUser& newUser);
	void removeUser(LoggedUser& userToRemove);
	std::vector<std::string> getAllUsers();
	std::vector<LoggedUser> getUsers();
	RoomData getData() const;
	void setIsActive(const bool isActive);
	bool isActive() const;
private:
	RoomData m_metadata;
	std::vector<LoggedUser> m_users;


};

