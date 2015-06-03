#ifndef SOCKETOPT_H__ 
#define SOCKETOPT_H__


bool Create_Winsock_Source(int nCPUs = 0, int IOThreads = 0, int high = 2, int low = 2);
void Destory_Socket_Source();

bool Make_Socket_Nonblocking(SOCKET nSocket);

typedef BOOL(WINAPI *ConnectExPtr)(SOCKET s, const struct sockaddr *name,
	int namelen, PVOID lpSendBuffer,
	DWORD dwSendDataLength, LPDWORD lpdwBytesSent,
	LPOVERLAPPED lpOverlapped);

typedef BOOL(WINAPI *AcceptExPtr)(SOCKET sListenSocket, SOCKET sAcceptSocket, PVOID  lpOutputBuffer,
	DWORD  dwReceiveDataLength, DWORD dwLocalAddressLength,
	DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived,
	LPOVERLAPPED lpOverlapped);

typedef BOOL(WINAPI *DisConnectExPtr)(SOCKET s, LPOVERLAPPED lpOverlapped, DWORD  dwFlags, DWORD  dwReserved);

typedef void (WINAPI * GetAcceptExSockaddrsPtr)(PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength,
	DWORD dwRemoteAddressLength, struct sockaddr **LocalSockaddr, LPINT LocalSockaddrLength,
struct sockaddr **RemoteSockaddr, LPINT RemoteSockaddrLength);


struct Win_Extension_fun
{
	ConnectExPtr _ConnectPtr;
	AcceptExPtr  _AcceptExPtr;
	DisConnectExPtr _DisConnectExPtr;
	GetAcceptExSockaddrsPtr _GetAcceptExSockaddrsPtr;
};

Win_Extension_fun *Get_Win_Extension_Fun();


struct Win_Sys_Info
{
	DWORD dwPageSize;
	DWORD dwCpus;
};


Win_Sys_Info *Get_Win_Sys_Info();

struct Win_IOCP_Info
{
	DWORD dwCPUs;
	DWORD IOThreads;
	HANDLE IOCPHandle;
};

Win_IOCP_Info *Get_Win_IOCP_Info();

HANDLE Create_IOCP_Port(int nCPUs = 0);

bool  Associate_IOCP_Port(SOCKET s, HANDLE IOCPHandle, ULONG_PTR CompletionKey);
#endif
