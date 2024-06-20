using System.Net.Sockets;

namespace Client
{
    internal class Communicator
    {
        private static Communicator instance;
        private Socket clientSocket;
        public void endCommunication()
        {
            clientSocket.Close();
        }
        public bool isSocketEmpty()
        {
            return clientSocket.Available == 0;
        }
        private Communicator(string IP, int PORT)
        {
            clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            clientSocket.Connect(IP, PORT);
        }

        public static Communicator GetInstance(string IP, int PORT)
        {
            //IP = "13.38.86.243"; uncomment when using the VM server
            if (instance == null)
            {
                instance = new Communicator(IP, PORT);
            }
            return instance;
        }

        public void SendMsg(byte[] msg)
        {
            clientSocket.Send(msg);
            
        }

        public (int code, string response) ReceiveCodeAndResponse()
        {
            byte[] codeBytes = new byte[8]; // Adjust the buffer size as needed
            clientSocket.Receive(codeBytes);
            int code = deserializer.deseriallizeCode(codeBytes);

            // Receive the response
            byte[] responseBytes = new byte[8192]; // Adjust the buffer size as needed
            clientSocket.Receive(responseBytes);
            string response = deserializer.deseriallizeResponse(responseBytes);

            // return tuple of code and response
            return (code, response);
        }
    }
}
