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
#include "ClassicLobbyScene.h"
#include "DataModel.h"
#include <stdio.h>
#include "TCPLogonID.h"

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
	CCLayer::onEnter();
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
	//用户名
	userName=static_cast<UILabel*>(m_pWidget->getWidgetByName("labelUserName"));
	initTCPLogon();
}
void GameLobbyScene::onExit(){
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
		//tagGameServer *server=DataModel::sharedDataModel()->tagGameServerList[0];
		TCPLogonID *tcpID=TCPLogonID::create();
		this->addChild(tcpID);
		tcpID->startSendThread();

		/*UIButton *button = (UIButton*)pSender;
		enterLobbyByMode(button->getTag());*/
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
	char *name=DataModel::sharedDataModel()->logonSuccessUserInfo->szNickName;
	
	
	userName->setText(UTEXT(name));
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
}
bool GameLobbyScene::OnEventTCPSocketRead(unsigned short wSocketID, TCP_Command tCommand, void * pDataBuffer, unsigned short wDataSize){
	CCLog("========================");
	return 1;
}
