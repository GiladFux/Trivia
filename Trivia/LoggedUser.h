#pragma once
#include <string>
#include <WinSock2.h>
class LoggedUser
{
public:
	LoggedUser();
	LoggedUser(std::string username, SOCKET sock);
	bool operator==(const LoggedUser& other) const;
	bool operator<(const LoggedUser& other) const;
	std::string getName() const;
	SOCKET getSocket() const;


private:
	std::string m_username;
	SOCKET m_socket;
};