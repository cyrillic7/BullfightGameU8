//
//  LogonScene.h
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#ifndef __BullfightGame__LogonScene__
#define __BullfightGame__LogonScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameHUD.h"
#include "TCPSocket.h"
#include "GameLogic.h"
USING_NS_CC;
USING_NS_CC_EXT;
class LogonScene:public CCLayer,public TCPSocket,public GameLogic
{
public:
	enum GameState
	{
		STATE_REDING=0, 
	};

	CC_SYNTHESIZE(GameHUD *, gameHUD, GameHUD);
	
public:
    LogonScene();
    ~LogonScene();
	virtual void onEnter();
	virtual void onExit();
    static CCScene* scene();
    
    CREATE_FUNC(LogonScene);
private:
	//初始化
	void initHUD();
	//初始化扑克层
	void initCardLayer();
	//添加背景
	void addBg();
	//测试逻辑
	void testLogic();
	//测试网络
	void testTcpSocket();

	bool OnEventTCPSocketRead(WORD	wSocketID, TCP_Command tCommand, void * pDataBuffer, WORD wDataSize);
};

#endif /* defined(__BullfightGame__LogonScene__) */