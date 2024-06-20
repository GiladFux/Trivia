using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
    struct Room
    {
        public uint id ;
        public bool isActive;
        public uint maxPlayers;
        public string name;
        public uint numOfQuestionsInGame;
        public uint timePerQuestion;
    }
    struct Rooms
    {
        public Room[] rooms;
    }
    struct Players
    {
        public string[] PlayersInRoom;
    }
    struct createRoomResponse
    {
        public uint ID;
    }
    struct RoomState
    {
        public uint AnswerTimeOut;
        public uint QuestionCount;
        public string[] Players;
        public bool HasGameBegun;
        public uint Status;

    }
    struct PossibleAnswers
    {
        public Dictionary<uint, string> Answers;
    }

    struct Question
    {
        public Dictionary<uint, string> Answers;
        public string QuestionString;
        public uint Status;
    }
    struct SubmitAnswerResponse
    {
        public uint Status;
        public uint CorrectID;
    }
    struct SubmitAnswerRequest
    {
        public uint answerId;
        public float time;
    }
    struct GameResults
    {
        public bool isWinner;
        public int Status;
        public GameResult[] Results;
    }
    struct GameResult
    {
        public float averageAnswerTime;
        public int correctAnswerCount;
        public int score;
        public string username;
        public int wrongAnswerCount;
    }
    struct ErrorResponse
    {
        public string ERROR;
    }
    struct PersonalStatistics
    {
        public string[] Statistics;
    }
    struct Stats
    {
        public string UserName;
        public uint Score;
    }

    struct TotalStatistics
    {
        public Stats[] Statistics;
    }



}
