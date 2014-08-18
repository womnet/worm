package worm
{
	import flash.utils.ByteArray;

	public class CPacket
	{
		/** 包最小长度 **/
		public static const MIN_PACKET_SIZE:uint = 10;
		/** 包头的长度 **/
		public static const PACKET_HEADER_SIZE:uint = 6;
		/** 包的最大长度: 7K **/
		public static const MAX_PACKET_SIZE:uint = 7232;

		/// 包头
		private var m_pPacketHeader:ByteArray;
		/// 数据头和数据
		private var m_pData:ByteArray;
		
		public function CPacket()
		{
			m_pPacketHeader = new ByteArray();
			m_pData = new ByteArray();
			m_pPacketHeader.clear();
			m_pData.clear();
			m_pPacketHeader.endian = "littleEndian";
			m_pData.endian = "littleEndian";
		}
		
		/** 获取包头 **/
		public function GetPacketHeader():ByteArray
		{
			return m_pPacketHeader;
		}
		/** 获取数据头和数据 **/
		public function GetData():ByteArray
		{
			return m_pData;
		}
		
		/** 获取下一次需要接收的数据长度 **/
		public function GetNextRecvSize():uint
		{
			return (GetPacketSize()-MIN_PACKET_SIZE-m_pData.length);
		}
		
		/** 验证包头是否正确 **/
		public function IsValid():Boolean
		{			
			if (GetPacketSize() < MIN_PACKET_SIZE)
			{
				return false;
			}

			if (GetPacketSize() >= MAX_PACKET_SIZE)
			{				
				return false;
			}

			var wTmp:uint = GetPacketSize() ^ GetDataSize();
			return (wTmp == GetVerCode());
		}
		/** 解密和解压缩数据包 **/
		public function Parse(pCryp:CCrypTea):void
		{
			if (IsCryp())
			{
				pCryp.Decryption(m_pData);
			}
			
			if (IsZip())
			{
				m_pData.uncompress();
			}
		}
		
		/** 获取包标志id **/
		public function GetMsgId():uint
		{
			return m_pPacketHeader[0];
		}
		/** 设置包验证码 **/
		private function SetVerCode():void
		{
			var wTmp:uint = GetPacketSize() ^ GetDataSize();			
			m_pPacketHeader.position = 0;
			m_pPacketHeader.writeShort(wTmp);
		}
		/** 获取包验证码 **/
		public function GetVerCode():uint
		{
			m_pPacketHeader.position = 0;
			return m_pPacketHeader.readUnsignedShort();
		}
		
		private function IsCryp():Boolean
		{
			return (0 != (m_pPacketHeader[3]&0x20));
		}
		
		private function IsZip():Boolean
		{
			return (0 != (m_pPacketHeader[3]&0x40));
		}
		/** 设置加密有效 **/
		private function SetCryp(b:Boolean):void
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
		private function SetZip(b:Boolean):void
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
		/** 获取整个包的实际长度 **/
		public function GetPacketSize():uint
		{
			m_pPacketHeader.position = 2;
			return (m_pPacketHeader.readUnsignedShort() & 0x1fff);
		}
		/** 更新整个包的实际长度 **/
		private function SetPacketSize():void
		{
			var nPacketSize:uint = PACKET_HEADER_SIZE + m_pData.length;
			
			m_pPacketHeader.position = 2;
			var nTmp:uint = m_pPacketHeader.readUnsignedShort() & 0xe000;
			nTmp |= nPacketSize;
			m_pPacketHeader.position = 2;
			m_pPacketHeader.writeShort(nTmp);
		}
		public function GetDataSize():uint
		{
			m_pPacketHeader.position = 4;
			var wSize:uint = m_pPacketHeader.readUnsignedShort();
			return (wSize & 0x1fff);
		}
		
		/** 更新包头中的数据原文长度 **/
		private function SetDataSize():void
		{
			var wDataSize:uint = m_pData.length & 0x1fff;
			m_pPacketHeader.position = 4;
			m_pPacketHeader.writeShort(wDataSize);
		}
		/** 设置交易码 **/
		public function SetOpcode(wOpcode:uint):void
		{
			m_pData.clear();
			m_pData.writeShort(wOpcode);
			m_pData.writeShort(0);			
		}
		/** 获取交易码 **/
		public function GetOpcode():uint
		{
			m_pData.position = 0;
			return m_pData.readUnsignedShort();
		}
		/** 设置返回码 **/
		public function SetRetCode(wRetCode:uint):void
		{
			m_pData.position = 2;
			m_pData.writeShort(wRetCode);
		}
		/** 获取返回码 **/
		public function GetRetCode():uint
		{
			m_pData.position = 2;
			return m_pData.readUnsignedShort();
		}
		
		/** 生成完整的数据包: 加密 压缩 计算包头 **/
		public function Build(pCryp:CCrypTea):void
		{
			m_pPacketHeader.clear();
			m_pPacketHeader.writeUnsignedInt(0);
			m_pPacketHeader.writeShort(0);

			SetDataSize();
		
			if (m_pData.length > 96)
			{
				m_pData.compress();
				SetZip(true);
			}
			else
			{
				SetZip(false);
			}
			
			pCryp.Encryption(m_pData);
			SetCryp(true);
			
			SetPacketSize();
			SetVerCode();
		}
	}
}
