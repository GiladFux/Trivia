#pragma once
#include "MyExceptions.h"
class InvalidQuestionsAmountException :	public MyException
{
public:
	InvalidQuestionsAmountException() : MyException("Only 1-50 question can be in a game")
	{}
};

