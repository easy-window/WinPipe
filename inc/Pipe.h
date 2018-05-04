#pragma once

typedef void (WINAPI * RECVCALLBACK)(void*, int, LPVOID);

class CPipe
{
public:
	CPipe(void);
	CPipe(LPCWSTR lpName, int nBuffSize = 0, bool bCreate = false);
	~CPipe(void);

	int Create(LPCWSTR lpName, int nBuffSize);
	int Connect(LPCWSTR lpName);
	bool IsOpen();
	int Close(void);
	int Send(void* Buffer, int nSize);
	int SetRecvCallback(RECVCALLBACK callback, LPVOID lpParam);

private:
	static UINT PipiRecvThread(LPVOID lpParam);

	HANDLE m_hPipe;
	bool m_bLoop;
	int m_nBuffSize;
	TCHAR m_szPipeName[1024];
	RECVCALLBACK m_RecvCallback;
	LPVOID m_lpParam;
};
