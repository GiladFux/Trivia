#pragma once
#include "MyExceptions.h"
class UserNotInRoomException :
    public MyException
{
public:
	UserNotInRoomException() : MyException("No such user in the room")
	{}
};

