#pragma once
#include "Question.h"
#include "structs.h"
#include <vector>
#include <unordered_map>
#include "LoggedUser.h"
#include "IDataBase.h"
#include <list>
#include "Room.h"
#include <map>
class Game
{
public:
	Game(IDataBase* db, Room room, unsigned int numOfQuestions);
	Question getQuestionForUser(LoggedUser user);
	void removePlayer(LoggedUser user);
	int submitAnswer(unsigned int answerID, LoggedUser& user, float time);
	std::vector<PlayerResult> getGameResults();
	unsigned int getId() const;
private:
	// fields
	IDataBase* m_database;
	std::vector<Question> m_questions;
	std::map<LoggedUser, GameData> m_players;
	unsigned int m_gameId;
	unsigned int m_numOfQuestions;
	Room m_room;
	// methods
	void submitGameStatsToDB(GameData data);

	

};

