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
#include "MainSceneBase.h"
#include "../Play/GameControl/GameControlOxTwo.h"
#include "../Network/TCPSocket/TCPSocket.h"
#include "../GameLogic/GameLogic.h"

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
class MainSceneOxTwo:public TCPSocket,public MainSceneBase
{
public:


	CC_SYNTHESIZE(GameControlOxTwo *, gameControl, GameControlOxTwo);

private:
public:
    MainSceneOxTwo();
    ~MainSceneOxTwo();
	virtual void onEnter();
	virtual void onExit();
    static CCScene* scene();
    //virtual bool init();
    CREATE_FUNC(MainSceneOxTwo);
	

	//收到发牌完成回调
	void onEventSendCardFnish();
	//设置状态并更新
	virtual void setGameStateWithUpdate(GameState gameState);
	virtual void setServerStateWithUpdate(GameState serverState);
private:
	//初始化
	void initGameControl();
	//初始化扑克层
	void initCardLayer();

	//添加背景
	void addBg();
//	bool OnEventTCPSocketRead(unsigned short	wSocketID, TCP_Command tCommand, void * pDataBuffer, unsigned short wDataSize);
	//更新状态
	void updateGameState();
	void updateServerState();
};

#endif /* defined(__BullfightGame__MainScene__) */
