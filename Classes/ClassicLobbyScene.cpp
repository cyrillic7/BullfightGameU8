//
//  ClassicLobbyScene.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#include "ClassicLobbyScene.h"
#include "Tools.h"
#include "GameConfig.h"
#include "PopDialogBoxUserInfo.h"
#include "GameLobbyScene.h"
#include "MainScene.h"
#include "TCPLogonID.h"
#include "CMD_GameServer.h"
ClassicLobbyScene::ClassicLobbyScene(){
  
}
ClassicLobbyScene::~ClassicLobbyScene(){
	CCLog("~ <<%s>>", __FUNCTION__);
	this->removeAllChildrenWithCleanup(true);
	GUIReader::purge();
	CCArmatureDataManager::purge();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}
CCScene* ClassicLobbyScene::scene()
{
    CCScene *scene = CCScene::create();
    ClassicLobbyScene *layer = ClassicLobbyScene::create();
    scene->addChild(layer);
    return scene;
}
void ClassicLobbyScene::onEnter(){
	CCLayer::onEnter();
	UILayer *m_pWidget = UILayer::create();
	this->addChild(m_pWidget);


	UILayout *pWidget = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIClassicLobby.ExportJson)));
	m_pWidget->addWidget(pWidget);

	UIButton* button = static_cast<UIButton*>(m_pWidget->getWidgetByName("buttonUser"));
	button->addTouchEventListener(this, SEL_TouchEvent(&ClassicLobbyScene::menuResetUser));

	for (int i = 0; i < 4; i++)
	{
		button = static_cast<UIButton*>(m_pWidget->getWidgetByName(CCString::createWithFormat("buttonStar%d",i)->getCString()));
		button->addTouchEventListener(this, SEL_TouchEvent(&ClassicLobbyScene::menuStar));
	}
	initTCPLogon();
}
void ClassicLobbyScene::onExit(){
	CCLayer::onExit();
}
void ClassicLobbyScene::initTCPLogon(){
	TCPLogonID *tcpID=TCPLogonID::create();
	this->addChild(tcpID);
	tcpID->setTag(999);
	tcpID->startSendThread();
}
void ClassicLobbyScene::menuResetUser(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		popDialogBoxUserInfo();
		break;
	default:
		break;
	}
}
void ClassicLobbyScene::menuStar(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		TCPLogonID *tcpID=(TCPLogonID *)this->getChildByTag(999);
		CMD_GR_UserSitDown sit;
		sit.wTableID=28;
		sit.wChairID=1;

		bool isSend=tcpID->ts.SendData(MDM_GR_USER,SUB_GR_USER_SITDOWN,&sit, sizeof(sit));
		CCLog("Classic:%d",isSend);
		//tcpID->sendData();
		//enterMainSceneByMode(((UIButton*)pSender)->getTag());
	}
		break;
	default:
		break;
	}
}
void ClassicLobbyScene::popDialogBoxUserInfo(){
	Tools::setTransitionAnimation(0, 0, GameLobbyScene::scene());
}
void ClassicLobbyScene::enterMainSceneByMode(int mode){
	switch (mode)
	{
	case LEVEL_0:
		Tools::setTransitionAnimation(0, 0, MainScene::scene());
		break;
	default:
		break;
	}
}