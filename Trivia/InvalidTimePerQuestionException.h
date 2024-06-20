#pragma once
#include "MyExceptions.h"
class InvalidTimePerQuestionException :	public MyException
{
public:
	InvalidTimePerQuestionException() : MyException("Time per question must be 5-30")
	{}
};

