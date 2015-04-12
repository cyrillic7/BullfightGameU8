/* 
 * File:   DefaultListerner.h
 * Author: beykery
 *
 * Created on 2013年12月30日, 下午7:33
 */

#ifndef DEFAULTLISTERNER_H
#define	DEFAULTLISTERNER_H
#include "SocketListerner.h"
#include "cocos2d.h"
USING_NS_CC;

#include "MemoryPool.h"

#define MAX_TCP_LEN 1024*5
struct SendData
{
	char sSendData[MAX_TCP_LEN];
	DWORD dwDataLen;
};

class LogonListerner : public SocketListerner,public CCNode {
public:
    LogonListerner();
    virtual ~LogonListerner();
    void OnClose(TCPSocket* so, bool fromRemote);
    void OnError(TCPSocket* so, const char* e);
    void OnIdle(TCPSocket* so);
    bool OnMessage(TCPSocket* so,unsigned short	wSocketID, TCP_Command tCommand, void * pDataBuffer, unsigned short wDataSize);
    void OnOpen(TCPSocket* so);
private:
	CConcurQueue <SendData >m_RecvData;
};

#endif	/* DEFAULTLISTERNER_H */

