#include "LoggedUser.h"

LoggedUser::LoggedUser() : m_username("default")
{
}

LoggedUser::LoggedUser(std::string username, SOCKET sock) : m_username(username), m_socket(sock)
{
}

bool LoggedUser::operator==(const LoggedUser& other) const
{
    return m_username == other.m_username;
}

bool LoggedUser::operator<(const LoggedUser& other) const
{
    return m_username < other.m_username;
}


std::string LoggedUser::getName() const
{
    return m_username;
}

SOCKET LoggedUser::getSocket() const
{
    return m_socket;
}
