using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace Client
{
    internal class deserializer
    {
        public static int deseriallizeCode(byte[] firstBytes)
        {
            string code = "";
            // Assuming each byte represents 8 bits
            for (int i = 0; i < 8; i++)
            {
                code += Convert.ToChar(firstBytes[i]);
            }
            int decimalValue = Convert.ToInt32(code, 2);
            return decimalValue;
        }
        public static string deseriallizeResponse(byte[] responseBytes)
        {
            string lengthBits = "";
            string request = "";
            // Assuming each byte represents 8 bits
            for (int i = 0; i < 32; i++) // deserialize first 4 bytes
            {
                lengthBits += Convert.ToChar(responseBytes[i]);
            }
            int length = Convert.ToInt32(lengthBits, 2);
            for(int i  = 0; i < length; i++) // deserialize all data by the length
            {
                request += Convert.ToChar(responseBytes[32 + i]);
            }
            byte[] bytes = new byte[request.Length / 8];
            for (int i = 0; i < request.Length; i += 8)
            {
                bytes[i / 8] = Convert.ToByte(request.Substring(i, 8), 2);
            }
            string jsonString = Encoding.UTF8.GetString(bytes);
            return jsonString;
        }

    }
}
