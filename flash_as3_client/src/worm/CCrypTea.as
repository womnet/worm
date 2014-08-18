package worm
{
	import flash.utils.ByteArray;
	
	public class CCrypTea
	{
		private var m_pKey:ByteArray;
		
		public function CCrypTea(strKey:String)
		{
			m_pKey = new ByteArray();
			m_pKey.writeMultiByte(strKey, "us-ascii");
		}
		/** 设置密钥 **/
		public function SetKey(pKey:ByteArray):void
		{
			m_pKey.clear();
			m_pKey.writeBytes(pKey, 0, 16);
		}
		
		/** 加密数据 **/
		public function Encryption(pData:ByteArray):void
		{
			var nSum:uint = 0;
			var y:int = 0;
			var z:int = (8 - (pData.length&7));
			if (8 != z)
			{
				pData.writeBytes(m_pKey, 0, z);
			}
			
			var j:uint = 0;
			var i:uint = 0;
			for (; i<pData.length>>2; i+=2)
			{				
				pData.position = i << 2;
				y = pData.readInt();
				pData.position = (i+1) << 2;
				z = pData.readInt();
				
				for (j-=j; j<16; j++)
				{   
					nSum += 0x9e3779b9;
					
					pData.position = i << 2;;
					y += ((z<<4)+m_pKey[0]) ^ (z+nSum) ^ ((z>>5)+m_pKey[1]);
					pData.writeUnsignedInt(y);
					
					pData.position = (i+1) << 2;
					z += ((y<<4)+m_pKey[2]) ^ (y+nSum) ^ ((y>>5)+m_pKey[3]);
					pData.writeUnsignedInt(z);
					
					//trace("y: "+y + "  z:  " + z);
				}
				nSum -= nSum;
			}
		}

		/** 解密数据 **/
		public function Decryption(pData:ByteArray):Boolean
		{		
			if (0 != (pData.length&7))
			{
				return false;
			}
			
			var y:int = 0;
			var z:int = 0;
			var nSum:int = 0x9e3779b9 << 4;
			
			var j:uint = 0;
			var i:uint = 0;
			for (i=0; i<(pData.length>>2); i+=2)
			{
				pData.position = i << 2;
				y = pData.readInt();
				pData.position = (i+1) << 2;
				z = pData.readInt();
				
				for (j=0; j<16; ++j)
				{
					pData.position = (i+1) << 2;
					z -= ((y<<4)+m_pKey[2]) ^ (y+nSum) ^ ((y>>5)+m_pKey[3]);
					pData.writeInt(z);
					
					pData.position = i << 2;
					y -= ((z<<4)+m_pKey[0]) ^ (z+nSum) ^ ((z>>5)+m_pKey[1]);
					pData.writeInt(y);
					
					nSum -= 0x9e3779b9;
				}
				nSum = 0x9e3779b9 << 4;
			}
			
			return true;
		}
	}
}