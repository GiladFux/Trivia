#pragma once
#include "MyExceptions.h"
class InvalidPlayersAmountException :	public MyException
{
public:
	InvalidPlayersAmountException() : MyException("Only 1-10 players can be play in one lobby")
	{}
};

