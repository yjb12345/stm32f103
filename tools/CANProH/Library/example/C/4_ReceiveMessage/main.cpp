
#include <stdio.h>
#include <conio.h>
#include <windows.h>


// include CANPro Library
#ifdef _X64
#pragma comment(lib, "..\\Lib\\CANProPlus_64.lib")
#else
#pragma comment(lib, "..\\Lib\\CANProPlus.lib")
#endif
#include "..\\Lib\\CANProPlus.h"


int ConnectDevice(CANProPlus *CAN);
void DisconnectDevice(CANProPlus *CAN);
void PrintHeader();
void PrintMessage(CAN_MESSAGE *msg);

int main()
{
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

	// start CAN
	CAN->StartTrace();

	printf("start receive message\n");
	printf("press any key to exit\n\n");

	// receive message
	PrintHeader();
	while (_kbhit() == 0)
	{
		int msg_cnt = CAN->GetTraceMsgCount();
		for (int i = 0; i < msg_cnt; i++)
		{
			CAN_MESSAGE msg;
			if (CAN->PopTraceMsg(&msg) == CAN_OK)
			{
				// print message
				PrintMessage(&msg);
			}
		}

		// 100 msec delay
		Sleep(100);
	}

	// stop CAN
	CAN->StopTrace();

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


// print message header
void PrintHeader()
{
	printf("    Time        Dir   Type   Frame   ID(Hex)   DLC         Data(Hex)\n");
	printf("-------------  -----  -----  ------  --------  ---  ------------------------\n");
}


// print CAN message
void PrintMessage(CAN_MESSAGE *msg)
{
	// time
	char str_time[20];
	sprintf_s(str_time, "%.6f", ((double)msg->time / 1000000.0));
	printf("%13s  ", str_time);

	// direct
	if (msg->info.BITS.type == CAN_MESSAGE_TYPE_RX)
		printf("RX     ");
	else if (msg->info.BITS.type == CAN_MESSAGE_TYPE_TX)
		printf("TX     ");
	else if (msg->info.BITS.type == CAN_MESSAGE_TYPE_ERROR)
		printf("Error  ");

	// error?
	if (msg->info.BITS.type == CAN_MESSAGE_TYPE_ERROR)
	{
		printf("[ ");

		// data[0] is error type
		unsigned char error_type = msg->data[0];
		if (error_type == CAN_ERROR_BUSOFF)
			printf("Busoff");
		else if (error_type == CAN_ERROR_PASSIVE)
			printf("Error Passive");
		else if (error_type == CAN_ERROR_TX_WARNING)
			printf("TX Warning");
		else if (error_type == CAN_ERROR_RX_WARNING)
			printf("RX Warning");
		else if (error_type == CAN_ERROR_BIT1)
			printf("BIT1");
		else if (error_type == CAN_ERROR_BIT0)
			printf("BIT0");
		else if (error_type == CAN_ERROR_ACK)
			printf("ACK");
		else if (error_type == CAN_ERROR_CRC)
			printf("CRC");
		else if (error_type == CAN_ERROR_FORM)
			printf("Form");
		else if (error_type == CAN_ERROR_STUFFING)
			printf("Stuffing");

		printf(" ]");
	}
	// normal message
	else
	{
		// id type
		if (msg->info.BITS.extended)
			printf("Ext    ");
		else
			printf("Std    ");

		// frame type
		if (msg->info.BITS.remote)
			printf("Remote  ");
		else
			printf("Data    ");

		// ID
		printf("%-8X  ", msg->id);
		
		// DLC
		printf("%-3d  ", msg->dlc);

		// data
		if (!msg->info.BITS.remote)
		{
			for (int i = 0; i < msg->dlc; i++)
			{
				printf("%02X ", msg->data[i]);
			}
		}
	}
	
	printf("\n");
}