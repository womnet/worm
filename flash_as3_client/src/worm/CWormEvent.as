package worm
{
	import flash.events.Event;
	
	public class CWormEvent extends Event
	{
		/** m_nEventId 标志事件id **/
		public static const WORM_EVENT:String = "Worm_Event";
		/** 事件id:
		 * 0: 连接服务器成功
		 * 1: 收到数据包
		 * 2: 断开连接
		 **/
		public var m_nEventId:uint;
		/** 事件id为1时设置包内容 **/
		public var m_pPacket:CPacket = null;
		
		public function CWormEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
		}
	}
}