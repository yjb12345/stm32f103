
#include <stdio.h>


// include CANPro Library
#ifdef _X64
	#pragma comment(lib, "..\\Lib\\CANProPlus_64.lib")
#else
	#pragma comment(lib, "..\\Lib\\CANProPlus.lib")
#endif
#include "..\\Lib\\CANProPlus.h"


typedef struct
{
	unsigned char presdiv;
	unsigned char propseg;
	unsigned char pseg1;
	unsigned char pseg2;
} CAN_SPEED_PROPERTY;

#define CAN_BAUDRATE_PRESET_NUM		11
CAN_SPEED_PROPERTY CANBaudratePreset[CAN_BAUDRATE_PRESET_NUM] =
{
	// {presdiv, propseg, pseg1, pseg2}
	{3, 3, 3, 2},		// 1000 Kbps
	{4, 3, 3, 2},		// 800 Kbps
	{7, 3, 3, 2},		// 500 Kbps
	{15, 3, 3, 2},		// 250 Kbps
	{19, 3, 3, 2},		// 200 Kbps
	{31, 3, 3, 2},		// 125 Kbps
	{39, 3, 3, 2},		// 100 Kbps
	{47, 3, 3, 2},		// 83.3 Kbps
	{79, 3, 3, 2},		// 50 Kbps
	{119, 3, 3, 2},		// 33.3 Kbps
	{199, 3, 3, 2},		// 20 Kbps
};


int ConnectDevice(CANProPlus *CAN);
void DisconnectDevice(CANProPlus *CAN);


int main()
{
	int result;

	// create CANPro handle
	CANProPlus *CAN = CANProPlus::CreateInstance();
	if (CAN == NULL)
	{
		printf("Failed load library");
		return 1;
	}

	// connect
	if (ConnectDevice(CAN) != 0)
	{
		// fail
		return 1;
	}
	printf("connect device\n");


	// get device configuration
	printf("--- Get Configuration ---\n");
	CAN_CONFIGURATION config;
	result = CAN->GetConfiguration(&config);
	if (result == CAN_OK)
	{
		printf("PRESDIV : %d\n", config.presdiv);
		printf("PROPSEG : %d\n", config.propseg);
		printf("PSEG1 : %d\n", config.pseg1);
		printf("PSEG2 : %d\n", config.pseg2);
		printf("Listen(NoACK) mode : %s\n", (config.listen_mode ? "Enable" : "Disable"));
		if (config.device_type == DEVICE_TYPE_CANPROV3)
		{
			printf("CAN Mode : ");
			if (config.can_mode == CAN_MODE_HCAN)
				printf("Hi-CAN");
			else if (config.can_mode == CAN_MODE_LCAN)
				printf("Lo-CAN");
			else if (config.can_mode == CAN_MODE_SCAN)
				printf("SingleWire-CAN");
			printf("\n");
		}
	}
	else if (result == CAN_INVALID_PARAMETER)
	{
		printf("GetConfiguration() error : Invalid Parameter\n");
	}
	else if (result == CAN_TIMEOUT)
	{
		printf("GetConfiguration() error : Timeout\n");
	}
	

	// calculate CAN baudrate
	double bps;
	bps = ((48000000.0 / ((double)(config.presdiv + 1) * (double)(config.propseg + config.pseg1 + config.pseg2 + 4))) / 1000.0);
	printf("CAN Speed : %.1f Kbps\n\n", bps);

	// change device configuration
	printf("--- Set Configuration ---\n");
	const int SELECT = 2;	// select 500 Kbps
	config.presdiv = CANBaudratePreset[SELECT].presdiv;
	config.propseg = CANBaudratePreset[SELECT].propseg;
	config.pseg1 = CANBaudratePreset[SELECT].pseg1;
	config.pseg2 = CANBaudratePreset[SELECT].pseg2;
	config.listen_mode = 0;	// disable listen mode
	config.can_mode = CAN_MODE_HCAN;
	result = CAN->SetConfiguration(&config);
	if (result == CAN_OK)
	{
		printf("Changed CAN baudrate : 500 Kbps\n");
	}
	else if (result == CAN_INVALID_PARAMETER)
	{
		printf("SetConfiguration() error : Invalid Parameter\n");
	}
	else if (result == CAN_TIMEOUT)
	{
		printf("SetConfiguration() error : Timeout\n");
	}
	else if (result == CAN_FAIL)
	{
		printf("SetConfiguration() error : Failed set configuration\n");
	}

	// disconnect
	DisconnectDevice(CAN);

	// delete handle
	CANProPlus::DestroyInstance(CAN);

	return 0;
}


// Connect
int ConnectDevice(CANProPlus *CAN)
{
	// get device list
	int device_count = 0;
	CAN_DEVICE_INFO *device_list = CAN->GetDeviceList(&device_count);
	if (device_count == 0)
	{
		return 1;
	}

	// select device-0
	int select_device_index = 0;

	// connection
	if (CAN->Connect(device_list[select_device_index].serial) != CAN_OK)
	{
		return 1;
	}

	// free memory
	CAN->Free(device_list);

	return 0;
}


// Disconnect
void DisconnectDevice(CANProPlus *CAN)
{
	CAN->Disconnect();
}