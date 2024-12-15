using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

// Reference add "CANProPlus_NET.dll"
using CANProPlus_NET;


namespace _4_ReceiveMessage
{
	class Program
	{
		static void Main(string[] args)
		{
			CANProPlus CAN = new CANProPlus();

			// connect
			if (ConnectDevice(CAN) == false)
			{
				// fail
				return;
			}
			Console.WriteLine("connect device");

			// start CAN
			CAN.StartTrace();

			Console.WriteLine("start receive message");
			Console.WriteLine("press any key to exit\n");

			// receive message
			PrintHeader();
			while (Console.KeyAvailable == false)
			{
				int msg_cnt = CAN.GetTraceMsgCount();
				for (int i = 0; i < msg_cnt; i++)
				{
					CAN_MESSAGE msg = new CAN_MESSAGE();
					if (CAN.PopTraceMsg(ref msg) == CAN_RESULT.OK)
					{
						// print message
						PrintMessage(msg);
					}
				}

				// 100 msec delay
				System.Threading.Thread.Sleep(100);
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


		// print message header
		static void PrintHeader()
		{
			Console.WriteLine("    Time        Dir   Type   Frame   ID(Hex)   DLC         Data(Hex)");
			Console.WriteLine("-------------  -----  -----  ------  --------  ---  ------------------------");
		}


		// print CAN message
		static void PrintMessage(CAN_MESSAGE msg)
		{
			StringBuilder str = new StringBuilder();

			// time
			str.AppendFormat("{0,13:F6}  ", ((double)msg.Time / 1000000.0));

			// direct
			if (msg.Type == CAN_MESSAGE_TYPE.RX)
				str.Append("RX     ");
			else if (msg.Type == CAN_MESSAGE_TYPE.TX)
				str.Append("TX     ");
			else if (msg.Type == CAN_MESSAGE_TYPE.ERROR)
				str.Append("Error  ");

			// error?
			if (msg.Type == CAN_MESSAGE_TYPE.ERROR)
			{
				str.Append("[ ");

				// data[0] is error type
				CAN_ERROR error_type = (CAN_ERROR)msg.Data[0];
				if (error_type == CAN_ERROR.BUSOFF)
					str.Append("Busoff");
				else if (error_type == CAN_ERROR.PASSIVE)
					str.Append("Error Passive");
				else if (error_type == CAN_ERROR.TX_WARNING)
					str.Append("TX Warning");
				else if (error_type == CAN_ERROR.RX_WARNING)
					str.Append("RX Warning");
				else if (error_type == CAN_ERROR.BIT1)
					str.Append("BIT1");
				else if (error_type == CAN_ERROR.BIT0)
					str.Append("BIT0");
				else if (error_type == CAN_ERROR.ACK)
					str.Append("ACK");
				else if (error_type == CAN_ERROR.CRC)
					str.Append("CRC");
				else if (error_type == CAN_ERROR.FORM)
					str.Append("Form");
				else if (error_type == CAN_ERROR.STUFFING)
					str.Append("Stuffing");

				str.Append(" ]");
			}
			// normal message
			else
			{
				// id type
				if (msg.IsExtended)
					str.Append("Ext    ");
				else
					str.Append("Std    ");

				// frame type
				if (msg.IsRemote)
					str.Append("Remote  ");
				else
					str.Append("Data    ");

				// ID
				str.AppendFormat("{0,-8:X}  ", msg.ID);

				// DLC
				str.AppendFormat("{0,-3}  ", msg.DLC);

				// Data
				if (msg.IsRemote == false)
				{
					for (int i = 0; i < msg.DLC; i++)
					{
						str.AppendFormat("{0,2:X2} ", msg.Data[i]);
					}
				}
			}

			Console.WriteLine(str);
		}
	}
}
