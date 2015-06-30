//
//  MainSceneBase.h主场景基类
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//
#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../GameLogic/GameLogic.h"
#include "../Play/CardLayer/CardLayerBase.h"
#include "../Play/PlayerLayer/PlayerLayer.h"
USING_NS_CC;
USING_NS_CC_EXT;
#define MAX_CHAIR_COUNT        6				//最大椅子数
class MainSceneBase:public CCLayer,public GameLogic
{
public:
	//游戏状态
	enum GameState
	{
		//二人牛牛////////////////////////////////////////////////////////////////////////
		STATE_OBSERVER = 0,			//旁观状态
		STATE_DOWN,					//坐下
		STATE_READY,				//准备状态
		STATE_WAIT,					//等待服务端响应
		STATE_SEND_CARD,			//发牌状态
		STATE_CALL_BANKER,			//抢庄状态
		STATE_BETTING,				//投注
		STATE_OPT_OX,				//选牛
		STATE_END,					//结算
		STATE_SWAP_CARD,			//换牌
		STATE_WAIT_SWAP_CARE,		//等待其它人换牌
		//百人牛牛////////////////////////////////////////////////////////////////////////
		STATE_GAME_FREE ,							//空闲状态
		STATE_GAME_PLACE_JETTON,				//下注
		STATE_GAME_SEND_CARD,					//发牌
		STATE_GAME_SHOW_CARE_FINISH,		//显示牌完成
		STATE_GAME_END,								//结算
	};
	CC_SYNTHESIZE(GameState, gameState, GameState);
	CC_SYNTHESIZE(GameState, serverState, ServerState);
	//椅子位置
	CCPoint posChair[MAX_CHAIR_COUNT];
	//扑克层
	CardLayerBase *cardLayer;
	//玩家信息层
	PlayerLayer *playerLayer;
public:
    MainSceneBase();
    ~MainSceneBase();
	virtual void onEnter();
	virtual void onExit();
	//初始化玩家信息层
	virtual void initPlayerLayer();
	//收到准备完成回调
	void onEventReadyFnish();
	//加载Loading
	void addLoadingLayer(std::string socketName);
	//移除Loading
	void removeLoadingLayer();
	//加载标题
	void addTitle();
public:
	//设置状态并更新
	virtual void setGameStateWithUpdate(GameState gameState){}
	virtual void setServerStateWithUpdate(GameState serverState){}
};
