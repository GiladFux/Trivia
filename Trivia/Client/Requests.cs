using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
    internal class Requests
    {
        public struct LoginMessage
        {
            public string username { get; }
            public string password { get; }

            public LoginMessage(string Username, string Password)
            {
                username = Username;
                password = Password;
            }
        }

        public struct SignupMessage
        {
            public string username { get; }
            public string password { get; }
            public string email { get; }

            public SignupMessage(string Username, string Password, string Email)
            {
                username = Username;
                password = Password;
                email = Email;
            }
        }
        public struct CreateRoomRequest
        {
            public string roomName { get; }
            public int maxUsers { get; }
            public int questionCount { get; }
            public int answerTimeout {  get; }

            public CreateRoomRequest(string RoomName , int MaxUsers, int QuestionCount, int AnswerTimeout)
            {
                roomName = RoomName;
                maxUsers = MaxUsers;
                questionCount = QuestionCount;
                answerTimeout = AnswerTimeout;
            }
        }
        public struct JoinRoomRequest
        {
            public uint RoomID { get; }
            public JoinRoomRequest(uint roomID)
            {
                RoomID = roomID; 
            }
        }
        public struct GetPlayersRequest
        {
            public uint RoomID { get; }
            public GetPlayersRequest(uint roomID)
            {
                RoomID = roomID;
            }
        }
        
    }
}
