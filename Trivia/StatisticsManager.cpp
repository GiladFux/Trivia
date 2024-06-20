#include "StatisticsManager.h"

StatisticsManager::StatisticsManager(IDataBase* db) : m_database(db)
{
}

std::vector<UserScore> StatisticsManager::getHighScore()
{
    return m_database->getHighScores();
}

std::vector<std::string> StatisticsManager::getUserStatistics(const std::string& username)
{
    std::vector<std::string> stats;
    float avgTime = m_database->getPlayerAverageAnswerTime(username);
    int totalAns = m_database->getNumOfTotalAnswers(username);
    int correctAns = m_database->getNumOfCorrectAnswers(username);
    int gamesAmount = m_database->getNumOfPlayerGames(username);
    int score = m_database->getTotalPlayerScore(username);
    stats.push_back("AverageTime: " + std::to_string(avgTime));
    stats.push_back("TotalAnswers: " + std::to_string(totalAns));
    stats.push_back("TotalCorrectAnswers: " + std::to_string(correctAns));
    stats.push_back("TotalGamesPlayed: " + std::to_string(gamesAmount));
    stats.push_back("TotalScore:" + std::to_string(score));
    stats.push_back("UserName:" + username);
    return stats;
}
