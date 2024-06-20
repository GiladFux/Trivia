#pragma once
#include "MyExceptions.h"
class UserAlreadyExistsException : public MyException
{
public:
	UserAlreadyExistsException() : MyException("User already exists")
	{}
};

