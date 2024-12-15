
#pragma once

#ifdef CANPROPLUS_EXPORTS
	#define CANPROPLUS_API __declspec(dllexport)
#else
	#define CANPROPLUS_API __declspec(dllimport)
#endif


#pragma pack(push, 1)



// Device type
#define DEVICE_TYPE_CANPROH			0x05
#define DEVICE_TYPE_CANPROV3		0x08
#define DEVICE_TYPE_CANPROV4		0x0A

// CAN Mode
#define CAN_MODE_HCAN		0	// Hi-Speed CAN
#define CAN_MODE_LCAN		1	// Lo-Speed CAN
#define CAN_MODE_SCAN		2	// Single-Wire CAN


// CAN 에러 종류
typedef enum
{
	CAN_ERROR_NONE = 0,
	CAN_ERROR_BUSOFF = 1,
	CAN_ERROR_PASSIVE = 2,
	CAN_ERROR_TX_WARNING = 3,
	CAN_ERROR_RX_WARNING = 4,
	CAN_ERROR_TX_BUFF_OVERFLOW = 5,
	
	CAN_ERROR_BIT1 = 10,
	CAN_ERROR_BIT0 = 11,
	CAN_ERROR_ACK = 12,
	CAN_ERROR_CRC = 13,
	CAN_ERROR_FORM = 14,
	CAN_ERROR_STUFFING = 15,
	
} CAN_ERROR_TYPE;


#define CAN_MESSAGE_TYPE_RX			0
#define CAN_MESSAGE_TYPE_TX			1
#define CAN_MESSAGE_TYPE_ERROR		2

// CAN Message Structure (22 byte)
typedef struct
{
	union _info
	{
		unsigned char info;
		struct
		{
			unsigned char reserved	: 3;
			unsigned char hi_volt	: 1;		// 0 : Normal, 1 : High-Voltage Message(Only Single-Wire CAN)
			unsigned char extended	: 1;		// 0 : Standard, 1 : Extended
			unsigned char remote	: 1;		// 0 : Data, 1 : Remote
			unsigned char type		: 2;		// 0 : RX, 1 : TX, 2 : Error
		} BITS;
	} info;

	unsigned long long time;		// Time Stamp
	unsigned long id;				// ID
	unsigned char dlc;				// Data length count
	unsigned char data[8];			// Data
} CAN_MESSAGE;





// CANProPlus Device Configuration Structure
typedef struct
{
	// device type
	unsigned char device_type;

	// baudrate
	unsigned char presdiv;
	unsigned char propseg;
	unsigned char pseg1;
	unsigned char pseg2;

	// option
	unsigned char listen_mode;

	// CAN mode
	unsigned char can_mode;

	unsigned char reserved[29];
} CAN_CONFIGURATION;



// Device Info
typedef struct
{
	// device type
	unsigned char device_type;

	// device serial number
	char serial[16];

} CAN_DEVICE_INFO;


#pragma pack(pop)


// Function Result Enumeration
enum
{
	CAN_OK = 0,
	CAN_FAIL,
	CAN_CONNECTION_FAIL,
	CAN_TIMEOUT,
	CAN_INVALID_PARAMETER,
	CAN_DATA_NOT_EXIST,
	CAN_TX_MSG_BUFF_FULL,
};



// CANProPlus Class
class CANPROPLUS_API CANProPlus
{
public:
	static CANProPlus *CreateInstance();
	static void DestroyInstance(CANProPlus *instance);

	virtual int GetVersion(unsigned short *version) = 0;
	virtual int GetLibraryVersion(unsigned short *version) = 0;

	// old 
	virtual int GetDeviceCount() = 0;
	virtual void GetDeviceSerial(int index, char *serial) = 0;

	// new
	virtual CAN_DEVICE_INFO* GetDeviceList(int *device_count) = 0;
	virtual void Free(void *obj) = 0;

	virtual int	Connect(char *serial) = 0;
	virtual void Disconnect() = 0;
	virtual bool IsConnected() = 0;

	virtual int GetConfiguration(CAN_CONFIGURATION *config) = 0;
	virtual int SetConfiguration(CAN_CONFIGURATION *config) = 0;
	virtual int ResetDriver() = 0;

	virtual int StartTrace() = 0;
	virtual int StopTrace() = 0;
	virtual int GetTraceMsgCount() = 0;
	virtual int PopTraceMsg(CAN_MESSAGE *msg) = 0;

	virtual int PushTxMsg(CAN_MESSAGE *msg) = 0;
	virtual int StartTxMsg() = 0;
};
