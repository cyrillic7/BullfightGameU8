/*
 * PopDialogBoxSetUp.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxSetUp.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../Tools/BaseAttributes.h"
#include "../Tools/Tools.h"
#include "PopDialogBoxHelp.h"
#include "PopDialogBoxFeedback.h"
#include "PopDialogBoxAbout.h"
#include "PopDialogBoxChangePassword.h"
#include "../LogonScene/LogonScene.h"
#include "../MTNotificationQueue/LobbyMsgHandler.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxSetUp::PopDialogBoxSetUp()
{
	
    
}
PopDialogBoxSetUp::~PopDialogBoxSetUp() {
	CCLOG("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxSetUp::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxSetUp.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);

	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//用户昵称
	UILabel *pLUserName = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelUserName"));
	pLUserName->setText(GBKToUTF8(DataModel::sharedDataModel()->userInfo->szNickName));
	//用户头像
	UIImageView *pIVIcon = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageUserIcon")); 
	pIVIcon->loadTexture(CCString::createWithFormat("u_p_icon_%d.png", DataModel::sharedDataModel()->userInfo->wFaceID)->getCString(), UI_TEX_TYPE_PLIST);

	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::onMenuBackWithReadMsg));
	//音乐
	UIButton *pBMusic = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonMusic"));
	pBMusic->addTouchEventListener(this, toucheventselector(PopDialogBoxSetUp::onMenuMusic));
	changeMusicButtonState(pBMusic, DataModel::isMusic);
	//音效
	UIButton *pBSound = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonSound"));
	pBSound->addTouchEventListener(this, toucheventselector(PopDialogBoxSetUp::onMenuSound));
	changeMusicButtonState(pBSound, DataModel::isSound);
	//帮助按键
	backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonHelp"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBoxSetUp::onMenuHelp));
	//意见反馈
	backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonFeedback"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBoxSetUp::onMenuFeedback));
	//关于我们
	backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonAbout"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBoxSetUp::onMenuAbout));
	//修改登录密码
	backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonCLPassword"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBoxSetUp::onMenuChangePassword));
	//修改保险箱密码
	backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonCBPassword"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBoxSetUp::onMenuChangePassword));
	//切换帐号
	backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonChangeAccount"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBoxSetUp::onMenuChangeAccount));
	playAnimation();
	setLobbyReadMessage(false);
}
void PopDialogBoxSetUp::onExit(){
	CCLayer::onExit();
}
//帮助
void PopDialogBoxSetUp::onMenuHelp(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		PopDialogBoxHelp *help=PopDialogBoxHelp::create();
		this->addChild(help);
	}
}
//意见反馈
void PopDialogBoxSetUp::onMenuFeedback(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		PopDialogBoxFeedback *feedback=PopDialogBoxFeedback::create();
		this->addChild(feedback);
	}
}
//关于我们
void PopDialogBoxSetUp::onMenuAbout(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		PopDialogBoxAbout *about=PopDialogBoxAbout::create();
		this->addChild(about);
	}
}
//修改密码
void PopDialogBoxSetUp::onMenuChangePassword(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		UIButton *pBTemp = (UIButton*)object;
		if (pBTemp->getTag()==1)
		{
		}
		
		PopDialogBoxChangePassword *pChangePasswrod = PopDialogBoxChangePassword::create();
		this->addChild(pChangePasswrod);
		pChangePasswrod->setPasswordType(pBTemp->getTag());
	}
}
//切换帐号
void PopDialogBoxSetUp::onMenuChangeAccount(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		LobbyMsgHandler::sharedLobbyMsgHandler()->gameSocket.Destroy(true);
		DataModel *m = DataModel::sharedDataModel();
		CC_SAFE_RELEASE_NULL(m);

		Tools::setTransitionAnimation(0, 0, LogonScene::scene());

		
	}
}
//音乐开关
void PopDialogBoxSetUp::onMenuMusic(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		DataModel::isMusic = !DataModel::isMusic;
		if (DataModel::isMusic)
		{
		}
		else
		{

		}
		changeMusicButtonState((UIButton*)object, DataModel::isMusic);
		Tools::saveBoolByRMS(RMS_IS_MUSIC, DataModel::isMusic);
	}
}
//音效开关
void PopDialogBoxSetUp::onMenuSound(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		DataModel::isSound = !DataModel::isSound;
		if (DataModel::isSound)
		{
		}
		else
		{

		}
		changeMusicButtonState((UIButton*)object, DataModel::isSound);
		Tools::saveBoolByRMS(RMS_IS_SOUND, DataModel::isSound);
	}
}

//更改音乐状态
void PopDialogBoxSetUp::changeMusicButtonState(UIButton *pButton, bool isOpen){
	if (isOpen)
	{
		pButton->loadTextureNormal("on.png",UI_TEX_TYPE_PLIST);
		pButton->loadTexturePressed("on.png", UI_TEX_TYPE_PLIST);
	}
	else
	{
		pButton->loadTextureNormal("off.png", UI_TEX_TYPE_PLIST);
		pButton->loadTexturePressed("off.png", UI_TEX_TYPE_PLIST);
	}
}