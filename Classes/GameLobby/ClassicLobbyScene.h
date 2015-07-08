//
//  ClassicLobbyScene.h
//	经典厅
//
#pragma once
#include "cocos-ext.h"
#include "BaseLobbyScene.h"
#include "../MTNotificationQueue/MessageQueue.h"
#include "../MTNotificationQueue/MessageQueueGameIng.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
#define MAX_LEVEL_COUNT				4					//最大等级数
class ClassicLobbyScene:public BaseLobbyScene,public MessageQueue,public MessageQueueGameIng
{
private:
	UIButton* pBLevel[MAX_LEVEL_COUNT];
	enum GameLevel
	{
        
		LEVEL_0=1,					//初级
		LEVEL_1,					//中级
		LEVEL_2,					//高级
		LEVEL_3,					//大师
	};
	enum GameItem
	{
		ITEM_0=1,							//二人牛牛
		ITEM_1,								//通比牛牛
		ITEM_2,								//六人换牌
	};
	CC_SYNTHESIZE(GameItem,gameItem,GameItem);

	bool isDeleteList;
	bool isEnterGame;
	//游戏选择卡列表视图
	UIListView *pLVItems;
public:
    ClassicLobbyScene();
    ~ClassicLobbyScene();
	virtual void onEnter();
	virtual void onExit();

    static CCScene* scene();
    CREATE_FUNC(ClassicLobbyScene);
private:
	void update(float delta);
	
	void initTCPLogon(int index);
	//更新房间列表
	void updateRoomList();
	//更新房间列表
	void updateRoom(std::vector <tagGameServer *> vec);
	//初始化游戏选项触摸事件
	void initItemTouchEvent();
	//选择游戏项
	void selectGameItem(int iItemIndex);
	//弹出框
	void popDialogBox();
	//菜单设置用户信息
	void menuResetUser(CCObject* pSender, TouchEventType type);
	void menuStar(CCObject* pSender, TouchEventType type);
	//进入主场景
	void enterMainSceneByMode(int mode);

	void onPlay(CCObject *obj);
	void onConfigFinish(CCObject *obj);

	
	void onEventReadMessage(WORD wMainCmdID,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//socket连接成功
	void onEventConnect(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//登录
	void onEventLogon(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//配置
	void configEvent(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//用户状态
	void onSubUserState(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//复选框回调（选择游戏）
	void onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type);
};