using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace Client
{
    internal class Serializer
    {
        public static byte[] seriallizeRequest(string json, int code )
        {
            byte[] jsonBytes = Encoding.UTF8.GetBytes(json);
            int jsonLength = jsonBytes.Length;
            byte[] msgLengthBytes = BitConverter.GetBytes(IPAddress.HostToNetworkOrder(jsonLength));

            byte[] combinedBytes = new byte[1 + 4 + jsonLength];
            combinedBytes[0] = (byte)code;
            Buffer.BlockCopy(msgLengthBytes, 0, combinedBytes, 1, 4);
            Buffer.BlockCopy(jsonBytes, 0, combinedBytes, 1 + 4, jsonLength);
            return combinedBytes;
        }
    }
}
