
using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;

using UnityEngine;

namespace NetClient
{
    // 声明一个委托
    public delegate void SocketEvent(object sender, EventArgs arg);

    public class ClientSocket
    {
        // 客户端连接到服务器的socket
        private Socket m_socketClient;
        // 线程接收到的数据
        private Thread m_threadReciveData;
        // 数据包队列
        private Queue<byte[]> m_queuePacket;

        // 加密模块
        private CrypTea m_pCryp;

        /// 当前使用的包
        wmPacket m_pPacket;
        /// 当前已经收到的数据长度
        ushort m_wRecved;
        /// 包头
        byte[] m_pPacketHeader;
        /// 传入的数据包操作位置偏移量
        ushort m_wRecvData;

        // 数据包事件
        public event SocketEvent SocketData;

        private List<SocketEvent> onUpdate;

        public ClientSocket()
        {
            m_socketClient = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            m_queuePacket = new Queue<byte[]>();

            NewPacket();

            byte[] aKey = new byte[16];
            aKey = System.Text.Encoding.Default.GetBytes("[/28!~&0sM1-)G&z");

            m_pCryp = new CrypTea(aKey);
        }
        // 连接服务器
        public bool Connect(string strServerIp, int nServerPort)
        {
            IPEndPoint ie = new IPEndPoint(IPAddress.Parse(strServerIp), nServerPort);
            try
            {
                m_socketClient.Connect(ie);
            }
            //  处理参数为空引用异常 
            catch (ArgumentNullException ae)
            {
                Debug.Log("ArgumentNullException : " + ae.ToString());
                return false;
            }
            // 访问Socket时操作系统发生错误引发
            catch (SocketException se)
            {
                Debug.Log("SocketException : " + se.ToString());
                return false;
            }
            catch (Exception e)
            {
                Debug.Log("Unexpected exception : " + e.ToString());
            }
            ThreadStart myThreaddelegate = new ThreadStart(ReceiveMsg);
            m_threadReciveData = new Thread(myThreaddelegate);
            m_threadReciveData.Start();

            // 更新
            Update();

            return true;
        }

        public void ReceiveMsg()
        {
            int nRecvSize = 0;

            for (; ; )
            {
                byte[] pData = new byte[1024];
                nRecvSize = m_socketClient.Receive(pData);
                BuildPacket(pData, nRecvSize);

                //string stringdata = Encoding.UTF8.GetString(data, 0, recv);
                //m_queuePacket.Enqueue(data);
            }
        }
        private void BuildPacket(byte[] pData, int nDataSize)
        {
            /// 还需要的数据长度
            int nCurRecvSize = 0;
            /// 当前读取位置
            int nCurRead = 0;
            // 包头
            byte[] pPacketHeader;
            // 数据
            byte[] pPacketData;

            for (; ; )
            {
                pPacketHeader = m_pPacket.GetPacketHeader();
                pPacketData = m_pPacket.GetData();

                if (m_pPacket.GetPacketSize() == 0)
                {
                    /** 先收包头 **/
                    nCurRecvSize = (int)(SPacketHeader.PACKET_HEADER_SIZE - m_pPacket.GetCurPacketHeaderSize());
                    
                    if (0 != nCurRecvSize)
                    {
                        if (nDataSize < nCurRecvSize)
                        {
                            Array.Copy(pData, nCurRead, pPacketHeader, m_pPacket.GetCurPacketHeaderSize(), nDataSize);
                            m_pPacket.m_wRecvLen += nDataSize;
                            return;
                        }
                        Array.Copy(pData, nCurRead, pPacketHeader, m_pPacket.GetCurPacketHeaderSize(), nCurRecvSize);
                        nDataSize -= nCurRecvSize;
                        nCurRead += nCurRecvSize;
                        m_pPacket.m_wRecvLen += nCurRecvSize;

                        // 包头OK了, 校验包头
                        if (!m_pPacket.IsValid())
                        {
                            m_pPacket = new wmPacket();
                            /// 包异常
                            return;
                        }
                    }
                }
   
                /// 包数据
                nCurRecvSize = (int)(m_pPacket.GetPacketSize() - m_pPacket.m_wRecvLen);
                if (nDataSize < nCurRecvSize)
                {
                    Array.Copy(pData, nCurRead, pPacketData, m_pPacket.m_wRecvLen - SPacketHeader.PACKET_HEADER_SIZE, nDataSize);
                    m_pPacket.m_wRecvLen += nDataSize;
                    return;
                }
                Array.Copy(pData, nCurRead, pPacketData, m_pPacket.m_wRecvLen - SPacketHeader.PACKET_HEADER_SIZE, nCurRecvSize);
                nDataSize -= nCurRecvSize;
                nCurRead += nCurRecvSize;
                m_pPacket.m_wRecvLen += nCurRecvSize;

                /// 一个完整的包
                m_pPacket.Parse(m_pCryp);
                //pEvent = new CWormEvent(CWormEvent.WORM_EVENT);
                //pEvent.m_nEventId = 1;
                //pEvent.m_pPacket = m_pPacketBuf;
                //m_pCallback.dispatchEvent(pEvent);
                // 将包压入队列
                Update();
                // New出新包
                NewPacket();

                if (0 == nDataSize)
                {
                    return;
                }
            }
        }

        void NewPacket()
        {
            m_pPacket = new wmPacket();
            m_pPacketHeader = m_pPacket.GetPacketHeader();
            m_wRecvData -= m_wRecvData;
            m_wRecved -= m_wRecved;
        }

        // 发送
        public void Send(wmPacket oPacket)
        {
            // 判断socket连接
            if (null == m_socketClient)
            {
                return;
            }
            // 判断包
            if (null == oPacket)
            {
                return;
            }

            try
            {
                // 生成完整的数据包: 加密 压缩 计算包头
                oPacket.Build(m_pCryp);
                // 包头byte
                oPacket.GetPacketHeader();
                // 数据byte
                oPacket.GetData();
                // 创建新的byte[]
                byte[] curData = new byte[oPacket.GetPacketSize()];
                // 初始化
                Array.Clear(curData, 0, curData.Length);
                // 拷贝包头部分
                Array.Copy(oPacket.GetPacketHeader(), 0, curData, 0, oPacket.GetCurPacketHeaderSize());
                // 拷贝数据部分
                Array.Copy(oPacket.GetData(), 0, curData, oPacket.GetCurPacketHeaderSize(), oPacket.GetCurDataSize());
                //// 转换为网络数据
                //Array.Reverse(curData);   
                m_socketClient.Send(curData);
            }
            catch (SocketException se)
            {
                throw (se);
            }
            catch (ObjectDisposedException ode)
            {
                throw (ode);
            }
            catch (Exception ce)
            {
                Debug.Log(string.Format("Exception : {0}", ce.ToString()));
            }
        }

        // 关闭连接
        public void Close()
        {
            //关闭Socket 
            m_socketClient.Shutdown(SocketShutdown.Both);
            m_socketClient.Close();
        }

        /** 
         * 当有一个完整包的时候，该函数将被调用，并返回数据包分流结构体PacketShunt
         * 当返回值为null时客户端应不做处理
         * */
        protected void Update()
        {
            if (m_pPacket != null)
            {
                if (onUpdate != null)
                {
                    wmPacket oPacket = new wmPacket(m_pPacket);
                    onUpdate.ForEach(updateHandler => updateHandler(this, new NetSocketEventArgs(oPacket)));
                }
            }
        }


        public void AddUpdateEvent(SocketEvent newEvent) 
        {
            if (onUpdate == null)
            {
                onUpdate = new List<SocketEvent>();
                onUpdate.Add(newEvent);
            }
            else if (!onUpdate.Contains(newEvent))
            {
                onUpdate.Add(newEvent);
            }
            else 
            {
                Debug.Log("The onUpdate has contains :"+newEvent.Method.Name);
            }
        }

    }

    public class NetSocketEventArgs : EventArgs
    {
        public wmPacket packet;

        public NetSocketEventArgs(wmPacket packet)
        {
            this.packet = packet;
        }
    }

}
