// Dll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Dll.h"

BOOL initMSGMemAndSync (MSG_DATA * msgdata)
{
	msgdata->hMapFile = CreateFileMapping (
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		MSG_BUFF_SZ,
		szMSGName
	);
	if (msgdata->hMapFile == NULL)
	{
		_tprintf (TEXT ("Problema a criar memoria partilhada das mensagens (%d).\n"), GetLastError ());
		return FALSE;
	}

	msgdata->hRWMutex = CreateMutex (NULL, FALSE, MSG_MUTEX_NAME);
	if (msgdata->hRWMutex == NULL)
	{
		_tprintf (TEXT ("Problema ao criar rwMutext das mensagens (%d).\n"), GetLastError ());
		return FALSE;
	}
	return TRUE;
}

BOOL initGameMemAndSync (DATA * data)
{
	data->hMapFile = CreateFileMapping (
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		MSG_BUFF_SZ,
		szMSGName
	);
	if (data->hMapFile == NULL)
	{
		_tprintf (TEXT ("Problema a criar memoria partilhada das mensagens (%d).\n"), GetLastError ());
		return FALSE;
	}

	data->hMutex = CreateMutex (NULL, FALSE, MSG_MUTEX_NAME);
	if (data->hMutex == NULL)
	{
		_tprintf (TEXT ("Problema ao criar rwMutext das mensagens (%d).\n"), GetLastError ());
		return FALSE;
	}
	return TRUE;
}

void writeMSG (MSG_DATA * msgdata, SHARED_MSG * msg)
{
	int posW;
	WaitForSingleObject (msgdata->hRWMutex, INFINITE);
	
	posW = msgdata->msgs->posWrite;

	CopyMemory (msgdata->msgs->shared_msgs[posW], msg, MSG_BUFF_SZ);

	ReleaseMutex (msgdata->hRWMutex);
}

void readMSG (MSG_DATA * msgdata, SHARED_MSG * msg)
{
	int posR;

	WaitForSingleObject (msgdata->hRWMutex, INFINITE);
	posR = msgdata->msgs->posRead;

	CopyMemory (msg, msgdata->msgs->shared_msgs[posR], sizeof (SHARED_MSG));

	ReleaseMutex (msgdata->hRWMutex);
}

