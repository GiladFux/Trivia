#include "Game.h"
#include <algorithm>
#include <random>
#include "NoMoreQuestionsException.h"
Game::Game(IDataBase* db, Room room, unsigned int numOfQuestions) : m_database(db), m_gameId(room.getData().id), m_room(room), m_numOfQuestions(numOfQuestions)
{
    for (auto& user : room.getUsers())
    {
        GameData data;
        data.username = user.getName();
        m_players[user] = data;
    }
    std::list<Question> questions = m_database->getQuestions(numOfQuestions);

    // Shuffle the questions
    auto rng = std::default_random_engine{ std::random_device{}() };
    std::vector<Question> questionsVector(questions.begin(), questions.end());
    std::shuffle(questionsVector.begin(), questionsVector.end(), rng);

    for (auto& q : questionsVector)
    {
        m_questions.push_back(q);
    }
}

Question Game::getQuestionForUser(LoggedUser user)
{
        if (m_players[user].currentQuestionIndex == m_numOfQuestions)
            throw NoMoreQuestionsException();
        Question q = m_questions[m_players[user].currentQuestionIndex]; // return next question in vector and inc the index
        return q;
}

void Game::removePlayer(LoggedUser user) 
{
    submitGameStatsToDB(m_players[user]);
    m_players.erase(user);
}

int Game::submitAnswer(unsigned int answerID, LoggedUser& user, float time)
{
    GameData& data = m_players[user];
    try
    {
        data.currentQuestion = getQuestionForUser(user); // update new question
    }
    catch (NoMoreQuestionsException e)
    {
        std::cerr << e.what();
    }
    if (answerID == data.currentQuestion.getCorrectAnswerID()) // answer is correct
        data.correctAnswerCount++;
    else // incorrect
        data.wrongAnswerCount++;
    int totalAnswers = data.correctAnswerCount + data.wrongAnswerCount;

    data.averageAnswerTime = (float)(data.averageAnswerTime + time) / totalAnswers; // update time average

    int currQuestionCorrectId = data.currentQuestion.getCorrectAnswerID();
    m_players[user] = data;
    m_players[user].currentQuestionIndex++;
    return currQuestionCorrectId; // return the correct answer id to the user

}

std::vector<PlayerResult> Game::getGameResults()
{
    std::vector<PlayerResult> playerResults;
    for (auto GameData : m_players)
    {
        PlayerResult playerResult;
        playerResult.username = GameData.first.getName();
        playerResult.averageAnswerTime = GameData.second.averageAnswerTime;
        playerResult.correctAnswerCount = GameData.second.correctAnswerCount;
        playerResult.wrongAnswerCount = GameData.second.wrongAnswerCount;
        playerResult.score = m_database->getPlayerScore(playerResult);
        playerResults.push_back(playerResult);
    }
    return playerResults;
}

unsigned int Game::getId() const
{
    return m_gameId;
}

void Game::submitGameStatsToDB(GameData data)
{
    m_database->submitGameStatistics(data);
}
