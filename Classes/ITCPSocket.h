#pragma once
#include "Packet.h"
class ITCPSocket
{
public:
	virtual bool OnEventTCPSocketRead(unsigned short	wSocketID, TCP_Command tCommand, void * pDataBuffer, unsigned short wDataSize){return false;}
};

