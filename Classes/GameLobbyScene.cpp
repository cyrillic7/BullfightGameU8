//
//  GameLobbyScene.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//
#include "GameLobbyScene.h"
#include "Tools.h"
#include "GameConfig.h"
#include "PopDialogBoxUserInfo.h"
#include "PopDialogBoxLoading.h"
#include "ClassicLobbyScene.h"
#include "GameControlOxHundred.h"
#include "DataModel.h"
#include "PopDialogBoxSetUp.h"
#include "MainSceneOxHundred.h"
GameLobbyScene::GameLobbyScene()
{
}
GameLobbyScene::~GameLobbyScene(){
	CCLog("~ <<%s>>", __FUNCTION__);
	//移除对象
	this->removeAllChildrenWithCleanup(true);
	//清理数据
	GUIReader::purge();
	CCArmatureDataManager::purge();
	//清理纹理
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}
CCScene* GameLobbyScene::scene()
{
    CCScene *scene = CCScene::create();
    GameLobbyScene *layer = GameLobbyScene::create();
    scene->addChild(layer);
    return scene;
}
void GameLobbyScene::onEnter(){
	BaseLobbyScene::onEnter();
	/*//添加背景
	CCSize deviceSize=DataModel::sharedDataModel()->deviceSize;
	CCSprite *spriteBg=CCSprite::create("res/main_bg.jpg");
	this->addChild(spriteBg);
	spriteBg->setPosition(ccp(deviceSize.width/2,deviceSize.height/2));
	float scale=deviceSize.height/spriteBg->getContentSize().height;
	spriteBg->setScale(scale);*/
	UILayer *m_pWidget = UILayer::create();
	this->addChild(m_pWidget);


	UILayout *pWidget = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIGameLobby.ExportJson)));
	m_pWidget->addWidget(pWidget);

	/*UIButton* button = static_cast<UIButton*>(m_pWidget->getWidgetByName("buttonUser"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameLobbyScene::menuResetUser));

	button = static_cast<UIButton*>(m_pWidget->getWidgetByName("ButtonSetUp"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameLobbyScene::menuSetUp));*/
	UIButton* button=NULL;
	for (int i = 0; i < 3;i++)
	{
		button = static_cast<UIButton*>(m_pWidget->getWidgetByName(CCString::createWithFormat("buttonMode%d",i+1)->getCString()));
		button->addTouchEventListener(this, SEL_TouchEvent(&GameLobbyScene::menuSelectMode));
	}
	scroll=static_cast<UIScrollView*>(m_pWidget->getWidgetByName("ScrollView"));
	scroll->setInnerContainerSize(scroll->getContentSize());

	userName->setText(Tools::GBKToUTF8(DataModel::sharedDataModel()->userInfo->szNickName));
	/*//用户名
	userName=static_cast<UILabel*>(m_pWidget->getWidgetByName("labelUserName"));
	//金币
	pLabelGoldCount=static_cast<UILabel*>(m_pWidget->getWidgetByName("LabelGoldCount"));*/
}

void GameLobbyScene::onExit(){
	BaseLobbyScene::onExit();
}
void GameLobbyScene::menuResetUser(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		popDialogBox();
		break;
	default:
		break;
	}
}
void GameLobbyScene::menuSetUp(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			PopDialogBox *pdbSetUp = PopDialogBoxSetUp::create();
			this->addChild(pdbSetUp);
		}
		break;
	default:
		break;
	}
}
void GameLobbyScene::menuSelectMode(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		UIButton *button = (UIButton*)pSender;
		enterLobbyByMode(button->getTag());
	}
		break;
	default:
		break;
	}
}

void GameLobbyScene::popDialogBox(){
	PopDialogBox *pdb = PopDialogBoxUserInfo::create();
	this->addChild(pdb);
}
void GameLobbyScene::enterLobbyByMode(int mode){
	switch (mode)
	{
	case MODE_CLASSIC:
		{
				Tools::setTransitionAnimation(0, 0, ClassicLobbyScene::scene());
		}
		break;
	case MODE_Hundred:
		{
			Tools::setTransitionAnimation(0, 0, MainSceneOxHundred::scene());
		}
		break;
	default:
		break;
	}
}
