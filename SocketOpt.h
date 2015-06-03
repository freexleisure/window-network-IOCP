#ifndef SOCKETOPT_H__ 
#define SOCKETOPT_H__


bool Create_Winsock_Source(int high=2,int low=2);
void Destory_Socket_Source();


bool Make_Socket_Nonblocking(SOCKET nSocket);



typedef BOOL (WINAPI *ConnectExPtr)(SOCKET s,const struct sockaddr *name,
									int namelen,PVOID lpSendBuffer,
									DWORD dwSendDataLength,LPDWORD lpdwBytesSent,
									LPOVERLAPPED lpOverlapped);

typedef BOOL (WINAPI *AcceptExPtr)(SOCKET sListenSocket,SOCKET sAcceptSocket,PVOID  lpOutputBuffer,
								   DWORD  dwReceiveDataLength,DWORD dwLocalAddressLength,
								   DWORD dwRemoteAddressLength,LPDWORD lpdwBytesReceived,
								   LPOVERLAPPED lpOverlapped);

typedef BOOL(WINAPI *DisConnectExPtr)(SOCKET s,LPOVERLAPPED lpOverlapped,DWORD  dwFlags,DWORD  dwReserved);

typedef void (WINAPI * GetAcceptExSockaddrsPtr)(PVOID lpOutputBuffer,DWORD dwReceiveDataLength,DWORD dwLocalAddressLength,
										DWORD dwRemoteAddressLength,struct sockaddr **LocalSockaddr,LPINT LocalSockaddrLength,
										struct sockaddr **RemoteSockaddr,LPINT RemoteSockaddrLength);




#endif
