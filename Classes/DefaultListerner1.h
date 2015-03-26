/* 
 * File:   DefaultListerner1.h
 * Author: beykery
 *
 * Created on 2013年12月30日, 下午7:33
 */

#ifndef DefaultListerner1_H
#define	DefaultListerner1_H
#include "SocketListerner.h"
#include "cocos2d.h"
USING_NS_CC;
class DefaultListerner1 : public SocketListerner,public CCNode {
public:
    DefaultListerner1();
    virtual ~DefaultListerner1();
    void OnClose(TCPSocket* so, bool fromRemote);
    void OnError(TCPSocket* so, const char* e);
    void OnIdle(TCPSocket* so);
    bool OnMessage(TCPSocket* so,unsigned short	wSocketID, TCP_Command tCommand, void * pDataBuffer, unsigned short wDataSize);
    void OnOpen(TCPSocket* so);
private:

};

#endif	/* DefaultListerner1_H */

