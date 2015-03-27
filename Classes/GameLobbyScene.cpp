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
#include "DataModel.h"
#include "SocketThread.h"
#include "DefaultListerner.h"
#include <stdio.h>

#ifdef WIN32
#define UTEXT(str) GBKToUTF8(str)
#else
#define UTEXT(str) str
#endif

#ifdef WIN32
#include "platform/third_party/win32/iconv/iconv.h"

static char g_GBKConvUTF8Buf[5000] = {0};
const char* GBKToUTF8(const char *strChar)
{

	iconv_t iconvH;
	iconvH = iconv_open("utf-8","gb2312");
	if (iconvH == 0)
	{
		return NULL;
	}
	size_t strLength = strlen(strChar);
	size_t outLength = strLength<<2;
	size_t copyLength = outLength;
	memset(g_GBKConvUTF8Buf, 0, 5000);

	char* outbuf = (char*) malloc(outLength);
	char* pBuff = outbuf;
	memset( outbuf, 0, outLength);

	if (-1 == iconv(iconvH, &strChar, &strLength, &outbuf, &outLength))
	{
		iconv_close(iconvH);
		return NULL;
	}
	memcpy(g_GBKConvUTF8Buf,pBuff,copyLength);
	free(pBuff);
	iconv_close(iconvH);
	return g_GBKConvUTF8Buf;
}
#endif

GameLobbyScene::GameLobbyScene(){
	scheduleUpdate();
}
GameLobbyScene::~GameLobbyScene(){
	CCLog("~ <<%s>>", __FUNCTION__);
	unscheduleUpdate();
	
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
	CCLayer::onEnter();
	//contentScaleFactor
	CCSize deviceSize=DataModel::sharedDataModel()->deviceSize;
	//float scale=SCENE_SIZE.height/deviceSize.height;

	CCSprite *spriteBg=CCSprite::create("res/main_bg.jpg");
	this->addChild(spriteBg);
	spriteBg->setPosition(ccp(deviceSize.width/2,deviceSize.height/2));
	float scale=deviceSize.height/spriteBg->getContentSize().height;
	spriteBg->setScale(scale);
	
	UILayer *m_pWidget = UILayer::create();
	this->addChild(m_pWidget);


	UILayout *pWidget = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIGameLobby.ExportJson)));
	m_pWidget->addWidget(pWidget);

	UIButton* button = static_cast<UIButton*>(m_pWidget->getWidgetByName("buttonUser"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameLobbyScene::menuResetUser));
	for (int i = 0; i < 3;i++)
	{
		button = static_cast<UIButton*>(m_pWidget->getWidgetByName(CCString::createWithFormat("buttonMode%d",i+1)->getCString()));
		button->addTouchEventListener(this, SEL_TouchEvent(&GameLobbyScene::menuSelectMode));
	}
	scroll=static_cast<UIScrollView*>(m_pWidget->getWidgetByName("ScrollView"));
	scroll->setInnerContainerSize(scroll->getContentSize());
	//用户名
	userName=static_cast<UILabel*>(m_pWidget->getWidgetByName("labelUserName"));
	
	//添加监听事件
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(GameLobbyScene::callbackData),LISTENER_LOGON,NULL);
	initTCPLogon();
}
void GameLobbyScene::callbackData(CCObject *obj){
	//CMD_MB_LogonSuccess *ls = (CMD_MB_LogonSuccess*)obj;
	PopDialogBoxLoading *pdb = (PopDialogBoxLoading*)this->getChildByTag(189);
	pdb->removeFromParentAndCleanup(true);
	//////////////////////////////////////////////////////////////////////////
	//设置用户名
	//char *name=DataModel::sharedDataModel()->logonSuccessUserInfo->szNickName;
	//userName->setText(UTEXT(name));
	//userName->setText("dsdsg");
}
void GameLobbyScene::update(float dt){
	char *name=DataModel::sharedDataModel()->logonSuccessUserInfo->szNickName;
    if(strcmp(userName->getStringValue(),"游客")==0&&strcmp(name, "") != 0)
	{
		userName->setText(UTEXT(name));
		CCLog("333333333333");
	}
}
void GameLobbyScene::onExit(){
	//移除监听事件 
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, LISTENER_LOGON); 

	CCLayer::onExit();
}
void GameLobbyScene::menuResetUser(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		popDialogBoxUserInfo();
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

void GameLobbyScene::popDialogBoxUserInfo(){
	PopDialogBox *pdb = PopDialogBoxUserInfo::create();
	this->addChild(pdb);
	pdb->playAnimation();
}
void GameLobbyScene::enterLobbyByMode(int mode){
	switch (mode)
	{
	case MODE_CLASSIC:
		Tools::setTransitionAnimation(0, 0, ClassicLobbyScene::scene());
		break;
	default:
		break;
	}
}
void GameLobbyScene::initTCPLogon(){
	tcpLogon=TCPLogon::create();
	this->addChild(tcpLogon);
	tcpLogon->startSendThread();
	/*SocketThread *socketThread=SocketThread::GetInstance();
	//SocketThread::GetInstance()->getSocket().SetListerner(new DefaultListerner());
	socketThread->start();
	*/
	PopDialogBox *pdb = PopDialogBoxLoading::create();
	this->addChild(pdb);
	pdb->setTag(189);
}
bool GameLobbyScene::OnEventTCPSocketRead(unsigned short wSocketID, TCP_Command tCommand, void * pDataBuffer, unsigned short wDataSize){
	CCLog("========================");
	return 1;
}
