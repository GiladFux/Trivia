#include "Room.h"
#include "Communicator.h"
#include "RoomIsFullException.h"
Room::Room(RoomData data) : m_metadata(data)
{
}

void Room::addUser(const LoggedUser& newUser)
{
    if (m_users.size() >= m_metadata.maxPlayers)
    {
        throw RoomIsFullException();
    }
    m_users.push_back(newUser);
}

void Room::removeUser(LoggedUser& userToRemove)
{
    auto it = std::find(m_users.begin(), m_users.end(), userToRemove);
    if (it != m_users.end()) 
    {
        m_users.erase(it);
    }
    else
    {
        throw UserNotInRoomException();
    }
}

std::vector<std::string> Room::getAllUsers()
{
    std::vector<std::string> users;
    for (const auto& user : m_users)
    {
        users.push_back(user.getName());
    }
    return users;
}

std::vector<LoggedUser> Room::getUsers()
{
    return m_users;
}

RoomData Room::getData() const
{
    return m_metadata;
}

void Room::setIsActive(const bool isActive)
{
    m_metadata.isActive = isActive;
}

bool Room::isActive() const
{
    return m_metadata.isActive;
}

