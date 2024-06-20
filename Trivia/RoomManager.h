#pragma once
#include <unordered_map>
#include "Room.h"
#include <mutex>
class Room;

class RoomManager
{
public:
	void createRoom(const LoggedUser& roomOwner, const RoomData data);
	void deleteRoom(const int id);
	bool getRoomState(int ID);
	std::vector<RoomData> getRooms() const;
	Room& getRoom(int ID);
	unsigned int getIdForNewRoom();
private:
	std::unordered_map<unsigned int, Room> m_rooms;
	static unsigned int currentID; // current room ID
	static std::mutex idMutex;     // Mutex for currentID

};

