
#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameConfig.h"
#include "GameLogic.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
class Card;
class MainSceneBase;
class CardLayer:public CCLayer,GameLogic {
private:
	//扑克集
	CCBatchNode *batchCard;
	//发牌状态
	enum SendCardState
	{
		SEND_STATE_WAIT=0,			//等待发牌
		SEND_STATE_ING,				//发牌中
		SEND_STATE_END,				//发牌结束
	};
	CC_SYNTHESIZE(SendCardState, sendCardState, SendCardState);
	//能否发牌
	bool canSendCard[MAX_PLAYER];
	//发牌计数器
	short sSendCardCount;
	//牌
	Card *pCard[MAX_PLAYER*MAX_COUNT]; 
	//牛牛点数动画
	CCArmature *pAOxType[MAX_PLAYER];
	MainSceneBase*getMainScene();
public:
	CardLayer();
	~CardLayer();
	CREATE_FUNC(CardLayer);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	void setCanSendCard();
public:
	//更新状态
	virtual void updateServerState();
	//
	virtual void updateGameState();
	//显示牌
	void showCard(int index,int dataIndex);
	//提示牛牛
	bool promptOx(int oxIndex);
	//重排牛牛
	void sortingOx(int chairID,int showChairiD);

	//发牌
	void sendCard();
private:
	//初始化所有牌
	void initAllCard();
	//初始化牛牛点数动画
	void initOxType();
	//重置牌
	void resetCard();

	//发牌中
	virtual void sendCardIng();
	//发5张牌
	virtual void sendFiveCard(int index, int offsetIndex);
	//移动扑克
	void moveCardAction(CCArmature *armature, float fTime, CCPoint targetPos,int index);
	//获取牌放大倍数
	float getCardScale(int index);
	//单张牌发完回调
	void onSendCardFinish();
	//获取当前局总牌数
	short getCurAllCardCount();
	//坐标点转换
	CCPoint designResolutionToFrame(CCPoint designPos);
	//显示牛牛点数
	void showOxType(int chairiD,int oxType);
	void onPlayOxAnimation(CCNode *obj);
};
