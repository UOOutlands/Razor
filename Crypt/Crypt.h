#pragma once
#pragma pack(1)

#define DLL_VERSION "0.1.32"
#define CHECKSUM "\x30\xB9\xE0\x6A\x9E\x8B\x2B\xEC\xEB\x92\xC2\xD2\x4E\x32\xDD\xDD"

#define DLLFUNCTION __declspec(dllexport)
#define DLLVAR DLLFUNCTION

#ifdef _DEBUG
//#define LOGGING
#endif

enum IError
{
	SUCCESS,
	NO_UOWND,
	NO_TID,
	NO_HOOK,
	NO_SHAREMEM,
	LIB_DISABLED,
	NO_PATCH,
	NO_COPY,
	INVALID_PARAMS,

	UNKNOWN,
};

enum UONET_MESSAGE
{
	SEND = 1,
	RECV = 2,
	READY = 3,
	NOT_READY = 4,
	CONNECT = 5,
	DISCONNECT = 6,
	KEYDOWN = 7,
	MOUSE = 8,

	ACTIVATE = 9,
	FOCUS = 10,

	CLOSE = 11,
	STAT_BAR = 12,
	NOTO_HUE = 13,
	DLL_ERROR = 14,

	DEATH_MSG = 15,

	CALIBRATE_POS = 16,
	GET_POS = 17,

	OPEN_RPV = 18,

	SETWNDSIZE = 19,

	FINDDATA = 20,

	SMART_CPU = 21,
	NEGOTIATE = 22,
};

//#define SHARED_BUFF_SIZE 0x80000 // Client's buffers are 500k
#define SHARED_BUFF_SIZE 262144 // 250k
struct Buffer
{
	int Length;
	int Start;
	BYTE Buff[SHARED_BUFF_SIZE];
};

struct SharedMemory
{
	// these must be at the top in this order
	Buffer InRecv;
	Buffer OutRecv;
	Buffer InSend;
	Buffer OutSend;

	char TitleBar[1024];
	bool ForceDisconn;
	bool AllowDisconn;
	unsigned int TotalSend;
	unsigned int TotalRecv;
	unsigned short PacketTable[256];
	char DataPath[256];
	char DeathMsg[16];
	int Position[3];
	unsigned char CheatKey[16];
	unsigned char AuthBits[8];
	bool IsHaxed;
};

class PatchInfo
{
public:
	PatchInfo( DWORD addr, int len )
	{
		Address = addr;
		Length = len;
		Data = new char[Length];
		memcpy( Data, (const void*)Address, Length );
	}

	~PatchInfo()
	{
		delete[] Data;
	}

	DWORD Address;
	int Length;
	char *Data;
};

#define WM_PROCREADY WM_USER
#define WM_UONETEVENT WM_USER+1
#define WM_CUSTOMTITLE WM_USER+2
#define WM_UOA_MSG WM_USER+3

#ifndef WM_XBUTTONDOWN
#define WM_XBUTTONDOWN                  0x020B
#endif

extern HWND hWatchWnd;
extern HINSTANCE hInstance;
extern SharedMemory *pShared;
extern HANDLE CommMutex;

LRESULT CALLBACK UOAWndProc( HWND, UINT, WPARAM, LPARAM );
void Log( const char *format, ... );
DLLFUNCTION int InstallLibrary( HWND PostWindow, DWORD pId );
DLLFUNCTION void Shutdown( bool closeClient );
DLLFUNCTION HWND FindUOWindow();
DLLFUNCTION void *GetSharedAddress();
DLLFUNCTION int GetPacketLength( unsigned char *data, int len );
DLLFUNCTION bool IsDynLength( unsigned char packet );
DLLFUNCTION int GetUOProcId();
DLLFUNCTION DWORD InitializeLibrary( const char * );
DLLFUNCTION HANDLE GetCommMutex();


#define PACKET_TBL_STR "Got Logout OK packet!\0\0\0"
#define PACKET_TS_LEN 24

//static key1 C1 E2 1F D1 E8 D1 E9 0B C6 0B CA 35 static key2 81 F1 dynamic key 4D
#define CRYPT_KEY_STR "\xC1\xE2\x1F\xD1\xE8\xD1\xE9\x0B\xC6\x0B\xCA\x35"
#define CRYPT_KEY_LEN 12

//static key1 D1 E8 0B C6 C1 E2 1F 35 static key2 D1 E9 89 83 F0 00 42 00 8B 45 08 0B CA 81 F1 dynamic key 48
#define CRYPT_KEY_STR_3D "\xD1\xE8\x0B\xC6\xC1\xE2\x1F\x35"
#define CRYPT_KEY_3D_LEN 8
