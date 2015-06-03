
#ifndef NETOBJECT_H__
#define NETOBJECT_H__

class NetObject
{
public:
	NetObject();
	virtual ~NetObject();

protected:
	HANDLE _hIOCP;
};

#endif
