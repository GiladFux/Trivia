#pragma once
#include "MyExceptions.h"
class RoomIsFullException : public MyException
{
public:
    RoomIsFullException() : MyException("Room is full")
    {}
};

