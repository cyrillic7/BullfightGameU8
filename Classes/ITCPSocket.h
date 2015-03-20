#include "StructLogon.h"
class ITCPSocket
{
public:
	virtual bool OnEventTCPSocketRead(WORD	wSocketID, TCP_Command tCommand, void * pDataBuffer, WORD wDataSize) = 0;
};

