#pragma once
#include "IDataBase.h"
#include <vector>
#include "Game.h"
#include "Room.h"
class GameManager
{
public:
	GameManager(IDataBase* db);
	Game& createGame(Room& room, unsigned int numOfQuestions);
	void deleteGame(unsigned int gameId);
	Game& getGame(unsigned int id);
private:
	IDataBase* m_database;
	std::vector<Game> m_games;
};

