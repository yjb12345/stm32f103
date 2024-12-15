using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

// Reference add "CANProPlus_NET.dll"
using CANProPlus_NET;


namespace _3_SendMessage
{
	public class TX_SCHEDULE
	{
		public UInt32 ID;
		public bool Ext;
		public byte DLC;
		public byte[] Data = new byte[8];

		public int Period;
		public int Tick;

		public TX_SCHEDULE(uint ID, bool Ext, byte DLC, byte[] Data, int Period, int Tick)
		{
			this.ID = ID;
			this.Ext = Ext;
			this.DLC = DLC;
			this.Data = Data;
			this.Period = Period;
			this.Tick = Tick;
		}
	}

	class Program
	{
		static void Main(string[] args)
		{
			CANProPlus CAN = new CANProPlus();

			const int SCHEDULE_NUM = 3;
			TX_SCHEDULE[] tx_schedule = new TX_SCHEDULE[SCHEDULE_NUM]
			{
				//				ID,				Ext,		DLC,	Data,															Period,		Tick
				new TX_SCHEDULE( 0x1AD,			false,		8,		new byte[8]{ 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88},	100,		0),
				new TX_SCHEDULE( 0x1234ABCD,	true,		6,		new byte[6]{ 0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F},				20,			0),
				new TX_SCHEDULE( 0x07986485,	true,		8,		new byte[8]{ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37},	1000,		0)
			};


			// connect
			if (ConnectDevice(CAN) == false)
			{
				// fail
				return;
			}
			Console.WriteLine("connect device");

			// start CAN
			CAN.StartTrace();

			// periodic message send loop
			Console.WriteLine("start send message");
			Console.WriteLine("press any key to exit");
			while (Console.KeyAvailable == false)
			{
				// check schedule
				for (int i = 0; i < SCHEDULE_NUM; i++)
				{
					tx_schedule[i].Tick++;
					if (tx_schedule[i].Tick >= tx_schedule[i].Period)
					{
						tx_schedule[i].Tick = 0;

						// make
						CAN_MESSAGE msg = new CAN_MESSAGE();
						msg.Type = CAN_MESSAGE_TYPE.TX;
						msg.IsExtended = tx_schedule[i].Ext;
						msg.IsRemote = false;
						msg.ID = tx_schedule[i].ID;
						msg.DLC = tx_schedule[i].DLC;
						for (int j = 0; j < msg.DLC; j++)
						{
							msg.Data[j] = tx_schedule[i].Data[j];
						}

						// push tx queue
						CAN.PushTxMsg(ref msg);
					}
				}

				// flush tx queue
				CAN.StartTxMsg();

				// 1 msec delay
				System.Threading.Thread.Sleep(1);
			}

			// stop CAN
			CAN.StopTrace();

			// disconnect
			DisconnectDevice(CAN);
		}


		// Connect
		static bool ConnectDevice(CANProPlus CAN)
		{
			List<CAN_DEVICE_INFO> device_list = CAN.GetDeviceList();

			// get device count
			if (device_list.Count == 0)
			{
				return false;
			}

			// select device-0
			int index = 0;

			// connect
			if (CAN.Connect(device_list[index].DeviceSerial) != CAN_RESULT.OK)
			{
				return false;
			}

			return true;
		}


		// Disconnect
		static void DisconnectDevice(CANProPlus CAN)
		{
			CAN.Disconnect();
		}
	}
}
