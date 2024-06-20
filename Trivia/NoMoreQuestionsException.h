#pragma once
#include "MyExceptions.h"
class NoMoreQuestionsException : public MyException
{
public:
	NoMoreQuestionsException() : MyException("There are no more available questions")
	{}
};

