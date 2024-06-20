#include "Question.h"

Question::Question() : m_correctID(-1)
{
}

Question::Question(const std::string& question,
    const std::string& correctAns,
    const std::string& wrongAns1,
    const std::string& wrongAns2,
    const std::string& wrongAns3)
    : m_question(question), m_possibleAnswers{ correctAns, wrongAns1, wrongAns2, wrongAns3 }
{
    // shuffle the answers vector
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(m_possibleAnswers.begin(), m_possibleAnswers.end(), g);

    // find the index of the correct answer
    auto it = std::find(m_possibleAnswers.begin(), m_possibleAnswers.end(), correctAns);
    if (it != m_possibleAnswers.end()) 
    {
        m_correctID = std::distance(m_possibleAnswers.begin(), it);
    }
    else 
    {
        // indicates error
        m_correctID = -1;
    }
}

std::string Question::getQuestion() const
{
    return m_question;
}

std::vector<std::string> Question::getPossibleAnswers() const
{
    return m_possibleAnswers;
}

int Question::getCorrectAnswerID() const
{
    return m_correctID;
}
