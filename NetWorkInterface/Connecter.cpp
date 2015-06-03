#include "stdafx.h"
#include "Connecter.h"


Connecter::Connecter(void)
	:_socket(INVALID_SOCKET)
{
	_socket = socket(AF_INET,SOCK_STREAM,0);
	Make_Socket_Nonblocking(_socket);
}


Connecter::~Connecter(void)
{
}

