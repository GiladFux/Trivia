#include "GameManager.h"

GameManager::GameManager(IDataBase* db) : m_database(db)
{
}

Game& GameManager::createGame(Room& room, unsigned int numOfQuestions)
{
    Game* newGame = new Game(m_database, room, numOfQuestions);
    m_games.push_back(*newGame);
    return *newGame;
}
void GameManager::deleteGame(unsigned int gameId)
{
    for (auto it = m_games.begin(); it != m_games.end(); ++it)
    {
        if (it->getId() == gameId)
        {
            m_games.erase(it);
            break;
        }
    }
}

Game& GameManager::getGame(unsigned int id)
{
    for (auto& game : m_games)
    {
        if (game.getId() == id)
        {
            return game;
            break;
        }

    }
}
