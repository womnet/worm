package worm
{
	import flash.errors.IOError;
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
	import flash.events.IOErrorEvent;
	import flash.events.ProgressEvent;
	import flash.net.Socket;
	import flash.utils.ByteArray;
	
	public class CClientPipe extends EventDispatcher
	{
		/** 连接到服务器的socket **/
		private var m_pSocket:Socket = null;
		/** 包缓存 **/
		private var m_pPacketBuf:CPacket;
		/** 加解密模块 **/
		private var m_pCryp:CCrypTea;
		private var m_pCallback:EventDispatcher;
		
		public function CClientPipe(pCallback:EventDispatcher)
		{
			m_pCallback = pCallback;
			m_pCryp = new CCrypTea("[/28!~&0sM1-)G&z");
			m_pPacketBuf = new CPacket();
		}
		
		public function ConnectServer(strIp:String, wPort:uint):Boolean
		{
			try
			{
				m_pSocket = new Socket(strIp, wPort);
				m_pSocket.endian = "littleEndian";
				m_pSocket.addEventListener(Event.CONNECT, OnConnect);
				m_pSocket.addEventListener(Event.CLOSE, OnClose);
				return true;
			}
			catch(errObject:IOErrorEvent)
			{
				trace("错误信息:" + errObject.text);	
			}
			finally
			{
				//trace("虽然catch里已经有return方法,但位于return方法后面的finally里的代码依然会被执行.其实无论return方法是在try里,还是catch里,finally里的代码始终会被执行");
				return false;
			}
		}
		public function Close():void
		{
			try
			{
				if (null != m_pSocket && m_pSocket.connected)
				{
					m_pSocket.close();
				}
			}
			catch(errObject:IOErrorEvent)
			{
				trace("错误信息:" + errObject.text);	
			}
		}
		private function OnConnect(e:Event):void
		{
			m_pSocket.removeEventListener(Event.CONNECT, OnConnect);
			m_pSocket.addEventListener(ProgressEvent.SOCKET_DATA, OnSocketData);
			//连接成功，发送消息
			var pEvent:CWormEvent = new CWormEvent(CWormEvent.WORM_EVENT);
			pEvent.m_nEventId = 0;
			m_pCallback.dispatchEvent(pEvent);
		}
		
		private function OnSocketData(e:ProgressEvent):void
		{
			var pPacketHeader:ByteArray;
			var pPacketData:ByteArray;
			var nRecvSize:uint = 0;
			var pEvent:CWormEvent;
			
			for (;;)
			{
				if (!m_pSocket.connected)
				{
					return;
				}
				
				pPacketHeader = m_pPacketBuf.GetPacketHeader();
				pPacketData = m_pPacketBuf.GetData();
				
				/** 先收包头 **/
				nRecvSize = CPacket.PACKET_HEADER_SIZE - pPacketHeader.length;
				if (0 != nRecvSize)
				{
					if (m_pSocket.bytesAvailable < nRecvSize)
					{
						m_pSocket.readBytes(pPacketHeader, pPacketHeader.length, m_pSocket.bytesAvailable);
						return;
					}
					
					m_pSocket.readBytes(pPacketHeader, pPacketHeader.length, nRecvSize);
					// 包头OK了, 校验包头
					if (!m_pPacketBuf.IsValid())
					{
						/// 包异常
						pPacketHeader.clear();
						return;
					}
					
					if (0 == m_pSocket.bytesAvailable)
					{
						return;
					}
				}
				/// 包数据
				nRecvSize = m_pPacketBuf.GetPacketSize() - CPacket.PACKET_HEADER_SIZE - pPacketData.length;
				if (m_pSocket.bytesAvailable < nRecvSize)
				{
					m_pSocket.readBytes(pPacketData, pPacketData.length, m_pSocket.bytesAvailable);
					return;
				}
				
				m_pSocket.readBytes(pPacketData, pPacketData.length, nRecvSize);
				/// 一个完整的包
				m_pPacketBuf.Parse(m_pCryp);
				pEvent = new CWormEvent(CWormEvent.WORM_EVENT);
				pEvent.m_nEventId = 1;
				pEvent.m_pPacket = m_pPacketBuf;
				m_pCallback.dispatchEvent(pEvent);
				
				m_pPacketBuf = new CPacket();
				if (!m_pSocket.connected)
				{
					return;
				}
				if (0 == m_pSocket.bytesAvailable)
				{
					return;
				}
			}
		}
		
		private function OnClose(e:Event):void
		{
			var pEvent:CWormEvent = new CWormEvent(CWormEvent.WORM_EVENT);
			pEvent.m_nEventId = 2;
			m_pCallback.dispatchEvent(pEvent);
		}
		
		public function Send(oPacket:CPacket):void
		{
			if (null == m_pSocket)
			{
				return;
			}
			
			var pEvent:CWormEvent = new CWormEvent(CWormEvent.WORM_EVENT);
			pEvent.m_nEventId = 2;
			
			try
			{
				oPacket.Build(m_pCryp);
				m_pSocket.writeBytes(oPacket.GetPacketHeader());
				m_pSocket.writeBytes(oPacket.GetData());
				m_pSocket.flush();
				
			}
			catch(e:IOError)
			{
				m_pCallback.dispatchEvent(pEvent);
			}
			catch(e:SecurityError)
			{
				m_pCallback.dispatchEvent(pEvent);
			}
		}
		/** 设置密钥 **/
		public function SetKey(pKey:ByteArray):void
		{
			m_pCryp.SetKey(pKey);
		}
	}
}