
#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Tools/GameConfig.h"
USING_NS_CC;
USING_NS_CC_EXT;
//如果是android平台需要定义宏
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID||CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
#define RtlCopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#define CopyMemory RtlCopyMemory
typedef unsigned char       BYTE;
#endif
class MainSceneBase;
class  Card;

#define MAX_CARD_COUNT								5						//最大牌数
#define SEND_CARD_DELAY_TIME						0.05					//发牌延时时长 

class CardLayerBase:public CCLayer {
public:
	//牌数据
	BYTE card[6][5];
	//能否发牌
	bool canSendCard[MAX_PLAYER];
	//牌
	Card *pCard[MAX_PLAYER*MAX_CARD_COUNT]; 
	//发牌计数器
	short sSendCardCount;
	//发牌状态
	enum SendCardState
	{
		SEND_STATE_WAIT=0,			//等待发牌
		SEND_STATE_ING,				//发牌中
		SEND_STATE_END,				//发牌结束
	};
	//牛牛点数动画
	CCArmature *pAOxType[MAX_PLAYER];
private:
	//扑克集
	CCBatchNode *batchCard;
	CC_SYNTHESIZE(SendCardState, sendCardState, SendCardState);
public:
	CardLayerBase();
	~CardLayerBase();
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
public:
	//更新状态
	virtual void updateServerState();
	//
	virtual void updateGameState();
	//提示牛牛
	virtual bool promptOx(int oxIndex){return true;}
	//重排牛牛
	virtual void sortingOx(int chairID,int showChairiD){}
	
	//显示牌
	virtual void showCard(int index,int dataIndex){}
	//显示牛牛点数
	//virtual void showOxType(int chairiD,int oxType){}
public:
	MainSceneBase*getMainScene();
	//获取当前局总牌数
	short getCurAllCardCount();
	//发牌
	void sendCard();
	//重置牌
	void resetCard();
private:
	//设置发牌区域
	virtual void setCanSendCard();
	virtual void sendCardIng();

	//移动单张牌
	virtual void moveCardAction(CCArmature *armature, float fTime, CCPoint targetPos,int index);
	//发5张牌
	virtual void sendFiveCard(int index, int offsetIndex);
	//获取牌缩放比例
	virtual float getCardScale(int index);
private:
	//初始化所有牌
	void initAllCard();
	//初始化牛牛点数动画
	void initOxType();
	//单张牌发完回调
	void onSendCardFinish();
};
