#pragma once
#include "MyExceptions.h"
class WrongDetailsException : public MyException
{
public:
	WrongDetailsException() : MyException("Wrong password or username")
	{}
};

