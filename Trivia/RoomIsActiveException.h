#pragma once
#include "MyExceptions.h"
class RoomIsActiveException :	public MyException
{
public:
	RoomIsActiveException() : MyException("A game is already in progress in this room")
	{}
};

