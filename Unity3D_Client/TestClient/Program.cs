using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Unity3D_Client;

namespace TestClient
{
    class Program
    {
        private static ClientSocket m_pClientSocket;

        static void Main(string[] args)
        {
            m_pClientSocket = new ClientSocket();

            if (m_pClientSocket.Connect("192.168.1.9", 8080))
            {
                Console.WriteLine("连接到服务器");
            }

            wmPacket pPacket = new wmPacket();
            pPacket.SetOpcode(1);
            pPacket.SetRetCode(0);

            pPacket.WriteFloat(10.0f);
            //pPacket.WriteDouble(20.0d);
            //pPacket.WriteUint8(1);
            //pPacket.WriteUint16(1);
            //pPacket.WriteUint32(123);
            //pPacket.WriteBoolean(true);
            string str = "测1T.。";
            pPacket.WriteString(ref str);

            m_pClientSocket.Send(pPacket);

            pPacket = new wmPacket();
            pPacket.SetOpcode(1);
            pPacket.SetRetCode(0);

            pPacket.WriteFloat(10.0f);
            //pPacket.WriteDouble(20.0d);
            //pPacket.WriteUint8(1);
            //pPacket.WriteUint16(1);
            //pPacket.WriteUint32(123);
            //pPacket.WriteBoolean(true);
            pPacket.WriteString(ref str);

            m_pClientSocket.Send(pPacket);
        }
    }
}
