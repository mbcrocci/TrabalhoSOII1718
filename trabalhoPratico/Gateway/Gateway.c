#include "stdafx.h"

#include "../Dll/Dll.h"

DWORD WINAPI listenClientThread (void *p)
{
	MSG_DATA * msgs = (MSG_DATA *)p;
	
	// TODO: actually receive a msg and store it

	SHARED_MSG rcv;

	while (msgs->continueThread)
	{
		WaitForSingleObject (msgs->hRWMutex, INFINITE);

		writeMSG (msgs, &rcv);

		ReleaseMutex (msgs->hRWMutex);
	}

	return 0;
}

DWORD WINAPI listenServerThread (void *p)
{
	DATA * data = (DATA *)p;

	GAME_DATA gamedata;

	while (data->continueThread)
	{
		WaitForSingleObject (data->hMutex, INFINITE);
		//TODO: do something
		ReleaseMutex (data->hMutex);
	}
	return 0;
}

int _tmain (void)
{
#ifdef UNICODE
	_setmode (_fileno (stdin), _O_WTEXT);
	_setmode (_fileno (stdout), _O_WTEXT);
#endif

	MSG_DATA msgdata;
	DATA data;

	DWORD tidMSG;
	DWORD tidDATA;

	HANDLE tMSG;
	HANDLE tDATA;

	msgdata.hMapFile = OpenFileMapping (
		FILE_MAP_ALL_ACCESS,
		FALSE,
		szMSGName
	);
	if (msgdata.hMapFile == NULL)
	{
		_tprintf (TEXT ("\nMemoria para mensagens nao criada. A criar..."));
		if (!initMSGMemAndSync (&msgdata))
		{
			_tprintf (TEXT ("\nImpossivel continuar (%d).\n"), GetLastError ());
			exit (1);
		}
	}
	else
	{
		msgdata.hRWMutex = OpenMutex (SYNCHRONIZE, TRUE, MSG_MUTEX_NAME);
		if (msgdata.hRWMutex == NULL)
		{
			_tprintf (TEXT ("Impossivel abrir o mutex de msgs (%d)."), GetLastError ());
			return FALSE;
		}
	}


	data.hMapFile = OpenFileMapping (
		FILE_MAP_ALL_ACCESS,
		FALSE,
		szDataName
	);
	if (data.hMapFile == NULL)
	{
		_tprintf (TEXT ("\nMemoria para dados nao criada. A criar..."));
		if (!initGameMemAndSync (&data))
		{
			_tprintf (TEXT ("\nImpossivel continuar (%d).\n"), GetLastError ());
			exit (1);
		}
	}
	else
	{
		data.hMutex = OpenMutex (SYNCHRONIZE, TRUE, MSG_MUTEX_NAME);
		if (data.hMutex == NULL)
		{
			_tprintf (TEXT ("Impossivel abrir o mutex dos dados (%d)."), GetLastError ());
			return FALSE;
		}
	}

	msgdata.msgs = (MSG_MATRIX *)MapViewOfFile (
		msgdata.hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		MSG_BUFF_SZ
	);
	if (msgdata.msgs == NULL)
	{
		_tprintf (TEXT ("Erro na view da mem partilhada das msgs (%d).\n"), GetLastError ());
		CloseHandle (msgdata.hMapFile);
		return 1;
	}
	msgdata.msgs->posRead = 0;
	msgdata.msgs->posWrite = 0;

	data.shared = (GAME_DATA *)MapViewOfFile (
		data.hMapFile,
		FILE_READ_ACCESS,
		0,
		0,
		GAME_DATA_SZ
	);
	if (data.shared == NULL)
	{
		_tprintf (TEXT ("Erro na view da mem partilhada dos dados (%d).\n"), GetLastError ());
		CloseHandle (data.hMapFile);
		return 1;
	}

	msgdata.continueThread = 1;
	data.continueThread = 1;

	_beginthreadex (0, 0, listenClientThread, &msgdata, 0, &tidMSG);
	_beginthreadex (0, 0, listenServerThread, &data, 0, &tidDATA);

	tMSG = OpenThread (THREAD_ALL_ACCESS, FALSE, tidMSG);
	tDATA = OpenThread (THREAD_ALL_ACCESS, FALSE, tidDATA);

	// TODO: check (not sure if wrong)
	WaitForSingleObject (tMSG, INFINITE);
	WaitForSingleObject (tDATA, INFINITE);

	UnmapViewOfFile (msgdata.msgs);
	UnmapViewOfFile (data.shared);
	CloseHandle (msgdata.hMapFile);
	CloseHandle (data.hMapFile);
    return 0;
}