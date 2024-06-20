#pragma once
#include "MyExceptions.h"
class MessageFailedException : public MyException
{
	MessageFailedException() : MyException("Message send failed")
	{}

};

