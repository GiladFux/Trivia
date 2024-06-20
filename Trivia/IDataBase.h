#pragma once
#include <string>
#include "sqlite3.h"
#include <list>
#include "Question.h"
#include "structs.h"
class IDataBase 
{
public:

	virtual bool doesUserExists(const std::string& name) = 0;
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) = 0;
	virtual void addNewUser(const std::string& username, const std::string& password, const std::string& email) = 0;
	//stats
	virtual std::list<Question> getQuestions(const int amount) = 0;
	virtual float getPlayerAverageAnswerTime(const std::string& playerName) = 0;
	virtual int getNumOfCorrectAnswers(const std::string& playerName) = 0;
	virtual int getNumOfTotalAnswers(const std::string& playerName) = 0;
	virtual int getNumOfPlayerGames(const std::string& playerName) = 0;
	virtual int getPlayerScore(const PlayerResult& result) = 0;
	virtual int getTotalPlayerScore(const std::string& username) = 0;

	virtual std::vector<UserScore> getHighScores() = 0;
	virtual int submitGameStatistics(GameData data) = 0;



};