#include "stdafx.h"
#include "SocketOpt.h"



bool Create_Winsock_Source(int high,int low)
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


static void* Get_Extension_Function(SOCKET s,const GUID *whick_fun)
{
	void *funPtr = 0;
	DWORD dwBytes=0;
	WSAIoctl(s,SIO_GET_EXTENSION_FUNCTION_POINTER,(LPVOID)whick_fun,sizeof(*whick_fun),funPtr,sizeof(funPtr),&dwBytes,0,0);
	return funPtr;
}