#pragma once

#ifdef DLL_EXPORTS
#define DLL_IMP_API __declspec(dllexport)
#else
#define DLL_IMP_API __declspec(dllimport)
#endif // DLL_EXPORTS


#define BufferSize 100
#define Buffers 10

#define OPNAME_SZ 30
#define MSG_TEXT_SZ 75
typedef struct _MSG{
	unsigned int msgNum;
	unsigned int playerid;
	TCHAR szMessage[MSG_TEXT_SZ];

} SHARED_MSG;

#define MAX_MSGS 20
typedef struct _MSG_MATRIX {
	SHARED_MSG * shared_msgs[MAX_MSGS];
	int posRead, posWrite;
} MSG_MATRIX;

TCHAR szMSGName[] = TEXT ("fmMsgSpace");

#define MSG_BUFF_SZ sizeof(SHARED_MSG)
#define MSG_MUTEX_NAME TEXT("rMsgMutex")


typedef struct _MSG_DATA {
	HANDLE hMapFile;
	MSG_MATRIX * msgs;

	int continueThread;
	TCHAR name[OPNAME_SZ];
	HANDLE hRWMutex;

} MSG_DATA;

#define GAME_DATA_SZ sizeof(GAME_DATA)

typedef struct _GAME_DATA {
	TCHAR szOP[OPNAME_SZ];
} GAME_DATA;

TCHAR szDataName[] = TEXT("fmDataSpace");

typedef struct _DATA {
	HANDLE hMapFile;
	GAME_DATA * shared;
	int continueThread;
	TCHAR name[OPNAME_SZ];
	HANDLE hMutex;

} DATA;

extern DLL_IMP_API int nDLL;

DLL_IMP_API BOOL initMSGMemAndSync (MSG_DATA * msgdata);
DLL_IMP_API BOOL initGameMemAndSync (DATA * data);
DLL_IMP_API void writeMSG (MSG_DATA * msgdata, TCHAR * msgtext);
DLL_IMP_API void readMSG (MSG_DATA * msgdata, TCHAR * msgtext);
DLL_IMP_API void writeGame (DATA * data, GAME_DATA * gamedata);
DLL_IMP_API void readGame (DATA *data, GAME_DATA * gamedata);
