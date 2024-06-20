#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <random>
class Question
{
public:
	Question();
	Question(const std::string& question,
		const std::string& correctAns,
		const std::string& wrongAns1,
		const std::string& wrongAns2,
		const std::string& wrnogAns3);
	std::string getQuestion() const;
	std::vector<std::string> getPossibleAnswers() const;
	int getCorrectAnswerID() const;
private:
	std::string m_question;
	std::vector<std::string> m_possibleAnswers;
	int m_correctID;

};

