#include "stdafx.h"
#include "SocketOpt.h"


static void Init_Extension_Function();
static void Get_Sys_Info();


static Win_IOCP_Info IOCP_Info = { 0 };

bool Create_Winsock_Source(int nCPUs, int IOThreads,int high, int low)
{
	WSADATA wsaData;
	int nRet = WSAStartup(MAKEWORD(high,low),&wsaData);
	if(nRet)
	{
		NETTRACE("----Create_Winsock_Source error:%d---",WSAGetLastError());
		return false;
	}

	if(LOBYTE(wsaData.wVersion) != low && HIBYTE(wsaData.wVersion) != high)
	{
		NETTRACE("---Create_Winsock_Source is version error:%d---",WSAGetLastError());
		WSACleanup();
		return false;
	}

	Init_Extension_Function();
	Get_Sys_Info();
	
	IOCP_Info.dwCPUs     = nCPUs;
	IOCP_Info.IOThreads  = IOThreads;
	IOCP_Info.IOCPHandle = Create_IOCP_Port(nCPUs);

	return true;
}

void Destory_Socket_Source()
{
	int nRet = WSACleanup();
	if(nRet)
	{
		NETTRACE("-----Destory_Socket_Source error:%d------",WSAGetLastError());
	}
}


bool Make_Socket_Nonblocking(SOCKET nSocket)
{
	u_long block = 1;
	int nRet = ioctlsocket(nSocket,FIONBIO,&block);
	if(nRet)
	{
		NETTRACE("------Make_Socket_Nonblocking error:%d-----",WSAGetLastError());
	}
	return !nRet;
}


#ifndef WSAID_CONNECTEX
#define WSAID_CONNECTEX \
{0x25a207b9,0xddf3,0x4660,{0x8e,0xe9,0x76,0xe5,0x8c,0x74,0x06,0x3e}}
#endif

#ifndef WSAID_DISCONNECTEX
#define WSAID_DISCONNECTEX \
{0x7fda2e11,0x8630,0x436f,{0xa0, 0x31, 0xf5, 0x36, 0xa6, 0xee, 0xc1, 0x57}}
#endif

#ifndef WSAID_GETACCEPTEXSOCKADDRS
#define WSAID_GETACCEPTEXSOCKADDRS \
{0xb5367df2,0xcbac,0x11cf,{0x95,0xca,0x00,0x80,0x5f,0x48,0xa1,0x92}}
#endif

#ifndef WSAID_ACCEPTEX
#define WSAID_ACCEPTEX \
{0xb5367df1,0xcbac,0x11cf,{0x95,0xca,0x00,0x80,0x5f,0x48,0xa1,0x92}}
#endif


static Win_Extension_fun Win_Ex_fun = { 0 };

static void* Get_Extension_Function(SOCKET s,const GUID& whick_fun)
{
	void *funPtr = 0;
	DWORD dwBytes=0;
	int nRet = WSAIoctl(s,SIO_GET_EXTENSION_FUNCTION_POINTER,
		(LPVOID)&whick_fun,sizeof(whick_fun),
		funPtr,sizeof(funPtr),
		&dwBytes,0,0);

	if (nRet)
	{
		NETTRACE("---Get_Extension_Function error:%d----",WSAGetLastError());
	}

	return funPtr;
}

static void Init_Extension_Function()
{
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET)
	{
		NETTRACE("----Init_Extension_Function error:%d----",WSAGetLastError());
		return;
	}

	Win_Ex_fun._AcceptExPtr = (AcceptExPtr)Get_Extension_Function(s, WSAID_ACCEPTEX);
	Win_Ex_fun._ConnectPtr  = (ConnectExPtr)Get_Extension_Function(s, WSAID_CONNECTEX);
	Win_Ex_fun._DisConnectExPtr = (DisConnectExPtr)Get_Extension_Function(s, WSAID_DISCONNECTEX);
	Win_Ex_fun._GetAcceptExSockaddrsPtr = (GetAcceptExSockaddrsPtr)Get_Extension_Function(s, WSAID_GETACCEPTEXSOCKADDRS);

	closesocket(s);
}

Win_Extension_fun* Get_Win_Extension_Fun()
{
	return &Win_Ex_fun;
}


static Win_Sys_Info sys_info = { 0 };

static void Get_Sys_Info()
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	sys_info.dwPageSize = sysInfo.dwPageSize;
	sys_info.dwCpus = sysInfo.dwNumberOfProcessors;
}

Win_Sys_Info* Get_Win_Sys_Info()
{
	return &sys_info;
}


Win_IOCP_Info* Get_Win_IOCP_Info()
{
	return &IOCP_Info;
}


HANDLE Create_IOCP_Port(int nCPUs)
{
	if (nCPUs == 0)
	{
		Win_Sys_Info *pInfo = Get_Win_Sys_Info();
		IOCP_Info.dwCPUs = nCPUs = pInfo->dwCpus;
	}

	return CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, nCPUs);
}

bool Associate_IOCP_Port(SOCKET s, HANDLE IOCPHandle, ULONG_PTR CompletionKey)
{
	HANDLE h = CreateIoCompletionPort((HANDLE)s, IOCPHandle, CompletionKey, 0);
	if (!h)
	{
		NETTRACE("------Associate_IOCP_Ports error:%d-----",WSAGetLastError());
	}
	return h == IOCPHandle;
}



