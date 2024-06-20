#pragma once
#include "MyExceptions.h"
class AlreadyLoggedInException : public MyException
{
public:
	AlreadyLoggedInException() : MyException("User already logged in")
	{}
};

