#pragma once
#include "IDatabase.h"
#include <vector>
#include <iostream>
#include <string>
#include "LoggedUser.h"
#include "Json.hpp"
#include "structs.h"
#include <WinSock2.h>
using json = nlohmann::json;

class MenuRequestHandler;
class LoginManager
{
public:
	LoginManager(IDataBase* db);
	bool signup(const std::string& username, const std::string& password, const std::string& email, SOCKET sock);
	bool login(const std::string& username, const std::string& password, SOCKET sock);
	bool logout(const LoggedUser& user);
	LoggedUser& getLoggedUser(const std::string& username);
	LoggedUser getUserBySocket(SOCKET userSock);

private:
	IDataBase* m_db;
	std::vector<LoggedUser> m_loggedUsers;

};