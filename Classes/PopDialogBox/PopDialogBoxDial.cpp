/*
 * PopDialogBoxDial.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxDial.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../Tools/Tools.h"
#include "../Network/MD5/MD5.h"
#include "../Network/CMD_Server/Packet.h"
#include "../PopDialogBox/PopDialogBoxOptTipInfo.h"
#include "../GameLobby/GameLobbyScene.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxDial::PopDialogBoxDial()
	: eDialState(DIAL_FREE)
	, dialIndex(0)
{
	scheduleUpdate();
	//加载扑克动画文件
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(CCS_PATH_SCENE(AnimationDial.ExportJson));
}
PopDialogBoxDial::~PopDialogBoxDial() {
	CCLOG("~ <<%s>>",__FUNCTION__);
	unscheduleUpdate();
	//gameSocket.Destroy(true);
}
void PopDialogBoxDial::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxDial.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::onMenuBackWithReadMsg));
	//开始按键
	pBStart = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonStart"));
	pBStart->addTouchEventListener(this, toucheventselector(PopDialogBoxDial::onMenuStart));
	//转盘指针
	pIVPointer = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImagePointer"));

	pAEnd = CCArmature::create("AnimationDial");
	pIVPointer->addNode(pAEnd);
	pAEnd->getAnimation()->play("AnimationEnd");
	pAEnd->setVisible(false);
	

	CCArmature *pABg = CCArmature::create("AnimationDial");
	pWidgetBg->addNode(pABg);
	pABg->getAnimation()->play("AnimationBg");

	pAStartButton = CCArmature::create("AnimationDial");
	pBStart->addNode(pAStartButton);
	pAStartButton->getAnimation()->play("AnimationButton");
	/*
	
	connectSignServer();*/
	setLobbyReadMessage(false);

	playAnimation();
}
void PopDialogBoxDial::onExit(){
	setLobbyReadMessage(true);
	CCLayer::onExit();
}
//连接服务器
void PopDialogBoxDial::connectDialServer(){
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
	//////////////////////////////////////////////////////////////////////////
	if (getLoading())
	{
		getLoading()->setIOutTime(this);
	}
	connectSuccess();
}
//转盘开始按键
void PopDialogBoxDial::onMenuStart(CCObject *object, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		switch (eDialState)
		{
		case PopDialogBoxDial::DIAL_FREE:
		{
			pAEnd->setVisible(false);
			setDialState(DIAL_ING);
			connectDialServer();
		}
			break;
		default:
			break;
		}
	}
		break;
	default:
		break;
	}
}
//超时回调
void PopDialogBoxDial::onOutTime(){
}
//////////////////////////////////////////////////////////////////////////
void PopDialogBoxDial::update(float delta){
	if (isReadMessage)
	{
		//MessageQueue::update(delta);
	}
	gameSocket.updateSocketData(delta);
}
//读取网络消息回调
void PopDialogBoxDial::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_MB_SOCKET://socket连接成功
	{
		connectSuccess();
	}
	break;
	case MDM_GP_USER_SERVICE://用户服务
	{
		onEventUserService(wSubCmdID, pDataBuffer, wDataSize);
		//移除loading
		//this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
		gameSocket.Destroy(true);
	}
	break;
	default:
		CCLOG("other:%d   %d<<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
		break;
	}
}
//连接成功
void PopDialogBoxDial::connectSuccess(){
	switch (eDialState)
	{
	case PopDialogBoxDial::DIAL_ING:
	{
		CMD_GP_UserID userID;
		userID.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;

		MD5 m;
		m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(), DataModel::sharedDataModel()->sLogonPassword.length());
		std::string md5PassWord = m.GetMd5();
		strcpy(userID.szPassword, md5PassWord.c_str());
		//发送数据
		gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_DIAL, &userID, sizeof(userID));
	}
		break;
	default:
		break;
	}
}
//用户服务
void PopDialogBoxDial::onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_DIAL:
	{
		onSubDial(pDataBuffer,wDataSize);
	}
		break;
	default:
		CCLOG("   %d<<%s>>", wSubCmdID, __FUNCTION__);
	break;
	}
}
//转盘信息
void PopDialogBoxDial::onSubDial(void * pDataBuffer, unsigned short wDataSize){
	CMD_GP_DialLog *signInfo = (CMD_GP_DialLog*)pDataBuffer;
	if (signInfo->dwRet==0)
	{
		dialIndex = signInfo->dwIndex;
		float endAnlg = 360 * 15 + 360.0 / 12.0 * dialIndex;
		CCEaseExponentialIn  *in = CCEaseExponentialIn::create(CCRotateTo::create(8.0, 360 * 15));
		CCEaseExponentialOut  *out = CCEaseExponentialOut::create(CCRotateTo::create(8.0, endAnlg));
		CCCallFunc *call = CCCallFunc::create(this, SEL_CallFunc(&PopDialogBoxDial::dialAnimationFinsh));
		CCSequence *seq = CCSequence::create(in, out, call,NULL);
		pIVPointer->runAction(seq);
	}
	else if (signInfo->dwRet == 2)
	{
		setDialState(DIAL_FREE);
		//showTipInfo(GBKToUTF8(signInfo->szDescribeString).c_str());
		//showTipInfo(GBKToUTF8(signInfo->szDescribeString).c_str(), this, 0);
		PopDialogBoxOptTipInfo *pUTipInfo = PopDialogBoxOptTipInfo::create();
		this->addChild(pUTipInfo, 100);
		pUTipInfo->setIHornMsgAssist(this);
		pUTipInfo->setTipInfoData(GBKToUTF8(signInfo->szDescribeString).c_str());
	}
	else{
		setDialState(DIAL_FREE);
		showTipInfo(GBKToUTF8(signInfo->szDescribeString).c_str());
	}
}
//确定
void PopDialogBoxDial::onSendSure(CCLayer *layer){
	((GameLobbyScene*)this->getParent())->addVip();
	layer->removeFromParentAndCleanup(true);
}
//动画完成
void PopDialogBoxDial::dialAnimationFinsh(){
	pBStart->loadTextureNormal("vipgo.png", UI_TEX_TYPE_PLIST);
	pBStart->loadTexturePressed("vipgo_press.png", UI_TEX_TYPE_PLIST);
	pAStartButton->getAnimation()->play("AnimationButton1");
	pAEnd->setVisible(true);

	scheduleOnce(SEL_SCHEDULE(&PopDialogBoxDial::showReward),0.5);
}
//显示奖励
void PopDialogBoxDial::showReward(float dt){
	CCArmature *pABg = CCArmature::create("AnimationDial");
	pWidgetBg->addNode(pABg,100);
	pABg->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(PopDialogBoxDial::onAnimationEventOver));//动画播完回调用
	pABg->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(PopDialogBoxDial::onAnimationEventFrame));
	pABg->getAnimation()->play(CCString::createWithFormat("AnimationGetProp%d", dialIndex)->getCString());
}
void PopDialogBoxDial::onError(const char *e){
    //PopDialogBox::onError(e);
    showTipInfo(e, this);
    //移除loading
    if (this->getChildByTag(TAG_LOADING))
    {
        this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
    }
}
void PopDialogBoxDial::onCloseTipInfo(cocos2d::CCLayer *pTipInfo){
    this->removeFromParent();
}
void PopDialogBoxDial::onAnimationEventOver(CCArmature *pArmature, MovementEventType movementType, const char *movementID){
	switch (movementType)
	{
	case cocos2d::extension::COMPLETE:
	case cocos2d::extension::LOOP_COMPLETE:
	{
		for (int i = 0; i < 12; i++)
		{
			CCString *aName = CCString::createWithFormat("AnimationGetProp%d", i);
			if (strcmp(movementID, aName->getCString()) == 0){
				pArmature->removeFromParentAndCleanup(true);
				setDialState(DIAL_FREE);
				return;
			}
		}
	}
	break;
	default:
		break;
	}
}
void PopDialogBoxDial::onAnimationEventFrame(CCBone *bone, const char *evt, int originFrameIndex, int currentFrameIndex){
	/*if (strcmp(evt, "bomb1") == 0)
	{

	}*/
}
