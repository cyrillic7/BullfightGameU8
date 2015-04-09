/* 
 * SocketListerner.h
 * 
 */

#ifndef SOCKETLISTERNER_H
#define	SOCKETLISTERNER_H
#include "TCPSocket.h"
#include "Thread.h"
#include "Packet.h"
class SocketListerner : public Thread {
public:
	SocketListerner();
	virtual ~SocketListerner();
	virtual bool OnMessage(TCPSocket* so,unsigned short	wSocketID, TCP_Command tCommand, void * pDataBuffer, unsigned short wDataSize) = 0;
	virtual void OnClose(TCPSocket* so, bool fromRemote) = 0;
	virtual void OnIdle(TCPSocket* so) = 0;
	virtual void OnOpen(TCPSocket* so) = 0;
	virtual void OnError(TCPSocket* so, const char* e) = 0;
	void SetContext(TCPSocket* context);
	virtual void Run();

private:
	TCPSocket* context;
};
#endif	/* SOCKETLISTERNER_H */

