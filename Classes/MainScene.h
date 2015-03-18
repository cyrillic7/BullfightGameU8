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
USING_NS_CC;
USING_NS_CC_EXT;
class MainScene:public CCLayer
{
public:
	enum GameState
	{
		STATE_REDING=0, 
	};

	CC_SYNTHESIZE(GameHUD *, gameHUD, GameHUD);
	
public:
    MainScene();
    ~MainScene();
	virtual void onEnter();
	virtual void onExit();
    static CCScene* scene();
    
    CREATE_FUNC(MainScene);
private:
	//初始化
	void initHUD();
	//初始化扑克
	void initCard();
	//添加背景
	void addBg();
	void testLogic();
};

#endif /* defined(__BullfightGame__MainScene__) */
