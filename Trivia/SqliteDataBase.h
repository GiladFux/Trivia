#pragma once
#include "IDataBase.h"
#include <io.h>
#include <iostream>
#include <windows.h>
#include <wininet.h>
#include "JsonRequestPacketDeserializer.h"
#define TRIVIA_SITE "https://opentdb.com/api.php?amount=50&category=18&difficulty=medium&type=multiple"
#define TIME_FOR_ANSWER 10
#pragma comment(lib, "wininet")

class SqliteDatabase : public IDataBase
{
public:
	SqliteDatabase();
	bool doesUserExists(const std::string& username) override;
	bool doesPasswordMatch(const std::string& username,const std::string& password) override;
	void addNewUser(const std::string& username, const std::string& password, const std::string& email) override;
	//stats
	std::list<Question> getQuestions(const int amount) override;
	float getPlayerAverageAnswerTime(const std::string& playerName) override;
	int getNumOfCorrectAnswers(const std::string& playerName) override;
	int getNumOfTotalAnswers(const std::string& playerName) override;
	int getNumOfPlayerGames(const std::string& playerName) override;
	int getPlayerScore(const PlayerResult& result) override;
	int getTotalPlayerScore(const std::string& username) override;
	std::vector<UserScore> getHighScores() override;
	int submitGameStatistics(GameData data) override;
	
private:
	static int questionCallBack(void* data, int argc, char** argv, char** azColName);
	static int usernameCallBack(void* data, int argc, char** argv, char** azColName);
	std::string decodeHtmlEntities(const std::string& str);
	void initiallzeDB();
	static int countCallBack(void* data, int argc, char** argv, char** azColName);
	void CreateDB();
	bool open();
	void executeOperation(const std::string& command, int (*callback)(void*, int, char**, char**) = nullptr, void* data = nullptr);
	sqlite3* m_db;
	
};
