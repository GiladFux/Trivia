#include "RoomManager.h"
#include <iostream>
unsigned int RoomManager::currentID = 1; // first RoomID
std::mutex RoomManager::idMutex;

void RoomManager::createRoom(const LoggedUser& roomOwner, const RoomData data) // add room to rooms map and ass the owner user
{
    Room newRoom(data);
    newRoom.addUser(roomOwner);
    m_rooms[data.id] = newRoom;
    
}

void RoomManager::deleteRoom(const int id)
{
    m_rooms.erase(id);
}

bool RoomManager::getRoomState(int ID)
{
    return m_rooms[ID].getData().isActive;
}

std::vector<RoomData> RoomManager::getRooms() const
{
    std::vector<RoomData> rooms;
    for (auto& room : m_rooms)
    {
        if(!room.second.isActive())
            rooms.push_back(room.second.getData());
    }
    return rooms;
}

Room& RoomManager::getRoom(int ID)
{
    return m_rooms[ID];
}

unsigned int RoomManager::getIdForNewRoom()
{
    std::lock_guard<std::mutex> lock(idMutex); // Lock the mutex

    unsigned int id = currentID; 
    ++currentID;               
    return id;                    
}
