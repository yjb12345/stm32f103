
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


typedef struct
{
	unsigned long ID;
	unsigned char Ext;
	unsigned char DLC;
	unsigned char Data[8];

	unsigned int Period;
	unsigned int Tick;
} TX_SCHEDULE;

#define SCHEDULE_NUM	3

TX_SCHEDULE tx_schedule[SCHEDULE_NUM] = 
{// ID,				Ext,	DLC,	Data,												Period,		Tick
	{0x1AD,			0,		8,		{0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88},	100,		0},
	{0x1234ABCD,	1,		6,		{0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F},				20,			0},
	{0x07986485,	1,		8,		{0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37},	1000,		0}
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

	// start CAN
	CAN->StartTrace();

	// periodic message send loop
	printf("start send message\n");
	printf("press any key to exit\n");
	while (_kbhit() == 0)
	{
		// check schedule
		for (int i = 0; i < SCHEDULE_NUM; i++)
		{
			tx_schedule[i].Tick++;
			if (tx_schedule[i].Tick >= tx_schedule[i].Period)
			{
				tx_schedule[i].Tick = 0;

				// make message
				CAN_MESSAGE msg;
				msg.info.BITS.type = CAN_MESSAGE_TYPE_TX;
				msg.info.BITS.extended = tx_schedule[i].Ext;
				msg.info.BITS.remote = 0;	// 0 : Data Frame, 1 : Remote Frame
				msg.id = tx_schedule[i].ID;
				msg.dlc = tx_schedule[i].DLC;
				for (int j = 0; j < msg.dlc; j++)
				{
					msg.data[j] = tx_schedule[i].Data[j];
				}

				// push tx queue
				CAN->PushTxMsg(&msg);
			}
		}

		// flush tx queue
		CAN->StartTxMsg();

		// 1 msec delay
		Sleep(1);
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

