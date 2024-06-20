#pragma once
#include "structs.h"
#include "Json.hpp"
#include <vector>

typedef std::vector<unsigned char> Buffer;
using json = nlohmann::json;

class JsonRequestPacketDeserializer
{
public :
	static unsigned int deserializeCode(char buffer);
	static LoginRequest deserializeLoginRequest(Buffer buffer);
	static SignupRequest deserializeSignupRequest(Buffer buffer);
	static GetPlayersInRoomRequst deserializeGetPlayersRequest(Buffer buffer);
	static JoinRoomRequst deserializeJoinRoomRequest(Buffer buffer);
	static CreateRoomRequst deserializeCreateRoomRequest(Buffer buffer);
	static SubmitAnswerRequest deserializeSubmitAnswerRequest(Buffer buffer);
	static unsigned int deserializeLength(char* buffer);
private:
	static json deserializeBytesToJson(const std::string& jsonString);

};

