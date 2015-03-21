//
//  MainScene.h
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#ifndef __BullfightGame__MainScene__
#define __BullfightGame__MainScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameHUD.h"
#include "TCPSocket.h"
#include "GameLogic.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "pthread/pthread.h"
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) ||  (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
#include "CCPThreadWinRT.h"

typedef void THREAD_VOID;
#define THREAD_RETURN
#else

typedef void* THREAD_VOID;
#define THREAD_RETURN 0
#endif

USING_NS_CC;
USING_NS_CC_EXT;
class MainScene:public CCLayer,public TCPSocket,public GameLogic
{
public:
	enum GameState
	{
		STATE_REDING=0, 
	};

	CC_SYNTHESIZE(GameHUD *, gameHUD, GameHUD);
private:
	static pthread_t threadLogon;
public:
    MainScene();
    ~MainScene();
	virtual void onEnter();
	virtual void onExit();
    static CCScene* scene();
    
    CREATE_FUNC(MainScene);

	void testTcpSocket();
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
	
	int threadStart();
	static void* networkThread(void*);
	bool OnEventTCPSocketRead(WORD	wSocketID, TCP_Command tCommand, void * pDataBuffer, WORD wDataSize);
};

#endif /* defined(__BullfightGame__MainScene__) */
