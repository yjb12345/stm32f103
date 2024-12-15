using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

// Reference add "CANProPlus_NET.dll"
using CANProPlus_NET;


namespace _1_Start
{
	class Program
	{
		static void Main(string[] args)
		{
			CANProPlus CAN = new CANProPlus();

			// get library version
			UInt16 lib_version = 0;
			CAN.GetLibraryVersion(ref lib_version);
			Console.WriteLine("Library version : v{0}.{1}", 
				((lib_version >> 8) & 0xFF).ToString("X"), (lib_version & 0xFF).ToString("X02"));

			// get device list
			List<CAN_DEVICE_INFO> device_list = CAN.GetDeviceList();
			int device_count = device_list.Count;
			Console.WriteLine("Device count : {0}\n", device_count);

			// print device list
			for(int i = 0; i < device_count; i++)
			{
				Console.WriteLine("[ Device - {0} ]", i);

				Console.Write("Description : ");
				if (device_list[i].DeviceType == CAN_DEVICE_TYPE.CANPRO_H)
					Console.WriteLine("CANProH");
				else if (device_list[i].DeviceType == CAN_DEVICE_TYPE.CANPRO_V3)
					Console.WriteLine("CANProV3");
				else
					Console.WriteLine("Unknown");

				Console.WriteLine("Serial : {0}\n", device_list[i].DeviceSerial);
			}

			if (device_count > 0)
			{
				int index = 0;

				// connection device-0
				if (CAN.Connect(device_list[index].DeviceSerial) == CAN_RESULT.OK)
				{
					// success connection !
					Console.WriteLine("Success device-0 connection");

					// get device version
					UInt16 device_version = 0;
					CAN.GetVersion(ref device_version);
					Console.WriteLine("Device version : v{0}.{1}",
						((device_version >> 8) & 0xFF).ToString("X"), (device_version & 0xFF).ToString("X02"));

					// disconnect device
					CAN.Disconnect();
					Console.WriteLine("Disconnect");
				}
				else
				{
					// fail
					Console.WriteLine("Failed device 0 connection");
				}
			}
		}
	}
}
