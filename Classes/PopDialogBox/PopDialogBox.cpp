/*
 * PopDialogBox.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBox.h"
//#include "../Network/ListernerThread/LogonGameListerner.h"
#include "../Network/CMD_Server/Packet.h"
#include "../Network/HttpDownload/ImageDownloader.h"
#include "../Network/TCPSocket/CGameSocket.h"
#include "PopDialogBoxLoading.h"
#include "PopDialogBoxOtherLoading.h"
#include "PopDialogBoxTipInfo.h"
#include "PopDialogBoxInputNum.h"
#include "../GameLobby/BaseLobbyScene.h"
#include "../Tools/DataModel.h"
#include "../PopDialogBox/PopDialogBoxDial.h"
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
	CCLOG("~ <<%s>>",__FUNCTION__);
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
//获取loading
PopDialogBoxLoading *PopDialogBox::getLoading(){
	PopDialogBoxLoading *pLoading = (PopDialogBoxLoading*)this->getChildByTag(TAG_LOADING);
	return pLoading;
}
//连接服务器
void PopDialogBox::connectServer(){
	//PopDialogBoxOtherLoading *box = PopDialogBoxOtherLoading::create();
	PopDialogBox *box = PopDialogBoxLoading::create();
	this->addChild(box, 10, TAG_LOADING);

	if (gameSocket.getSocketState() != CGameSocket::SOCKET_STATE_CONNECT_SUCCESS)
	{
		if (DataModel::sharedDataModel()->ipaddr.length() == 0)
		{
			std::string sLogonAddr = DataModel::sharedDataModel()->getLogonAddr();
			struct hostent* hostInfo = gethostbyname(sLogonAddr.c_str());
			if (hostInfo)
			{
				DataModel::sharedDataModel()->ipaddr = inet_ntoa(*(struct in_addr *)*hostInfo->h_addr_list);
			}
		}
		gameSocket.setIGameSocket(this);
		gameSocket.Create(DataModel::sharedDataModel()->ipaddr.c_str(), PORT_LOGON);
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
////显示提示语
void PopDialogBox::showTipInfo(const char* sInfo,IPopAssistTipInfo *pITipInfo){
	PopDialogBoxTipInfo *pTipInfo = PopDialogBoxTipInfo::create();
	this->addChild(pTipInfo, 10);
	pTipInfo->setIPopAssistTipInfo(pITipInfo);
	pTipInfo->setTipInfoContent(sInfo);
}
//显示提示语
void PopDialogBox::showTipInfo(const char* sInfo, IPopAssistTipInfo *pITipInfo, int eButtonType){
	PopDialogBoxTipInfo *pTipInfo = PopDialogBoxTipInfo::create();
	this->addChild(pTipInfo, 10);
	pTipInfo->setTipButtonContent((PopDialogBoxTipInfo::TipButtonType)eButtonType);
	pTipInfo->setIPopAssistTipInfo(pITipInfo);
	pTipInfo->setTipInfoContent(sInfo);
}
//设置大厅是否读取网络消息
void PopDialogBox::setLobbyReadMessage(bool isRead){
	((BaseLobbyScene*)this->getParent())->isReadMessage = isRead;
}
//显示数量输入框
void PopDialogBox::showInputNumBox(BuyType eBuyType, const char* cPropName, const char* cPropImagePuth, long lMaxNum, long long lPice, long vipDiscount, int vipLevel, IPopAssist* pIPop){
	PopDialogBoxInputNum *box = PopDialogBoxInputNum::create();
	if (eBuyType==BUY_AUCTION||eBuyType==BUY_SHOP_VIP)
	{
		box->isShowVipDiscount = false;
	}
	else
	{
		box->isShowVipDiscount = true;
	}
	this->addChild(box, 10, TAG_INPUT_BOX);
	box->setInputData(eBuyType, cPropName, cPropImagePuth, lMaxNum, lPice, vipDiscount, vipLevel);
	box->setIPopAssist(pIPop);
}
//加载下载图片
void PopDialogBox::addDownloadImage(UIWidget *widget, const char *name, CCPoint pos, float fScale, int zOrder, bool useMask){
	widget->removeAllNodes();
	CCNode *pNImage = CCNode::create();
	widget->addNode(pNImage, zOrder);
	const char* url_item = IMAGE_URL(name);
	const char* image_name = IMAGE_NAME(name);
	CCLOG("%s <<%s>>",url_item, __FUNCTION__);
	ImageDownloader* item = ImageDownloader::create();
	item->SendHttpRequest(this, imagenotification_selector(PopDialogBox::loadCompleteCallBack), url_item, pNImage, image_name);
	
	pNImage->setPosition(pos);
	pNImage->setScale(fScale);
}
//图片下载完成回调
void PopDialogBox::loadCompleteCallBack(){

}
//添加EditBox
void PopDialogBox::addEditBox(UITextField *pTextField, EditBoxInputMode eInputMode){
	pTextField->setTouchEnabled(false);
	//CCEditBox* m_pEditName = CCEditBox::create(pTextField->getContentSize(), CCScale9Sprite::createWithSpriteFrameName("u_info_change_bg.png"));
	CCEditBox* pEditBox = CCEditBox::create(pTextField->getSize(), CCScale9Sprite::create("res/u_input.png"));
	pEditBox->setPosition(CCPointZero);
	pEditBox->setFontSize(pTextField->getFontSize());
	pEditBox->setFontColor(pTextField->getColor());//设置文字颜色
	

	pEditBox->setPlaceHolder(pTextField->getPlaceHolder());//占位符
	pEditBox->setPlaceholderFontSize(pTextField->getFontSize());

    if (pTextField->getMaxLength()>0) {
        pEditBox->setMaxLength(pTextField->getMaxLength());//最大长度
    }
	
	if (pTextField->isPasswordEnabled())
	{
		pEditBox->setInputFlag(kEditBoxInputFlagPassword);//设置为密码模式
	}
	pEditBox->setInputMode(eInputMode);//设置键盘模式
	pEditBox->setTouchPriority(0);
	pEditBox->setAnchorPoint(pTextField->getAnchorPoint());
	pEditBox->setTag(TAG_INPUT_EDIT_BOX);
	pEditBox->setText(pTextField->getStringValue());
	pEditBox->setReturnType(kKeyboardReturnTypeDefault);//键盘回车键名字
	pEditBox->setDelegate(this);
	//pEditBox->setColor(ccc3(243,218,185));
	//pEditBox->setOpacity(0);
	pTextField->addNode(pEditBox, 10);
}
void PopDialogBox::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
	CCLOG("editBox %p DidBegin !", editBox);
}

void PopDialogBox::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
	CCLOG("editBox %p DidEnd !", editBox);
}

void PopDialogBox::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
	CCLOG("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void PopDialogBox::editBoxReturn(CCEditBox* editBox)
{
	CCLOG("editBox %p was returned !", editBox);

	/*if (m_pEditName == editBox)
	{
		m_pTTFShowEditReturn->setString("Name EditBox return !");
	}
	else if (m_pEditPassword == editBox)
	{
		m_pTTFShowEditReturn->setString("Password EditBox return !");
	}
	else if (m_pEditEmail == editBox)
	{
		m_pTTFShowEditReturn->setString("Email EditBox return !");
	}*/
}

/*//更新socket收发数据
void PopDialogBox::updateSocketData(){
	if (gameSocket.getSocketState() == CGameSocket::SOCKET_STATE_FREE) {
		return;
	}
	if (gameSocket.getSocketState() == CGameSocket::SOCKET_STATE_ERROR) {
		showTipInfo("与服务器断开连接");
		//移除loading
		if (this->getChildByTag(TAG_LOADING))
		{
			this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
		}
		gameSocket.setSocketState(CGameSocket::SOCKET_STATE_FREE);
		return;
	}
	if (gameSocket.getSocketState() == CGameSocket::SOCKET_STATE_CONNECT_FAILURE) {
		showTipInfo(" 连接服务器失败 ");
		//移除loading
		if (this->getChildByTag(TAG_LOADING))
		{
			this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
		}
		gameSocket.setSocketState(CGameSocket::SOCKET_STATE_FREE);
		return;
	}
	//检查
	if (!gameSocket.Check()) {
		//gameSocket = NULL;
		//onConnectionAbort();
		gameSocket.setSocketState(CGameSocket::SOCKET_STATE_ERROR);
		// 掉线了
		CCLOG("abort------------- <<%s>>", __FUNCTION__);
		return;
	}
	gameSocket.Flush();
	// 接收数据（取得缓冲区中的所有消息，直到缓冲区为空）
	while (true)
	{
		char buffer[_MAX_MSGSIZE] = { 0 };
		int nSize = sizeof(buffer);
		char* pbufMsg = buffer;
		if (gameSocket.getSocketState() != CGameSocket::SOCKET_STATE_CONNECT_SUCCESS)
		{
			if (gameSocket.getSocketState() != CGameSocket::SOCKET_STATE_FREE)
			{
				gameSocket.setSocketState(CGameSocket::SOCKET_STATE_ERROR);
			}
			break;
		}
		if (!gameSocket.ReceiveMsg(pbufMsg, nSize)) {
			break;
		}
		// 接收数据（取得缓冲区中的所有消息，直到缓冲区为空）
		TCP_Head* pHead = (TCP_Head*)pbufMsg;
		WORD wDataSize = nSize - sizeof(TCP_Head);
		void * pDataBuffer = pbufMsg + sizeof(TCP_Head);
		onEventReadMessage(pHead->CommandInfo.wMainCmdID, pHead->CommandInfo.wSubCmdID, pDataBuffer, wDataSize);
	}
}*/
void PopDialogBox::onError(const char* sErroe){
	showTipInfo(sErroe);
	//移除loading
	if (this->getChildByTag(TAG_LOADING))
	{
		this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
	}
}
bool PopDialogBox::onMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	onEventReadMessage(wMainCmdID,wSubCmdID,pDataBuffer,wDataSize);
	return true;
}
void PopDialogBox::onOpen(){

}