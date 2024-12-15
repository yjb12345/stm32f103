
#include <stdio.h>
#include <Windows.h>

// include CANPro Library
#ifdef _X64
	#pragma comment(lib, "..\\Lib\\CANProPlus_64.lib")
#else
	#pragma comment(lib, "..\\Lib\\CANProPlus.lib")
#endif
#include "..\\Lib\\CANProPlus.h"



int main()
{
	// create CANPro handle
	CANProPlus *CAN = CANProPlus::CreateInstance();
	if (CAN == NULL)
	{
		printf("Failed load library");
		return 1;
	}

	// get library version
	unsigned short lib_version;
	CAN->GetLibraryVersion(&lib_version);
	printf("Library version : v%X.%02X\n", ((lib_version >> 8) & 0xFF), (lib_version & 0xFF));

	int device_count = 0;
	CAN_DEVICE_INFO *device_list = CAN->GetDeviceList(&device_count);

	printf("Device count : %d\n\n", device_count);

	if (device_count > 0 && device_list != NULL)
	{
		// print device list
		for (int i = 0; i < device_count; i++)
		{
			printf("[ Device - %d ]\n", i);

			printf("Description : ");
			if (device_list[i].device_type == DEVICE_TYPE_CANPROH)
				printf("CANProH");
			else if (device_list[i].device_type == DEVICE_TYPE_CANPROV3)
				printf("CANProV3");
			else
				printf("Unknown");
			printf("\n");
			
			printf("Serial : %s\n\n", device_list[i].serial);
		}

		// select device-0
		int select_device_index = 0;
		
		// connection device-0
		if (CAN->Connect(device_list[select_device_index].serial) == CAN_OK)
		{
			// success connection !
			printf("Success device-0 connection\n");

			// get device version
			unsigned short device_version;
			CAN->GetVersion(&device_version);
			printf("Device version : v%d.%02d\n", ((device_version >> 8) & 0xFF), (device_version & 0xFF));

			// disconnect device
			CAN->Disconnect();
			printf("Disconnect\n");
		}
		else
		{
			// fail
			printf("Failed device-0 connection\n");
		}

		// free memory
		CAN->Free(device_list);
	}

	// delete handle
	CANProPlus::DestroyInstance(CAN);

	return 0;
}