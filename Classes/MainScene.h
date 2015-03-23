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
#include "GameControl.h"
#include "TCPSocket.h"
#include "GameLogic.h"
#include "CardLayer.h"
#include "PlayerLayer.h"

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
	//游戏状态
	enum GameState
	{
		STATE_OBSERVER=0,			//旁观状态
		STATE_READY,				//准备状态
		STATE_SEND_CARD,			//发牌状态
	};
	CC_SYNTHESIZE(GameState,gameState,GameState);
	CC_SYNTHESIZE(GameState,serverState,ServerState);

	CC_SYNTHESIZE(GameControl *, gameHUD, GameControl);
	//扑克层
	CardLayer *cardLayer;
	//玩家信息层
	PlayerLayer *playerLayer;
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

	//收到准备完成回调
	void onEventReadyFnish();
	//收到发牌完成回调
	void onEventSendCardFnish();
	//设置状态并更新
	void setGameStateWithUpdate(GameState gameState);
	void setServerStateWithUpdate(GameState serverState);
private:
	//初始化
	void initHUD();
	//初始化扑克层
	void initCardLayer();
	//初始化玩家信息层
	void initPlayerLayer();
	//添加背景
	void addBg();
	//测试逻辑
	void testLogic();
	//测试网络
	
	int threadStart();
	static void* networkThread(void*);
	bool OnEventTCPSocketRead(WORD	wSocketID, TCP_Command tCommand, void * pDataBuffer, WORD wDataSize);
	//更新状态
	void updateState();
};

#endif /* defined(__BullfightGame__MainScene__) */
