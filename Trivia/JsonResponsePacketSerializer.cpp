#include "JsonResponsePacketSerializer.h"
#include <bitset>
#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
using json = nlohmann::json;

// this function return a Buffer of error message by the protocol
Buffer JsonResponsePacketSerializer::serializeResponse(ErrorResponse resp)
{
	Buffer respBuffer;
	
	json responseJson =
	{
		{"ERROR", resp.message}
	};
	Buffer dataBuffer = convertJsonToBuffer(responseJson);
	Buffer packetBuffer = getPacketBuffer(calculateLengthOfJson(responseJson), ERROR_MSG);
	respBuffer = getFullProtocolPacket(packetBuffer, dataBuffer);
	return respBuffer;

}
// this function return buffer of login response by the protocol
Buffer JsonResponsePacketSerializer::serializeResponse(LoginResponse resp)
{
	Buffer respBuffer;
	json responseJson =
	{
		{"Status", resp.status}
	};
	Buffer dataBuffer = convertJsonToBuffer(responseJson);
	Buffer packetBuffer = getPacketBuffer(calculateLengthOfJson(responseJson), resp.status);
	respBuffer = getFullProtocolPacket(packetBuffer, dataBuffer);
	return respBuffer;

}

// this fucntion return a buffer of signup response
Buffer JsonResponsePacketSerializer::serializeResponse(SignupResponse resp)
{
	Buffer respBuffer;
	json responseJson =
	{
		{"Status", resp.status}
	};
	Buffer dataBuffer = convertJsonToBuffer(responseJson);
	Buffer packetBuffer = getPacketBuffer(calculateLengthOfJson(responseJson), resp.status);
	respBuffer = getFullProtocolPacket(packetBuffer, dataBuffer);
	return respBuffer;
}

// this function return a Buffer that represent the json in binary
Buffer JsonResponsePacketSerializer::convertJsonToBuffer(json& jsonObject)
{
	std::string jsonString = jsonObject.dump();
	Buffer bytes(jsonString.begin(), jsonString.end());
	return bytes;
}

Buffer JsonResponsePacketSerializer::serializeResponse(LogoutResponse resp)
{
	Buffer respBuffer;
	json responseJson =
	{
		{"Status", resp.status}
	};
	Buffer dataBuffer = convertJsonToBuffer(responseJson);
	Buffer packetBuffer = getPacketBuffer(calculateLengthOfJson(responseJson), resp.status);
	respBuffer = getFullProtocolPacket(packetBuffer, dataBuffer);
	return respBuffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetRoomResponse resp) // serialize by creating json that contains array of roomData
{
	Buffer respBuffer;
	json responseJson;
	// create a json array to store room data
	json roomsArray = json::array();
	for (const auto& room : resp.rooms) 
	{
		// Create a json object for each RoomData
		json roomObject = 
		{
			{"id", room.id},
			{"name", room.name},
			{"maxPlayers", room.maxPlayers},
			{"numOfQuestionsInGame", room.numOfQuestionsInGame},
			{"timePerQuestion", room.timePerQuestion},
			{"isActive", room.isActive}
		};
		roomsArray.push_back(roomObject);
	}
	responseJson["Rooms"] = roomsArray;

	Buffer dataBuffer = convertJsonToBuffer(responseJson);
	Buffer packetBuffer = getPacketBuffer(calculateLengthOfJson(responseJson), resp.status);
	respBuffer = getFullProtocolPacket(packetBuffer, dataBuffer);
	return respBuffer;
}



Buffer JsonResponsePacketSerializer::serializeResponse(GetPlayersInRoomResponse resp) // return buffer of data stores in json object, players names in array
{
	Buffer respBuffer;
	json responseJson;
	// create a json array to store players data
	json playersArray = json::array();
	for (const auto& player : resp.players)
	{
		playersArray.push_back(player);
	}
	responseJson["PlayersInRoom"] = playersArray;

	Buffer dataBuffer = convertJsonToBuffer(responseJson);
	Buffer packetBuffer = getPacketBuffer(calculateLengthOfJson(responseJson), SUCCESS);
	respBuffer = getFullProtocolPacket(packetBuffer, dataBuffer);
	return respBuffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse resp)
{
	Buffer respBuffer;
	json responseJson =
	{
		{"Status", resp.status}
	};
	Buffer dataBuffer = convertJsonToBuffer(responseJson);
	Buffer packetBuffer = getPacketBuffer(calculateLengthOfJson(responseJson), resp.status);
	respBuffer = getFullProtocolPacket(packetBuffer, dataBuffer);
	return respBuffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse resp)
{
	Buffer respBuffer;
	json responseJson =
	{
		{"Status", resp.status},
		{"ID", resp.id}
	};
	Buffer dataBuffer = convertJsonToBuffer(responseJson);
	Buffer packetBuffer = getPacketBuffer(calculateLengthOfJson(responseJson), resp.status);
	respBuffer = getFullProtocolPacket(packetBuffer, dataBuffer);
	return respBuffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetHighScoreResponse resp)
{
	Buffer respBuffer;
	json responseJson;
	json statisticsArray = json::array();
	for (const auto& statistic : resp.statistics) 
	{
		json userScoreJson =
		{
			{"UserName", statistic.username},
			{"Score", statistic.score}
		};
		statisticsArray.push_back(userScoreJson);
	}

	responseJson["Statistics"] = statisticsArray;
	Buffer dataBuffer = convertJsonToBuffer(responseJson);

	Buffer packetBuffer = getPacketBuffer(calculateLengthOfJson(responseJson), resp.status);
	respBuffer = getFullProtocolPacket(packetBuffer, dataBuffer);
	return respBuffer;
}


Buffer JsonResponsePacketSerializer::serializeResponse(GetPersonalStatsResponse resp) 
{
	Buffer respBuffer;
	json responseJson;

	json statisticsArray = json::array();
	for (const auto& statistic : resp.statistics)
	{
		statisticsArray.push_back(statistic);
	}

	responseJson["Statistics"] = statisticsArray;
	Buffer dataBuffer = convertJsonToBuffer(responseJson);

	Buffer packetBuffer = getPacketBuffer(calculateLengthOfJson(responseJson), resp.status);
	respBuffer = getFullProtocolPacket(packetBuffer, dataBuffer);
	return respBuffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(CloseRoomResponse resp)
{
	Buffer respBuffer;
	json responseJson =
	{
		{"Status", resp.status}
	};
	Buffer dataBuffer = convertJsonToBuffer(responseJson);
	Buffer packetBuffer = getPacketBuffer(calculateLengthOfJson(responseJson), resp.status);
	respBuffer = getFullProtocolPacket(packetBuffer, dataBuffer);
	return respBuffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(StartGameResponse resp)
{
	Buffer respBuffer;
	json responseJson =
	{
		{"Status", resp.status}
	};
	Buffer dataBuffer = convertJsonToBuffer(responseJson);
	Buffer packetBuffer = getPacketBuffer(calculateLengthOfJson(responseJson), resp.status);
	respBuffer = getFullProtocolPacket(packetBuffer, dataBuffer);
	return respBuffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetRoomStateResponse resp)
{
	Buffer respBuffer;
	json responseJson =
	{
		{"Status", resp.status},
		{"HasGameBegun", resp.hasGameBegun},
		{"Players", resp.players},
		{"QuestionCount", resp.questionCount},
		{"AnswerTimeOut", resp.answerTimeout}
	
	};
	Buffer dataBuffer = convertJsonToBuffer(responseJson);
	Buffer packetBuffer = getPacketBuffer(calculateLengthOfJson(responseJson), resp.status);
	respBuffer = getFullProtocolPacket(packetBuffer, dataBuffer);
	return respBuffer;
	return Buffer();
}

Buffer JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse resp)
{
	Buffer respBuffer;
	json responseJson =
	{
		{"Status", resp.status},
		{"User" , resp.username}
	};
	Buffer dataBuffer = convertJsonToBuffer(responseJson);
	Buffer packetBuffer = getPacketBuffer(calculateLengthOfJson(responseJson), resp.status);
	respBuffer = getFullProtocolPacket(packetBuffer, dataBuffer);
	return respBuffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetGameResultsResponse resp)
{
	Buffer respBuffer;
	
	json resultsArray = json::array();
	for (const auto& result : resp.results)
	{
		resultsArray.push_back(gameResultToJson(result));
	}
	json responseJson =
	{
		{"Status", resp.status}, 
		{"Results", resultsArray},
		{"isWinner", resp.isWinner}
	};
	Buffer dataBuffer = convertJsonToBuffer(responseJson);
	Buffer packetBuffer = getPacketBuffer(calculateLengthOfJson(responseJson), resp.status);
	respBuffer = getFullProtocolPacket(packetBuffer, dataBuffer);
	return respBuffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(SubmitAnswerResponse resp)
{
	
	Buffer respBuffer;
	json responseJson =
	{
		{"Status", resp.status},
		{"CorrectID" , resp.correctAnswerId}
	};
	Buffer dataBuffer = convertJsonToBuffer(responseJson);
	Buffer packetBuffer = getPacketBuffer(calculateLengthOfJson(responseJson), resp.status);
	respBuffer = getFullProtocolPacket(packetBuffer, dataBuffer);
	return respBuffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetQuestionResponse resp)
{
	Buffer respBuffer;
	json answersJson = json::object();
	for (const auto& pair : resp.answers) 
	{
		answersJson[std::to_string(pair.first)] = pair.second;
	}
	json responseJson =
	{
		{"Status", resp.status},
		{"QuestionString", resp.question},
		{"Answers", answersJson}
	};
	Buffer dataBuffer = convertJsonToBuffer(responseJson);
	Buffer packetBuffer = getPacketBuffer(calculateLengthOfJson(responseJson), resp.status);
	respBuffer = getFullProtocolPacket(packetBuffer, dataBuffer);
	return respBuffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(LeaveGameResponse resp)
{
	Buffer respBuffer;
	json responseJson =
	{
		{"Status", resp.status}
	};
	Buffer dataBuffer = convertJsonToBuffer(responseJson);
	Buffer packetBuffer = getPacketBuffer(calculateLengthOfJson(responseJson), resp.status);
	respBuffer = getFullProtocolPacket(packetBuffer, dataBuffer);
	return respBuffer;
}


json JsonResponsePacketSerializer::gameResultToJson(const PlayerResult result)
{
	return json{
		{"averageAnswerTime", result.averageAnswerTime},
		{"correctAnswerCount", result.correctAnswerCount},
		{"username", result.username},
		{"wrongAnswerCount", result.wrongAnswerCount},
		{"score", result.score}

	};
}


int JsonResponsePacketSerializer::calculateLengthOfJson(json object)
{
	std::string jsonObject = object.dump();
	return jsonObject.length() * 8;
}

std::string JsonResponsePacketSerializer::intToBinary(int number)
{
	return std::bitset<32>(number).to_string();

}
// this function return Buffer of 4 bytes of a number (size of data)
Buffer JsonResponsePacketSerializer::get4BytesFromNumber(size_t size)
{
	Buffer respBuffer;
	std::string bits = intToBinary(size);
	for (int i = 0; i < 32; i += 8) 
	{
		std::string byteString = bits.substr(i, 8); // extract 8 bits
		unsigned char byte = std::bitset<8>(byteString).to_ulong(); // convert to byte
		respBuffer.push_back(byte);
	}
	return respBuffer;
}
// this function serialize a message excluding the data
Buffer JsonResponsePacketSerializer::getPacketBuffer(int size, int code)
{
	Buffer respBuffer;
	Buffer bytes = get4BytesFromNumber(size); // get size of data in 32 bits
	std::bitset<8> codeBits(code);
	// Push binary representation of code
	unsigned char codeByte = static_cast<unsigned char>(codeBits.to_ulong());
	respBuffer.push_back(codeByte);


	
	for (const auto byte : bytes) // insert msg size in binary
	{
		respBuffer.push_back(byte);
	}

	return respBuffer;
}

// this function construct a buffer of packet buffer (Code and Length), and data then return it
Buffer JsonResponsePacketSerializer::getFullProtocolPacket(Buffer& packetBuffer, Buffer& dataBuffer)
{
	Buffer respBuffer;
	for (const auto& byte : packetBuffer) // insert code and size into buffer
	{
		respBuffer.push_back(byte);
	}
	for (const auto& byte : dataBuffer) // insert json bytes into buffer
	{
		respBuffer.push_back(byte);
	}
	return respBuffer;
}

char* JsonResponsePacketSerializer::convertBufferToCstring(Buffer& buffer)
{
	std::string binaryString;
	for (const auto byte : buffer)
	{
		for (int i = 7; i >= 0; --i) // iterate over each bit in the byte
		{
			binaryString += ((byte >> i) & 1) ? '1' : '0'; // convert to binary and right shift the byte
		}
	}

	char* binaryCString = new char[binaryString.length() + 1];
	strcpy_s(binaryCString, binaryString.length() + 1, binaryString.c_str());
	return binaryCString;
}


Buffer JsonResponsePacketSerializer::stringToBuffer(const char* str)
{
	//turning the char * to buffer
	Buffer buffer(str, str + strlen(str));
	return buffer;
}

