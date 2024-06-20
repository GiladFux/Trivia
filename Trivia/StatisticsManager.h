#pragma once
#include "IDataBase.h"
#include <vector>
#include <string>
class StatisticsManager
{
public:
	StatisticsManager(IDataBase* db);
	std::vector<UserScore> getHighScore();
	std::vector<std::string> getUserStatistics(const std::string& username);

private:
	IDataBase* m_database;
};

