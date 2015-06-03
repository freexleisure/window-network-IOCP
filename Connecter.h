
#ifndef CONNECTER_H__
#define CONNECTER_H__


#include "NetObject.h"

class Connecter : public NetObject
{
public:
	Connecter(void);
	~Connecter(void);

	bool Connect_To_Server(const std::string strIP,unsigned short nPort);

private:
	SOCKET _socket;
	std::string _strIP;
	unsigned short _nPort;
};


#endif
