/*
 * PopDialogBox.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBox.h"
#include "../Network/ListernerThread/LogonGameListerner.h"
#include "../Network/HttpDownload/ImageDownloader.h"
#include "PopDialogBoxLoading.h"
#include "PopDialogBoxTipInfo.h"
#include "PopDialogBoxInputNum.h"
#include "../GameLobby/BaseLobbyScene.h"
std::string PopDialogBox::sSocketName = "";
PopDialogBox::PopDialogBox()
:pUILayer(NULL)
,pWidgetBg(NULL)
, isReadMessage(true)
{
	pUILayer = UILayer::create();
	addChild(pUILayer);
	
	this->setTouchEnabled(true);
	this->setTouchPriority(0);
	this->setTouchMode(kCCTouchesOneByOne);
}


PopDialogBox::~PopDialogBox() {
	CCLog("~ <<%s>>",__FUNCTION__);
	//GUIReader::purge();
	//this->removeFromParentAndCleanup(true);
	//CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	//this->removeAllChildrenWithCleanup(true);
	//SceneReader::sharedSceneReader()->purge();
	//ActionManager::shareManager()->purge();
	//CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	//CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("ccs/UI/UIAboutHelp/UIAboutHelp0.plist");
	//CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void PopDialogBox::menuBack(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		/*Tools::playSound(kSoundBack);
		GameHUD *gameHUD = DataModel::sharedDataModel()->getMainSceneManage()->getGameHUD();
		gameHUD->changePauseState(false);
		//this->getParent()->removeChild(this, true);
		*/
		this->removeFromParentAndCleanup(true);
	}
}
void PopDialogBox::onMenuBackWithReadMsg(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		setLobbyReadMessage(true);
		this->removeFromParentAndCleanup(true);
	}
}
void PopDialogBox::playAnimation(){
	if (pWidgetBg)
	{
		pWidgetBg->runAction(CCEaseBackOut::create(CCScaleTo::create(0.2, 1)));
	}
}
//连接服务器
void PopDialogBox::connectServer(std::string socketName){
	PopDialogBox *box = PopDialogBoxLoading::create();
	this->addChild(box, 10, TAG_LOADING);
	box->setSocketName(socketName);

	//setSocketName(socketName);

	TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(socketName);
	TCPSocket *tcp = getSocket();
	if (tcp)
	{
		tcp->createSocket(GAME_IP, PORT_LOGON, new LogonGameListerner());
	}
}
void PopDialogBox::setSocketName(std::string sName){
	sSocketName = sName;
}
////显示提示语
void PopDialogBox::showTipInfo(const char* sInfo){
	PopDialogBoxTipInfo *pTipInfo = PopDialogBoxTipInfo::create();
	this->addChild(pTipInfo, 10);
	pTipInfo->setTipInfoContent(sInfo);
}
//设置大厅是否读取网络消息
void PopDialogBox::setLobbyReadMessage(bool isRead){
	((BaseLobbyScene*)this->getParent())->isReadMessage = isRead;
}
//显示数量输入框
void PopDialogBox::showInputNumBox(BuyType eBuyType, const char* cPropName, const char* cPropImagePuth, long lMaxNum, long long lPice, IPopAssist* pIPop){
	PopDialogBoxInputNum *box = PopDialogBoxInputNum::create();
	this->addChild(box, 10, TAG_INPUT_BOX);
	box->setInputData(eBuyType, cPropName, cPropImagePuth, lMaxNum, lPice);
	box->setIPopAssist(pIPop);
}
//加载下载图片
void PopDialogBox::addDownloadImage(UIWidget *widget, const char *name, CCPoint pos, float fScale, bool useMask){
	widget->removeAllNodes();
	CCNode *pNImage = CCNode::create();
	widget->addNode(pNImage, -100);
	const char* url_item = IMAGE_URL(name);
	const char* image_name = IMAGE_NAME(name);

	ImageDownloader* item = ImageDownloader::create();
	item->SendHttpRequest(this, imagenotification_selector(PopDialogBox::loadCompleteCallBack), url_item, pNImage, image_name);
	//pNImage->setScale(0.2);
}
//图片下载完成回调
void PopDialogBox::loadCompleteCallBack(){

}