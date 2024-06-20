#pragma once
#include <string>
#include "IRequestHandler.h"
#include <ctime>
#include <vector>
#include <unordered_map>
#include "Question.h"
class Question;
typedef std::vector<unsigned char> Buffer;
class IRequestHandler;
enum codes { CLOSE_ROOM = 40, START_GAME = 41, GET_ROOM_STATE = 42, LEAVE_ROOM = 43,EXIT_ROOM = 44 , LEAVE_ROOM_SUCCESS = 45, LEAVE_RESULTS = 46,
	CREATE_ROOM = 50, GET_ROOMS = 51, GET_PLAYERS = 52, JOIN_ROOM = 53, GET_STATS = 54, GET_LEADERBOARD = 55,
	LEAVE_GAME = 60, GET_QUESTION = 61, SUBMIT_ANSWER = 62, GET_GAME_RESULT = 63,
	LOGOUT = 203, LOGIN = 200,
	ERROR_MSG = 100, SIGNUP = 201, FAILED = 101, SUCCESS = 202 };

struct LoginResponse
{
	unsigned int status;
};

struct SignupResponse
{
	unsigned int status;
};

struct SignupRequest
{
	std::string username;
	std::string password;
	std::string email;
};

struct LoginRequest
{
	std::string username;
	std::string password;
};

struct RequestResult
{
	char* response;
	IRequestHandler* newHandler;
};

struct RequestInfo
{
	unsigned int requestID;
	time_t receivalTime = 0;
	Buffer buffer;
};

struct ErrorResponse
{
	std::string message;
};

struct LogoutResponse
{
	unsigned int status;
};

struct RoomData
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	bool isActive;
};
struct GetRoomResponse
{
	unsigned int status;
	std::vector<RoomData> rooms;
};

struct GetPlayersInRoomResponse
{
	std::vector<std::string> players;
};
struct UserScore
{
	std::string username;
	unsigned int score;
	UserScore(const std::string& username, int score) : username(username), score(score) {}

};
struct GetHighScoreResponse
{
	unsigned int status;
	std::vector<UserScore> statistics;
};

struct GetPersonalStatsResponse
{
	unsigned int status;
	std::vector<std::string> statistics;
};

struct JoinRoomResponse
{
	unsigned int status;
};

struct CreateRoomResponse
{
	unsigned int status;
	unsigned int id;
};

struct CreateRoomRequst
{
	std::string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;
};

struct GetPlayersInRoomRequst
{
	unsigned int roomid;
};

struct JoinRoomRequst
{
	unsigned int roomid;
};


struct CloseRoomResponse
{
	unsigned int status;
};

struct StartGameResponse
{
	unsigned int status;
};

struct GetRoomStateResponse
{
	unsigned int status;
	bool hasGameBegun;
	std::vector<std::string> players;
	unsigned int questionCount;
	int answerTimeout;
};

struct LeaveRoomResponse
{
	unsigned int status;
	std::string username;
};

struct LeaveGameResponse
{
	unsigned int status;
};

struct GetQuestionResponse
{
	unsigned int status;
	std::string question;
	std::unordered_map<unsigned int, std::string> answers;
};

struct SubmitAnswerResponse
{
	unsigned int status;
	unsigned int correctAnswerId;
};

struct PlayerResult
{
	std::string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	float averageAnswerTime;
	unsigned int score;
};

struct GetGameResultsResponse
{
	unsigned int status;
	bool isWinner = false;
	std::vector<PlayerResult> results;
};
struct SubmitAnswerRequest
{
	unsigned int answerId;
	float time;
};

struct GameData
{
	std::string username;
	Question currentQuestion;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	float averageAnswerTime;
	unsigned int currentQuestionIndex ;
	GameData() : correctAnswerCount(0), wrongAnswerCount(0), averageAnswerTime(0), currentQuestionIndex(0)
	{
	}
};