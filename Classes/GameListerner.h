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
#include "GameLogic.h"
USING_NS_CC;
class GameListerner : public SocketListerner,public CCNode,GameLogic {
public:
    GameListerner();
    virtual ~GameListerner();
    void OnClose(TCPSocket* so, bool fromRemote);
    void OnError(TCPSocket* so, const char* e);
    void OnIdle(TCPSocket* so);
    bool OnMessage(TCPSocket* so,unsigned short	wSocketID, TCP_Command tCommand, void * pDataBuffer, unsigned short wDataSize);
    void OnOpen(TCPSocket* so);
private:
	//登录
	bool logonEvent(TCPSocket* pSocket,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//配置
	bool configEvent(TCPSocket* pSocket,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//用户信息
	bool userEvent(TCPSocket* pSocket,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//状态信息
	bool statusEvent(TCPSocket* pSocket,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//框架命令
	bool frameEvent(TCPSocket* pSocket,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//游戏命令
	bool gameEvent(TCPSocket* pSocket,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);

	//////////////////////////////////////////////////////////////////////////
	//用户进入
	bool OnSocketSubUserEnter(TCPSocket* pSocket,void * pDataBuffer, unsigned short wDataSize);
};

#endif	/* DefaultListerner1_H */

