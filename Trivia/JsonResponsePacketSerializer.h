#pragma once
#include "structs.h"
#include "Json.hpp"
#include <vector>
#include "Question.h"
using json = nlohmann::json;
typedef std::vector<unsigned char> Buffer;
class JsonResponsePacketSerializer
{
public:
	static Buffer stringToBuffer(const char* str);
	static char* convertBufferToCstring(Buffer& buffer);
	static Buffer convertJsonToBuffer(json& jsonObject);
	static Buffer serializeResponse(ErrorResponse resp);
	static Buffer serializeResponse(LoginResponse resp);
	static Buffer serializeResponse(SignupResponse resp);
	static Buffer serializeResponse(LogoutResponse resp);
	static Buffer serializeResponse(GetRoomResponse resp);
	static Buffer serializeResponse(GetPlayersInRoomResponse resp);
	static Buffer serializeResponse(JoinRoomResponse resp);
	static Buffer serializeResponse(CreateRoomResponse resp);
	static Buffer serializeResponse(GetHighScoreResponse resp);
	static Buffer serializeResponse(GetPersonalStatsResponse resp);
	static Buffer serializeResponse(CloseRoomResponse resp);
	static Buffer serializeResponse(StartGameResponse resp);
	static Buffer serializeResponse(GetRoomStateResponse resp);
	static Buffer serializeResponse(LeaveRoomResponse resp);
	static Buffer serializeResponse(GetGameResultsResponse resp);
	static Buffer serializeResponse(SubmitAnswerResponse resp);
	static Buffer serializeResponse(GetQuestionResponse resp);
	static Buffer serializeResponse(LeaveGameResponse resp);


	// helper methods
private:
	static json gameResultToJson(const PlayerResult result);
	static int calculateLengthOfJson(json object);
	static std::string intToBinary(int number);
	static Buffer get4BytesFromNumber(size_t size);
	static Buffer getPacketBuffer(int size, int code);
	static Buffer getFullProtocolPacket(Buffer& packetBuffer, Buffer& dataBuffer);
	
};

