using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

// Reference add "CANProPlus_NET.dll"
using CANProPlus_NET;


namespace _2_CANConfig
{
	public class CANBaudrate
	{
		public byte presdiv;
		public byte propseg;
		public byte pseg1;
		public byte pseg2;

		public CANBaudrate(byte presdiv, byte propseg, byte pseg1, byte pseg2)
		{
			this.presdiv = presdiv;
			this.propseg = propseg;
			this.pseg1 = pseg1;
			this.pseg2 = pseg2;
		}
	}

	class Program
	{
		static void Main(string[] args)
		{
			CANProPlus CAN = new CANProPlus();

			// CAN baudrate preset
			CANBaudrate[] CANBaudratePreset = new CANBaudrate[11]
			{
				// {presdiv, propseg, pseg1, pseg2}
				new CANBaudrate(3, 3, 3, 2),		// 1000 Kbps
				new CANBaudrate(4, 3, 3, 2),		// 800 Kbps
				new CANBaudrate(7, 3, 3, 2),        // 500 Kbps
				new CANBaudrate(15, 3, 3, 2),		// 250 Kbps
				new CANBaudrate(19, 3, 3, 2),		// 200 Kbps
				new CANBaudrate(31, 3, 3, 2),		// 125 Kbps
				new CANBaudrate(39, 3, 3, 2),		// 100 Kbps
				new CANBaudrate(47, 3, 3, 2),		// 83.3 Kbps
				new CANBaudrate(79, 3, 3, 2),		// 50 Kbps
				new CANBaudrate(119, 3, 3, 2),		// 33.3 Kbps
				new CANBaudrate(199, 3, 3, 2)       // 20 Kbps
			};

			// connect
			if (ConnectDevice(CAN) == false)
			{
				// fail
				return;
			}
			Console.WriteLine("connect device");

			// get device configuration
			Console.WriteLine("--- Get Configuration ---");
			CAN_CONFIGURATION config = new CAN_CONFIGURATION();
			CAN_RESULT result = CAN.GetConfiguration(ref config);
			if (result == CAN_RESULT.OK)
			{
				Console.WriteLine("PRESDIV : {0}", config.PRESDIV);
				Console.WriteLine("PROPSEG : {0}", config.PROPSEG);
				Console.WriteLine("PSEG1 : {0}", config.PSEG1);
				Console.WriteLine("PSEG2 : {0}", config.PSEG2);
				Console.WriteLine("Listen(NoACK) mode : {0}", (config.EnableListenMode ? "Enable" : "Disable"));
			}
			else if (result == CAN_RESULT.INVALID_PARAMETER)
			{
				Console.WriteLine("GetConfiguration() error : Invalid Parameter");
			}
			else if (result == CAN_RESULT.TIMEOUT)
			{
				Console.WriteLine("GetConfiguration() error : Timeout");
			}

			// calculate CAN baudrate
			double bps;
			bps = ((48000000.0 / ((double)(config.PRESDIV + 1) * (double)(config.PROPSEG + config.PSEG1 + config.PSEG2 + 4))) / 1000.0);
			Console.WriteLine("CAN Speed : {0} Kbps", bps.ToString("F1"));

			// change device configuration
			Console.WriteLine();
			Console.WriteLine("--- Set Configuration ---");
			const int SELECT = 2;
			config.PRESDIV = CANBaudratePreset[SELECT].presdiv;
			config.PROPSEG = CANBaudratePreset[SELECT].propseg;
			config.PSEG1 = CANBaudratePreset[SELECT].pseg1;
			config.PSEG2 = CANBaudratePreset[SELECT].pseg2;
			config.EnableListenMode = false;
			result = CAN.SetConfiguration(ref config);
			if (result == CAN_RESULT.OK)
			{
				Console.WriteLine("Changed CAN baudrate : 500 Kbps");
			}
			else if (result == CAN_RESULT.INVALID_PARAMETER)
			{
				Console.WriteLine("SetConfiguration() error : Invalid Parameter");
			}
			else if (result == CAN_RESULT.TIMEOUT)
			{
				Console.WriteLine("SetConfiguration() error : Timeout");
			}
			else if (result == CAN_RESULT.FAIL)
			{
				Console.WriteLine("SetConfiguration() error : Failed set configuration");
			}

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
