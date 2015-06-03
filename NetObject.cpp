#include "stdafx.h"
#include "NetObject.h"


NetObject::NetObject()
	:_hIOCP(Get_Win_IOCP_Info()->IOCPHandle)
{
}


NetObject::~NetObject()
{
}
