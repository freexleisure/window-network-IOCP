#include "stdafx.h"
#include "Connecter.h"


Connecter::Connecter(void)
	:_socket(INVALID_SOCKET),
	_nPort(0)
{
	_socket = socket(AF_INET,SOCK_STREAM,0);
	if (_socket != INVALID_SOCKET)
		Make_Socket_Nonblocking(_socket);
}


Connecter::~Connecter(void)
{
	if (_socket != INVALID_SOCKET)
	{
		closesocket(_socket);
	}
}

bool Connecter::Connect_To_Server(const std::string strIP, unsigned short nPort)
{
	if (INVALID_SOCKET == _socket)
		return false;

	Win_Extension_fun *pExFun = Get_Win_Extension_Fun();
	if (pExFun && pExFun->_ConnectPtr)
	{
		sockaddr_in addr = {0};
		addr.sin_port = htons(nPort);
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = inet_addr(strIP.c_str());

		Associate_IOCP_Port(_socket,_hIOCP, 1);
		//Window is so strange ... 
		if (bind(_socket, (sockaddr*)&addr, sizeof(addr) == SOCKET_ERROR))
		{
			if (WSAGetLastError() != WSAEINVAL)
				return false;
		}

		int rc = pExFun->_ConnectPtr(_socket,(sockaddr*)&addr,sizeof(addr),0,0,0,0);
		if (rc || WSAGetLastError() == WSA_IO_PENDING)
		{
			return true;
		}
		else
		{
			CancelIoEx((HANDLE)_socket, 0);
		}
	}

	return false;
}

