
using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;

namespace NetClient
{
    public struct SDataHeader
    {
        /// 数据的最大长度
        public static readonly uint MAX_PACKET_DATA_SIZE = 7152;

        /// 交易码
        public ushort m_wOpcode;
        /// 返回码
        public ushort m_wRetCode;
        /// 数据, 使用时做指针使用
        public byte[] m_pData;

        public void Reset()
        {
            m_wOpcode -= m_wOpcode;
            m_wRetCode -= m_wRetCode;
        }
    }

    public struct SPacketHeader
    {
        /// 包的最大长度: 7K
        public static readonly uint MAX_PACKET_SIZE = 7168;
        /// 包头长度
        public static readonly uint PACKET_HEADER_SIZE = 6;
        /// 包的最小长度
        public static readonly uint MIN_PACKET_SIZE = 10;

        /// 包的验证码
        public ushort m_wVerCode;

        /// 数据头
        public SDataHeader m_pDataHeader;

        public void Reset()
        {
            m_wVerCode -= m_wVerCode;
            m_pDataHeader.Reset();
        }
    }

    public class wmPacket
    {
        // 包头
        private byte[] m_pPacketHeader;
        // 数据头
        private byte[] m_pData;
        /// 包头的当前长度
        private ushort m_wHeaderLen;
        /// 数据的当前长度
        private ushort m_wDataLen;
        public int m_wRecvLen;

        /// 当前读取位置
        private ushort m_wCurReadPos;
        /// 当前写入位置
        private ushort m_wCurWritePos;


        public wmPacket(wmPacket source) 
        {
            m_pPacketHeader = new byte[SPacketHeader.PACKET_HEADER_SIZE];
            Array.Copy(source.GetPacketHeader(), m_pPacketHeader, SPacketHeader.PACKET_HEADER_SIZE);
            m_pData = new byte[SDataHeader.MAX_PACKET_DATA_SIZE];
            Array.Copy(source.GetData(), m_pData, SDataHeader.MAX_PACKET_DATA_SIZE);

            m_wHeaderLen = source.m_wHeaderLen;
            m_wDataLen = source.m_wDataLen;
            m_wCurReadPos = source.m_wCurReadPos;
            m_wCurWritePos = source.m_wCurWritePos;
            m_wRecvLen -= m_wRecvLen;
        }

        public wmPacket()
        {
            m_pPacketHeader = new byte[SPacketHeader.PACKET_HEADER_SIZE];
            Array.Clear(m_pPacketHeader, 0, m_pPacketHeader.Length);// 初始化
            
            m_pData = new byte[SDataHeader.MAX_PACKET_DATA_SIZE];
            Array.Clear(m_pData, 0, m_pData.Length);// 初始化

            //m_wHeaderLen = (ushort)SPacketHeader.PACKET_HEADER_SIZE;
            //m_wDataLen = sizeof(ushort) * 2;
            m_wHeaderLen -= m_wHeaderLen;
            m_wDataLen -= m_wDataLen;
            m_wRecvLen -= m_wRecvLen;

            //m_pPacketHdr.m_pDataHeader = new SDataHeader();
            //m_pPacketHdr.m_pDataHeader.m_pData = new byte[SDataHeader.MAX_PACKET_DATA_SIZE];

            m_wCurReadPos = 4;
            m_wCurWritePos = 4;
        }
        /** 获取包头 **/
        public byte[] GetPacketHeader()
        {
            return m_pPacketHeader;
        }
        /// 当前包头的长度
        public ushort GetCurPacketHeaderSize()
        {
            return m_wHeaderLen;
        }
        
        /** 获取数据头和数据 **/
        public byte[] GetData()
        {
            return m_pData;
        }
        /// 当前数据的长度
        public ushort GetCurDataSize()
        {
            return m_wDataLen;
        }
        /// 设置当前数据长度，因压缩导致数据长度变化
        public void SetCurDataSize(ushort value)
        {
            m_wDataLen = value;
        }

        /** 获取下一次需要接收的数据长度 **/
        public ushort GetNextRecvSize()
        {
            return (ushort)(GetPacketSize() - SPacketHeader.MIN_PACKET_SIZE - m_wDataLen);
        }
        /** 获取整个包的实际长度 **/
        public ushort GetPacketSize()
        {
            return (ushort)(BitConverter.ToUInt16(m_pPacketHeader, 2) & 0x1fff);
        }
        /** 更新整个包的实际长度 **/
        private void SetPacketSize()
        {
            ushort wPacketSize = (ushort)(SPacketHeader.PACKET_HEADER_SIZE + m_wDataLen);

            ushort wCurValue = BitConverter.ToUInt16(m_pPacketHeader, 2);
            wCurValue &= 0xe000;
            wCurValue |= wPacketSize;
            m_pPacketHeader[2] = (byte)wCurValue;
            m_pPacketHeader[3] = (byte)(wCurValue >> 8);
        }
        /** 验证包头是否正确 **/
        public bool IsValid()
        {
            if (GetPacketSize() < SPacketHeader.MIN_PACKET_SIZE)
            {
                return false;
            }

            if (GetPacketSize() >= SPacketHeader.MAX_PACKET_SIZE)
            {
                return false;
            }

            ushort wTmp = (ushort)(GetPacketSize() ^ GetDataSize());
            return (wTmp == GetVerCode());
        }
        /** 解密和解压缩数据包 **/
        public void Parse(CrypTea pCryp)
        {
            if (IsCryp())
            {
				pCryp.Decryption(m_pData, (ushort)(GetPacketSize() - SPacketHeader.PACKET_HEADER_SIZE));
            }

            //if (IsZip())
            //{
            //    m_pData.uncompress();
            //}
        }
        /** 设置包验证码 **/
        private void SetVerCode()
        {
            ushort wTmp = (ushort)(GetPacketSize() ^ GetDataSize());
            m_pPacketHeader[0] = (byte)wTmp;
            m_pPacketHeader[1] = (byte)(wTmp >> 8);
        }
        /** 获取包验证码 **/
        public byte GetVerCode()
        {
            return (byte)(BitConverter.ToUInt16(m_pPacketHeader, 0));
        }

        private bool IsCryp()
        {
            return (0 != (m_pPacketHeader[3] & 0x20));
        }

        private bool IsZip()
        {
            return (0 != (m_pPacketHeader[3] & 0x40));
        }
        /** 设置加密有效 **/
        private void SetCryp(bool b)
        {
            if (b)
            {
                m_pPacketHeader[3] |= 0x20;
            }
            else
            {
                m_pPacketHeader[3] &= 0xdf;
            }
        }
        private void SetZip(bool b)
        {
            if (b)
            {
                m_pPacketHeader[3] |= 0x40;
            }
            else
            {
                m_pPacketHeader[3] &= 0xbf;
            }
        }
        
        /** 获取整个数据的实际长度 **/
        public ushort GetDataSize()
        {
            ushort wSize = BitConverter.ToUInt16(m_pPacketHeader, 4);
            wSize &= 0x1fff;
            return wSize;
        }

        /** 更新包头中的数据原文长度 **/
        private void SetDataSize()
        {
            ushort wDataSize = m_wDataLen;
            wDataSize &= 0x1fff;

            m_pPacketHeader[4] = (byte)wDataSize;
            m_pPacketHeader[5] = (byte)(wDataSize >> 8);
        }
        /** 设置交易码 **/
        public void SetOpcode(ushort wOpcode)
        {
            /// 重新初始化读写位置,
            m_wCurReadPos = 4;
            m_wCurWritePos = 4;
            /// 重新初始化包头长度、数据长度
            m_wHeaderLen = (ushort)SPacketHeader.PACKET_HEADER_SIZE;
            m_wDataLen = sizeof(ushort) * 2;

            m_pData[0] = (byte)wOpcode;
            m_pData[1] = (byte)(wOpcode >> 8);
        }
        /** 获取交易码 **/
        public ushort GetOpcode()
        {
            return BitConverter.ToUInt16(m_pData, 0);
        }
        /** 设置返回码 **/
        public void SetRetCode(ushort wRetCode)
        {
            m_pData[2] = (byte)wRetCode;
            m_pData[3] = (byte)(wRetCode >> 8);
        }
        /** 获取返回码 **/
        public ushort GetRetCode()
        {
            return BitConverter.ToUInt16(m_pData, 2);
        }

        /** 生成完整的数据包: 加密 压缩 计算包头 **/
        public void Build(CrypTea pCryp)
        {
            m_pPacketHeader[0] = 0;
            m_pPacketHeader[1] = 0;
            m_pPacketHeader[2] = 0;
            m_pPacketHeader[3] = 0;
            m_pPacketHeader[4] = 0;
            m_pPacketHeader[5] = 0;

            SetDataSize();

            /// 进行压缩
            if (m_wCurWritePos >= 96)
            {
                // 当前有效数据
                byte[] pValidData = new byte[GetCurDataSize()];
                Array.Copy(m_pData, 0, pValidData, 0, pValidData.Length);
                // 压缩后的数据
                //byte[] pData = Compress.Compression(pValidData);
                byte[] pData;
                Compress.CompressData(pValidData, out pData);
                // 将压缩后的数据拷贝到数据数组中
                Array.Copy(pData, 0, m_pData, 0, pData.Length);
                // 重新设置写位置
                SetCurDataSize((ushort)pData.Length);
                // 设置整个包实际的长度
                SetPacketSize();
                SetZip(true);
            }
            else
            {
                SetZip(false);
            }
            /// 进行加密
            byte[] pKey = pCryp.GetKey();
            if (null == pKey)
            {
                return;
            }
            // 按8字节对齐
            uint wTemp = (uint)(8 - (GetCurDataSize() & 7)); //
            if (8 != wTemp)
            {
                WriteBytes(ref pKey, (uint)0, (uint)wTemp);
            }
            pCryp.Encryption(m_pData, GetDataSize());
            SetCryp(true);

            SetPacketSize();
            SetVerCode();
        }
        /** 将指定字节数组 bytes（起始偏移量为 offset）中包含 length 个字节的字节序列写入字节流。 **/
        public void WriteBytes(ref byte[] byteArray, uint offset, uint length)
        {
            if (offset > byteArray.Length)
            {
                offset = (uint)byteArray.Length; 
            }
            if (length > byteArray.Length - offset)
            {
                length = (uint)(byteArray.Length - offset);
            }
            Array.Copy(byteArray, 0, m_pData, m_wCurWritePos, length);

            // 移动写位置
            m_wCurWritePos += (ushort)length;

            // 数据的当前长度
            m_wDataLen += (ushort)length;

        }

        /** 填入uint8,一字节 **/
        public void WriteUint8(byte value)
        {
            m_pData[m_wCurWritePos] = value;

            /// 写入位置移动
            m_wCurWritePos += sizeof(byte);

            // 数据的当前长度
            m_wDataLen += sizeof(byte);
        }
        /** 读取uint8,一字节 **/
        public byte ReadUint8()
        {
            byte value = (byte)m_pData[m_wCurReadPos];
            /// 读取位置移动
            m_wCurReadPos += sizeof(byte);

            return value;
        }

        /** 填入bool值,一字节 **/
        public void WriteBoolean(bool value)
        {
            m_pData[m_wCurWritePos] = (byte)(value ? 1 : 0);

            /// 写入位置移动
            m_wCurWritePos += sizeof(bool);
            // 数据的当前长度
            m_wDataLen += sizeof(byte);
        }
        /** 读取bool值,一字节 **/
        public bool ReadBoolean()
        {
            bool value = m_pData[m_wCurReadPos] == 1;
            /// 读取位置移动
            m_wCurReadPos += sizeof(bool);

            return value;
        }

        /** 填入uint16,两字节 **/
        public void WriteUint16(ushort value)
        {
            m_pData[m_wCurWritePos] = (byte)value;
            m_pData[m_wCurWritePos + 1] = (byte)(value >> 8);
            /// 写入位置移动
            m_wCurWritePos += sizeof(ushort);
            // 数据的当前长度
            m_wDataLen += sizeof(ushort);
        }
        /** 读取uint16,两字节 **/
        public ushort ReadUint16()
        {
            ushort temp = BitConverter.ToUInt16(m_pData, m_wCurReadPos);
            /// 读取位置移动
            m_wCurReadPos += sizeof(ushort);

            return temp;
        }

        /** 填入uint32,四字节 **/
        public void WriteUint32(uint value)
        {
            m_pData[m_wCurWritePos] = (byte)value;
            m_pData[m_wCurWritePos + 1] = (byte)(value >> 8);
            m_pData[m_wCurWritePos + 2] = (byte)(value >> 16);
            m_pData[m_wCurWritePos + 3] = (byte)(value >> 24);
            /// 写入位置移动
            m_wCurWritePos += sizeof(uint);
            // 数据的当前长度
            m_wDataLen += sizeof(uint);
        }
        /** 读取uint32,四字节 **/
        public uint ReadUint32()
        {
            uint temp = BitConverter.ToUInt32(m_pData, m_wCurReadPos);
            /// 读取位置移动
            m_wCurReadPos += sizeof(uint);

            return temp;
        }

        /** 填入Float **/
        public void WriteFloat(float value)
        {
            // 将Float转为byte数组
            byte[] byteArray = BitConverter.GetBytes(value);
            Array.Copy(byteArray, 0, m_pData, m_wCurWritePos, byteArray.Length);

            /// 写入位置移动
            m_wCurWritePos += (ushort)byteArray.Length;
            // 数据的当前长度
            m_wDataLen += (ushort)byteArray.Length;
        }
        /** 读取Float **/
        public float ReadFloat()
        {
            float temp = BitConverter.ToSingle(m_pData, m_wCurReadPos);
            /// 读取位置移动
            m_wCurReadPos += sizeof(float);

            return temp;
        }

        /** 填入Float **/
        public void WriteDouble(double value)
        {
            // 将Float转为byte数组
            byte[] byteArray = BitConverter.GetBytes(value);
            Array.Copy(byteArray, 0, m_pData, m_wCurWritePos, byteArray.Length);

            /// 写入位置移动
            m_wCurWritePos += (ushort)byteArray.Length;
            // 数据的当前长度
            m_wDataLen += (ushort)byteArray.Length;
        }
        /** 读取Float **/
        public double ReadDouble()
        {
            double temp = BitConverter.ToDouble(m_pData, m_wCurReadPos);
            /// 读取位置移动
            m_wCurReadPos += sizeof(double);

            return temp;
        }

//         /** 填入uint64,八字节 **/
//         public void WriteUint64(ulong value)
//         {
//             for (byte i = 0; i < 8; i++ )
//             {
//                 m_pData[m_wCurWritePos + i] = (byte)(value >> (8*i));
//             }
//             /// 写入位置移动
//             m_wCurWritePos += sizeof(ulong);
//         }
//         /** 读取uint64,八字节 **/
//         public ulong ReadUint64()
//         {
//             ulong temp = BitConverter.ToUInt64(m_pData, m_wCurReadPos);
//             /// 读取位置移动
//             m_wCurReadPos += sizeof(ulong);
// 
//             return temp;
//         }

        /** 
         * 填入字符串,动态字节数
         * @param str:传入字符串的引用
         * PS:压包前有两字节数据，表示字符串长度 **/
        public void WriteString(ref string str)
        {
            // 将字符串转换为byte数组
            byte[] byteArray = Encoding.UTF8.GetBytes(str);

            // 压入两字节字符串长度
            ushort wStrLen = (ushort)(byteArray.Length);
            m_pData[m_wCurWritePos] = (byte)wStrLen;
            m_pData[m_wCurWritePos + 1] = (byte)(wStrLen >> 8);
            m_wCurWritePos += sizeof(ushort);//移动写入位置

            Array.Copy(byteArray, 0, m_pData, m_wCurWritePos, wStrLen);

            // 移动写位置
            m_wCurWritePos += wStrLen;

            // 数据的当前长度
            m_wDataLen += sizeof(ushort);
            m_wDataLen += wStrLen;
        }
        /** 
         * 读取字符串,动态字节数 
         * @param str:传入字符串的引用
         * @param wStrSize:字符串长度
         * **/
        public void ReadString(out string str)
        {
            // 在包中读取字符串长度
            ushort wStrLen = BitConverter.ToUInt16(m_pData, m_wCurReadPos);
            m_wCurReadPos += sizeof(ushort);//移动读取位置

            // 临时字节数组
            byte[] byteArray = new byte[wStrLen];
            Array.Copy(m_pData, m_wCurReadPos, byteArray, 0, wStrLen);//拷贝
            // 将临时字节数组转换为字符串并做返回
            str = System.Text.Encoding.UTF8.GetString(byteArray);
            m_wCurReadPos += (ushort)byteArray.Length;//移动读取位置
        }
    }
}
