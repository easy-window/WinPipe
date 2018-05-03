#include "StdAfx.h"
#include "Pipe.h"

CPipe::CPipe(void)
{
	m_hPipe = INVALID_HANDLE_VALUE;
	m_bLoop = false;
	m_RecvCallback = NULL;
	m_nBuffSize = 0;
}

CPipe::CPipe(LPCWSTR lpName, int nBuffSize, bool bCreate)
{
	m_hPipe = INVALID_HANDLE_VALUE;
	m_bLoop = false;
	m_RecvCallback = NULL;
	m_nBuffSize = nBuffSize;
	if (bCreate)
	{
		Create(lpName, nBuffSize);
	}
	else
	{
		Connect(lpName);
	}
}

CPipe::~CPipe(void)
{
	Close();
}

int CPipe::Create(LPCWSTR lpName, int nBuffSize)
{
	if (!lpName || nBuffSize <= 0) return 1;
	m_hPipe = CreateNamedPipe( 
		lpName,             
		PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,       
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 
		PIPE_UNLIMITED_INSTANCES, 
		nBuffSize,         
		nBuffSize,
		0,
		NULL);            

	if (m_hPipe==INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	_tcscpy(m_szPipeName, lpName);
	m_nBuffSize = nBuffSize;
	m_bLoop = true;
	AfxBeginThread(PipiRecvThread, this);
	return 0;
}

int CPipe::Connect(LPCWSTR lpName)
{
	if (!WaitNamedPipe(lpName, NMPWAIT_USE_DEFAULT_WAIT))
	{
		TRACE0("No Read Pipe Accessible");
		return 1;  
	}  

	m_hPipe = CreateFile(
		lpName, 
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, 
		OPEN_EXISTING,
		0,
		NULL);

	if (m_hPipe == INVALID_HANDLE_VALUE)
	{
		DWORD dwError = GetLastError();
		if (ERROR_BROKEN_PIPE == dwError) return 2;
		TRACE1("Open Read Pipe Failed! Error:%d\n", dwError);
		return dwError;
	}
	return 0;
}

int CPipe::Close(void)
{
	m_bLoop = false;
	m_RecvCallback = NULL;
	m_nBuffSize = 0;
	if (INVALID_HANDLE_VALUE != m_hPipe)
		SetCommMask(m_hPipe, 0);
	if (INVALID_HANDLE_VALUE != m_hPipe) 
		CloseHandle(m_hPipe);
	m_hPipe = INVALID_HANDLE_VALUE;
	return 0;
}

int CPipe::Send(void* Buffer, int nSize)
{
	DWORD dwReturn = 0;
	if (!WriteFile(m_hPipe, Buffer, nSize, &dwReturn, NULL))
	{
		DWORD dwError = GetLastError();
		if (ERROR_NO_DATA == dwError) {
			TRACE0("Pipe was closed!\n");
		} 
		else
		{
			TRACE1("Write Failed!%d\n", dwError);
		}
		return dwError;
	}
	return 0;
}

int CPipe::SetRecvCallback(RECVCALLBACK callback, LPVOID lpParam)
{
	if (!callback) return 1;
	m_RecvCallback = callback;
	m_lpParam = lpParam;
	return 0;
}

UINT CPipe::PipiRecvThread(LPVOID lpParam)
{
	CPipe* pMain = (CPipe*) lpParam;
	DWORD dwReturn = 0;

	while  (pMain->m_bLoop) {
		OVERLAPPED overlapped;
		memset(&overlapped,0,sizeof(overlapped));
		overlapped.hEvent = CreateEvent(
			NULL,  // no security attributes 
			TRUE,  // auto reset event 
			FALSE, // not signaled 
			NULL   // no name 
		);

		ConnectNamedPipe(pMain->m_hPipe, &overlapped);
		while(pMain->m_bLoop && WAIT_OBJECT_0 != WaitForSingleObject(overlapped.hEvent, 0)) Sleep(1000);

		DWORD dwEvtMask = 0;
		GetCommMask(pMain->m_hPipe, &dwEvtMask);
		dwEvtMask |= EV_RXCHAR;
		SetCommMask(pMain->m_hPipe, dwEvtMask);

		while (pMain->m_bLoop) {
			WaitCommEvent(pMain->m_hPipe, &dwEvtMask, NULL);
			if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
			{
				COMSTAT ComStat; 
				DWORD dwErrorFlags = 0; 
				ClearCommError(pMain->m_hPipe, &dwErrorFlags, &ComStat);
				DWORD dwLength = ComStat.cbInQue; 
				if(dwLength < pMain->m_nBuffSize)
					continue;

				char* buff = new char[pMain->m_nBuffSize];
				dwReturn = 0;
				if (ReadFile(pMain->m_hPipe, buff, pMain->m_nBuffSize, &dwReturn, NULL))
				{
					if(pMain->m_RecvCallback) pMain->m_RecvCallback(buff, dwReturn, pMain->m_lpParam);
				} 
				else 
				{
					dwReturn = GetLastError();
					if (ERROR_BROKEN_PIPE == dwReturn)
					{
						TRACE0("Pipe was disconnect!\n");
						break;
					}
					TRACE1("Read Failed! Error:%d\n", dwReturn);
				}
				delete[] buff;
			}
		}
		if (INVALID_HANDLE_VALUE != pMain->m_hPipe) 
			CloseHandle(pMain->m_hPipe);
		if (ERROR_BROKEN_PIPE == dwReturn && pMain->m_bLoop)
			pMain->Create(pMain->m_szPipeName, pMain->m_nBuffSize);
	}
	return 0;
}
