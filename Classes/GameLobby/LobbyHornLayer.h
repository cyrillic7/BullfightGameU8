//
//  LobbyHornLayer.h
//  游戏大厅
//
#pragma once
#include "cocos-ext.h"
#include "BaseLobbyScene.h"
#include "../MTNotificationQueue/MessageQueueGameIng.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
class LobbyHornLayer :public CCLayer, public CCEditBoxDelegate,public CStringAide
{
private:
	//喇叭消息背景
	UIImageView *pIVMsg;
	//喇叭消息滚动容器
	UIScrollView *pSVMsg;
	//输入框
	UITextField *pTFHornMsgContent;
	//
	std::vector<std::string> sContentList;
public:
    LobbyHornLayer();
    ~LobbyHornLayer();
	virtual void onEnter();
	virtual void onExit();
    CREATE_FUNC(LobbyHornLayer);
private:
	//菜单发送消息
	void onMenuSendMsg(CCObject* pSender, TouchEventType type);
	//触摸
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	//移除自己
	void removSelf();
	//显示消息框
	void onShowMsg();
	//隐藏消息框
	void hideMsg();
	//设置滚动容器数据
	void setScrollViewData();
	//创建消息内容
	UILabel *createMsgContent(std::string content);
	//添加EditBox
	void addEditBox(TextField *pTextField, EditBoxInputMode eInputMode);
	//输入框回调
	virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox);
	virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox);
	virtual void editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text);
	virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox);
	//显示提示语
	void showTipInfo(const char* sInfo);
	//更新喇叭消息
	void onUpdateHornMsg(CCObject *obj);
};
