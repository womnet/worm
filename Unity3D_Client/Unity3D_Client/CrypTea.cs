
using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;

namespace Unity3D_Client
{
    public class CrypTea
    {
        private byte[] m_pKey;

        public CrypTea(byte[] aKey)
        {
            SetKey(aKey);
        }
        /** 设置密钥 **/
        public void SetKey(byte[] aKey)
        {
            m_pKey = new byte[16];
            Array.Copy(aKey, 0, m_pKey, 0, aKey.Length);
//             for (byte i = 0; i < 16; ++i)
//             {
//                 m_pKey[i] = aKey[i];
//             }
        }
        public byte[] GetKey()
        {
            return m_pKey;
        }

        /** 加密数据 **/
        public void Encryption(byte[] pData, ushort nDataSize)
        {
            uint nSum = 0;
            int y = 0;
            int z = 0;

            int j = 0;
            int i = 0;
            for (; i < nDataSize >> 2; i += 2)
            {
                y = BitConverter.ToInt32(pData, i << 2);
                z = BitConverter.ToInt32(pData, (i + 1) << 2);

                for (j -= j; j < 16; j++)
                {
                    nSum += 0x9e3779b9;

                    y += (int)((z << 4) + m_pKey[0]) ^ (z + (int)nSum) ^ ((z >> 5) + m_pKey[1]);
                    pData[i << 2] = (byte)y;
                    pData[(i << 2) + 1] = (byte)(y >> 8);
                    pData[(i << 2) + 2] = (byte)(y >> 16);
                    pData[(i << 2) + 3] = (byte)(y >> 24);

                    z += (int)((y << 4) + m_pKey[2]) ^ (y + (int)nSum) ^ ((y >> 5) + m_pKey[3]);
                    pData[(i + 1) << 2] = (byte)z;
                    pData[((i + 1) << 2) + 1] = (byte)(z >> 8);
                    pData[((i + 1) << 2) + 2] = (byte)(z >> 16);
                    pData[((i + 1) << 2) + 3] = (byte)(z >> 24);
                }
                nSum -= nSum;
            }
        }

        /** 解密数据 **/
        public bool Decryption(byte[] pData, ushort nDataSize)
        {
            if (0 != (nDataSize & 7))
            {
                return false;
            }

            int y = 0;
            int z = 0;
            uint nSum = 0x9e3779b9 << 4;

            int j = 0;
            int i = 0;
            for (i = 0; i < (nDataSize >> 2); i += 2)
            {
                y = BitConverter.ToInt32(pData, i << 2);
                z = BitConverter.ToInt32(pData, (i + 1) << 2);

                for (j = 0; j < 16; ++j)
                {
                    z -= ((y << 4) + m_pKey[2]) ^ (y + (int)nSum) ^ ((y >> 5) + m_pKey[3]);
                    pData[(i + 1) << 2] = (byte)z;
                    pData[((i + 1) << 2) + 1] = (byte)(z >> 8);
                    pData[((i + 1) << 2) + 2] = (byte)(z >> 16);
                    pData[((i + 1) << 2) + 3] = (byte)(z >> 24);

                    y -= ((z << 4) + m_pKey[0]) ^ (z + (int)nSum) ^ ((z >> 5) + m_pKey[1]);
                    pData[i << 2] = (byte)y;
                    pData[(i << 2) + 1] = (byte)(y >> 8);
                    pData[(i << 2) + 2] = (byte)(y >> 16);
                    pData[(i << 2) + 3] = (byte)(y >> 24);

                    nSum -= 0x9e3779b9;
                }
                nSum = 0x9e3779b9 << 4;
            }

            return true;
        }
    }
}
