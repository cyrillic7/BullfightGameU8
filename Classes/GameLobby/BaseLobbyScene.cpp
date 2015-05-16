//
//  BaseLobbyScene.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#include "BaseLobbyScene.h"
#include "Tools/Tools.h"
#include "PopDialogBox/PopDialogBoxUserInfo.h"
#include "PopDialogBox/PopDialogBoxSetUp.h"
#include "PopDialogBox/PopDialogBoxMore.h"
#include "PopDialogBox/PopDialogBoxActivity.h"
#include "PopDialogBox/PopDialogBoxTask.h"
#include "PopDialogBox/PopDialogBoxBank.h"
#include "PopDialogBox/PopDialogBoxShop.h"
#include "PopDialogBox/PopDialogBoxAuction.h"
#include "PopDialogBox/PopDialogBoxRanking.h"
#include "Tools/DataModel.h"
BaseLobbyScene::BaseLobbyScene(){
}
BaseLobbyScene::~BaseLobbyScene(){
	CCLog("~ <<%s>>", __FUNCTION__);
}
//进入场景
void BaseLobbyScene::onEnter(){
	CCLayer::onEnter();
	//添加背景
	CCSize deviceSize=DataModel::sharedDataModel()->deviceSize;
	CCSprite *spriteBg=CCSprite::create("res/main_bg.jpg");
	this->addChild(spriteBg);
	spriteBg->setPosition(ccp(deviceSize.width/2,deviceSize.height/2));
	float scale=deviceSize.height/spriteBg->getContentSize().height;
	spriteBg->setScale(scale);
	//创建UI层
	UILayer *m_pWidget = UILayer::create();
	this->addChild(m_pWidget);
	//加载UI层
	UILayout *pWidget = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIBaseLobby.ExportJson)));
	m_pWidget->addWidget(pWidget);
	//绑定设置按键
	UIButton* button  = static_cast<UIButton*>(m_pWidget->getWidgetByName("ButtonSetUp"));
	button->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuCallback));
	//绑定用户信息按键
	pBUserInfo= static_cast<UIButton*>(m_pWidget->getWidgetByName("buttonUser"));
	pBUserInfo->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::menuResetUser));
	//绑定更多按键
	button = static_cast<UIButton*>(m_pWidget->getWidgetByName("ButtonMore"));
	button->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuCallback));
	//绑定活动按键
	button = static_cast<UIButton*>(m_pWidget->getWidgetByName("ButtonActivity"));
	button->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuCallback));
	//绑定任务按键
	button = static_cast<UIButton*>(m_pWidget->getWidgetByName("ButtonTask"));
	button->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuCallback));
	//绑定银行按键
	button = static_cast<UIButton*>(m_pWidget->getWidgetByName("ButtonBank"));
	button->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuCallback));
	//绑定商店按键
	button = static_cast<UIButton*>(m_pWidget->getWidgetByName("ButtonShop"));
	button->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuCallback));
	//绑定拍卖按键
	button = static_cast<UIButton*>(m_pWidget->getWidgetByName("ButtonAuction"));
	button->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuCallback));
	//绑定排行按键
	button = static_cast<UIButton*>(m_pWidget->getWidgetByName("ButtonRanking"));
	button->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuCallback));
	//用户名
	userName=static_cast<UILabel*>(m_pWidget->getWidgetByName("labelUserName"));
	//金币
	pLabelGoldCount=static_cast<UILabel*>(m_pWidget->getWidgetByName("LabelGoldCount"));
	pLabelGoldCount->setText(CCString::createWithFormat("%lld",DataModel::sharedDataModel()->userInfo->lScore)->getCString());
}
//退出场景
void BaseLobbyScene::onExit(){
	CCLayer::onExit();
}
//弹出框
void BaseLobbyScene::popDialogBox(PopType type){
	PopDialogBox *pdb = NULL;
	switch (type)
	{
	case BaseLobbyScene::POP_USER_INFO:
		pdb=PopDialogBoxUserInfo::create();
		break;
	case BaseLobbyScene::POP_SETUP:
		pdb=PopDialogBoxSetUp::create();
		break;
	case BaseLobbyScene::POP_MORE:
		pdb=PopDialogBoxMore::create();
		break;
	case BaseLobbyScene::POP_ACTIVITY:
		pdb=PopDialogBoxActivity::create();
		break;
	case BaseLobbyScene::POP_TASK:
		pdb=PopDialogBoxTask::create();
		break;
	case BaseLobbyScene::POP_BANK:
		pdb=PopDialogBoxBank::create();
		break;
	case BaseLobbyScene::POP_SHOP:
		pdb=PopDialogBoxShop::create();
		break;
	case BaseLobbyScene::POP_AUCTION:
		pdb=PopDialogBoxAuction::create();
		break;
	case BaseLobbyScene::POP_RANKING:
		pdb=PopDialogBoxRanking::create();
		break;
	default:
		break;
	}
	this->addChild(pdb);
}
//菜单回调
void BaseLobbyScene::onMenuCallback(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			UIButton *button=(UIButton*)pSender;
			if(strcmp(button->getName(),"ButtonSetUp")==0)
			{
				popDialogBox(POP_SETUP);
			}
			else if(strcmp(button->getName(),"ButtonMore")==0)
			{
				popDialogBox(POP_MORE);
			}
			else if(strcmp(button->getName(),"ButtonActivity")==0)
			{
				popDialogBox(POP_ACTIVITY);
			}
			else if(strcmp(button->getName(),"ButtonTask")==0)
			{
				popDialogBox(POP_TASK);
			}
			else if(strcmp(button->getName(),"ButtonTask")==0)
			{
				popDialogBox(POP_TASK);
			}
			else if(strcmp(button->getName(),"ButtonBank")==0)
			{
				popDialogBox(POP_BANK);
			}
			else if(strcmp(button->getName(),"ButtonShop")==0)
			{
				popDialogBox(POP_SHOP);
			}
			else if(strcmp(button->getName(),"ButtonAuction")==0)
			{
				popDialogBox(POP_AUCTION);
			}
			else if(strcmp(button->getName(),"ButtonRanking")==0)
			{
				popDialogBox(POP_RANKING);
			}
		}
		break;
	default:
		break;
	}
}
//用户信息回调
void BaseLobbyScene::menuResetUser(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		popDialogBox(POP_USER_INFO);
		break;
	default:
		break;
	}
}

