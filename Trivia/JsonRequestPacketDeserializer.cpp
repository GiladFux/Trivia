#include "JsonRequestPacketDeserializer.h"
#include <bitset>
#include <iostream>


unsigned int JsonRequestPacketDeserializer::deserializeCode(char buffer)
{
//casting the first byte (the code) to a integer by casting it first to unsigned char (because for some reason when trying to cast it first to 
//unsigned int its not actually working well ) and then casting it to unsigned int 
    unsigned int code = static_cast<unsigned int>(static_cast<unsigned char>(buffer));
    return code;
}

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(Buffer buffer)
{
    //creating loginRequest 
    LoginRequest request;
    //getting the position of the last } (the end of the json msg) and then setting the message to the json binary and then transfering it to json 
    //using the deseriallizeBytesToJson function
    auto it = std::find(buffer.begin(), buffer.end(), '}');
    int pos = it - buffer.begin() + 1;
    std::string Message(buffer.begin(), buffer.begin() + pos);
    json jsonRequest = deserializeBytesToJson(Message);
    //setting up the login request properties
    request.username = jsonRequest["username"];
    request.password = jsonRequest["password"];
    return request;
}


SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(Buffer buffer)
{
    //creating signupRequest 
    SignupRequest request;
    //getting the position of the last } (the end of the json msg) and then setting the message to the json binary and then transfering it to json 
    //using the deseriallizeBytesToJson function
    auto it = std::find(buffer.begin(), buffer.end(), '}');
    int pos = it - buffer.begin() + 1;
    std::string Message(buffer.begin(), buffer.begin() + pos);
    json jsonRequest = deserializeBytesToJson(Message);
    //setting up the signup request properties
    request.username = jsonRequest["username"];
    request.password = jsonRequest["password"];
    request.email = jsonRequest["email"];
    return request;

}

GetPlayersInRoomRequst JsonRequestPacketDeserializer::deserializeGetPlayersRequest(Buffer buffer)
{
    GetPlayersInRoomRequst request; 
    auto it = std::find(buffer.begin(), buffer.end(), '}');
    int pos = it - buffer.begin() + 1;
    std::string Message(buffer.begin(), buffer.begin() + pos);
    json jsonRequest = deserializeBytesToJson(Message);
    request.roomid = jsonRequest["RoomID"];
    return request;
}

JoinRoomRequst JsonRequestPacketDeserializer::deserializeJoinRoomRequest(Buffer buffer)
{
    JoinRoomRequst request;
    auto it = std::find(buffer.begin(), buffer.end(), '}');
    int pos = it - buffer.begin() + 1;
    std::string Message(buffer.begin(), buffer.begin() + pos);
    json jsonRequest = deserializeBytesToJson(Message);
    request.roomid = jsonRequest["RoomID"];
    return request;
}

CreateRoomRequst JsonRequestPacketDeserializer::deserializeCreateRoomRequest(Buffer buffer)
{
    CreateRoomRequst request;
    auto it = std::find(buffer.begin(), buffer.end(), '}');
    int pos = it - buffer.begin() + 1;
    std::string Message(buffer.begin(), buffer.begin() + pos);
    json jsonRequest = deserializeBytesToJson(Message);
    request.roomName = jsonRequest["roomName"];
    request.answerTimeout = jsonRequest["answerTimeout"];
    request.maxUsers = jsonRequest["maxUsers"];
    request.questionCount = jsonRequest["questionCount"];
    return request;
}

SubmitAnswerRequest JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(Buffer buffer)
{
    SubmitAnswerRequest request;
    auto it = std::find(buffer.begin(), buffer.end(), '}');
    int pos = it - buffer.begin() + 1;
    std::string Message(buffer.begin(), buffer.begin() + pos);
    json jsonRequest = deserializeBytesToJson(Message);
    request.answerId = jsonRequest["answerId"];
    request.time = jsonRequest["time"];
    return request;
}

unsigned int JsonRequestPacketDeserializer::deserializeLength(char* buffer)
{
    unsigned int length = 0;
    for (int i = 0; i < 4; i++) 
    {
        length *= 10;
        length += deserializeCode(buffer[i]);
    }
    return length;
}

json JsonRequestPacketDeserializer::deserializeBytesToJson(const std::string& jsonString)
{
    //using this method in order to transfer it from binary text to json
    try
    {
        json jsonObject = json::parse(jsonString);
        return jsonObject;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        throw; 
    }
}


