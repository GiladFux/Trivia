#pragma once
struct RequestInfo;
struct RequestResult;
#include "structs.h"
#include "AlreadyLoggedInException.h"
#include "WrongDetailsException.h"
#include "UserAlreadyExistsException.h"

class IRequestHandler
{
public:
	virtual ~IRequestHandler() = default;

	virtual bool isRequestRelevant(const RequestInfo& info) = 0;
	virtual RequestResult handleRequest(const RequestInfo& info) = 0;

};