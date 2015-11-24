/*
 * PopDialogBoxSign.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxSign.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../Tools/Tools.h"
#include "../Network/MD5/MD5.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxSign::PopDialogBoxSign()
	:eSignState(SIGN_LIST)
	, isSign(true)
{
	
	scheduleUpdate();
}
PopDialogBoxSign::~PopDialogBoxSign() {
	CCLOG("~ <<%s>>",__FUNCTION__);
	unscheduleUpdate();
	//TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_SIGN);
	//gameSocket.Destroy(true);
}
void PopDialogBoxSign::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxSign.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::onMenuBackWithReadMsg));
	backButton->setEnabled(false);
	//签到
	pBSign = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonSign"));
	pBSign->addTouchEventListener(this, toucheventselector(PopDialogBoxSign::onMenuSign));
	//签到奖励金币
	pLSignRewardGold = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelSignRewardGold"));
	//签到天数信息
	pLSignDayInfo = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelSignDayInfo"));
	//签到奖励列表
	pLVSign = static_cast<UIListView*>(pUILayer->getWidgetByName("ListViewSignReward"));
	//设置cell模式
	pLVSign->setItemModel(pLVSign->getItem(0));
	updateListSignInfo();

	setLobbyReadMessage(false);
	connectSignServer();

	playAnimation();
}
void PopDialogBoxSign::onExit(){
	CCLayer::onExit();
}
//连接服务器
void PopDialogBoxSign::connectSignServer(){
	connectServer();
	if (getLoading())
	{
		getLoading()->setIOutTime(this);
	}
	connectSuccess();
}
//签到按键
void PopDialogBoxSign::onMenuSign(CCObject *object, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		if (isSign)
		{
			setSignState(SIGN_ING);
			connectSignServer();
		}
		else
		{
			onMenuBackWithReadMsg(NULL, TOUCH_EVENT_ENDED);
		}
	}
		break;
	default:
		break;
	}
}
//超时回调
void PopDialogBoxSign::onOutTime(){
	isSign = false;
	//pBSign->setTitleText("关闭");
	pBSign->loadTextureNormal("GB_Btn_Normal.png", UI_TEX_TYPE_PLIST);
	pBSign->loadTexturePressed("GB_Btn_Down.png", UI_TEX_TYPE_PLIST);
}
//更新签到列表
void PopDialogBoxSign::updateListSignInfo(){
	UIListView *pLVTemp = pLVSign;

	pLVTemp->removeAllItems();
	int tempSize = vecSignInfo.size();
	if (tempSize == 0)
	{
		return;
	}

	for (int i = 0; i < tempSize; i++)
	{
		int inserterPos = pLVTemp->getItems()->count();
		pLVTemp->insertDefaultItem(inserterPos);

		UIImageView *pIVSignBg = static_cast<UIImageView*>(pLVTemp->getItem(inserterPos)->getChildByName("ImageSignBg"));
		//奖励金币
		UILabel *pLRewardGold = static_cast<UILabel*>(pIVSignBg->getChildByName("LabelRewardGoldNum"));
		pLRewardGold->setText(CCString::createWithFormat("%lld金币", vecSignInfo[i].lScore)->getCString());
		//签到天数
		UILabel *pLSignDay = static_cast<UILabel*>(pIVSignBg->getChildByName("LabelSignLimite"));
		//pLSignDay->setText(vecSignInfo[i].dwDay>27?"满月 ":CCString::createWithFormat("连续签到%ld天 ", vecSignInfo[i].dwDay)->getCString());
		pLSignDay->setText(CCString::createWithFormat("连续签到%ld天 ", vecSignInfo[i].dwDay)->getCString());

		//连续签到奖励
		UICheckBox *pCBRewardIcon = static_cast<UICheckBox*>(pIVSignBg->getChildByName("CheckBoxRewardIcon"));
		pCBRewardIcon->loadTextureBackGround(CCString::createWithFormat("u_s_reward_gold%d.png",i)->getCString(), UI_TEX_TYPE_PLIST);
	}
}
//更新签到天数
void PopDialogBoxSign::updateSignDayPanel(int iCurSignDay){
	for (int i = 0; i < 31; i++)
	{
		UIButton *pBSignCurDay = static_cast<UIButton*>(pUILayer->getWidgetByName(CCString::createWithFormat("ButtonSignDay%d",i)->getCString()));
		pBSignCurDay->loadTextureNormal("u_s_unsign.png", UI_TEX_TYPE_PLIST);
		pBSignCurDay->loadTextureDisabled("u_s_sign.png", UI_TEX_TYPE_PLIST);
		if (i<iCurSignDay)
		{
			pBSignCurDay->setBright(false);
		}
		else
		{
			pBSignCurDay->setBright(true);
		}
	}
	//更新列表
	for (int i = 0; i < vecSignInfo.size(); i++)
	{
		int day = vecSignInfo[i].dwDay;
		UIButton *pBSignCurDay = static_cast<UIButton*>(pUILayer->getWidgetByName(CCString::createWithFormat("ButtonSignDay%d", day-1)->getCString()));
		pBSignCurDay->loadTextureNormal(CCString::createWithFormat("u_s_gem_box%d.png",i)->getCString(), UI_TEX_TYPE_PLIST);
		pBSignCurDay->loadTextureDisabled(CCString::createWithFormat("u_s_gem_box_open%d.png", i)->getCString(), UI_TEX_TYPE_PLIST);

		//连续签到奖励
		UIImageView *pIVSignBg = static_cast<UIImageView*>(pLVSign->getItem(i)->getChildByName("ImageSignBg"));
		UICheckBox *pCBRewardIcon = static_cast<UICheckBox*>(pIVSignBg->getChildByName("CheckBoxRewardIcon"));
		if (iCurSignDay>=day)
		{
			pCBRewardIcon->setSelectedState(true);
		}
	}
	//pLSignDayInfo->setText(CCString::createWithFormat("%ld天后清空签到信息", 31L - iCurSignDay)->getCString());
}
//////////////////////////////////////////////////////////////////////////
void PopDialogBoxSign::update(float delta){
	if (isReadMessage)
	{
		//MessageQueue::update(delta);
	}
	gameSocket.updateSocketData(delta);
}
//读取网络消息回调
void PopDialogBoxSign::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
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
		this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
		//关闭网络
		//TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_SIGN);
		gameSocket.Destroy(true);
	}
	break;
	default:
		CCLOG("other:%d   %d<<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
		break;
	}
}
//连接成功
void PopDialogBoxSign::connectSuccess(){
	switch (eSignState)
	{
	case PopDialogBoxSign::SIGN_LIST:
	{
		CMD_GP_GetSignInTask getSignInTask;
		getSignInTask.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;
		getSignInTask.dwOpTerminal = 2;
		gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_GET_SIGNIN_TASK, &getSignInTask, sizeof(getSignInTask));
	}
		break;
	case PopDialogBoxSign::SIGN_ING:
	{
		CMD_GP_SignIn signIn;
		signIn.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;

		signIn.dwOpTerminal=2;

		MD5 m;
		m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(), DataModel::sharedDataModel()->sLogonPassword.length());
		std::string md5PassWord = m.GetMd5();
		strcpy(signIn.szLogonPass, md5PassWord.c_str());

											
		gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_SIGNIN, &signIn, sizeof(signIn));
	}
		break;
	default:
		break;
	}
}
//用户服务
void PopDialogBoxSign::onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_GET_SIGNIN_TASK:
	{
		onSubSignInfo(pDataBuffer,wDataSize);
	}
		break;
	case SUB_GP_SIGNIN:
	{
		onSubSignIn(pDataBuffer, wDataSize);
	}
		break;
	default:
		CCLOG("   %d<<%s>>", wSubCmdID, __FUNCTION__);
	break;
	}
}
//签到信息
void PopDialogBoxSign::onSubSignInfo(void * pDataBuffer, unsigned short wDataSize){
	assert(wDataSize == sizeof(CMD_GP_GetSignInTaskInfo));
	CMD_GP_GetSignInTaskInfo *signInfo = (CMD_GP_GetSignInTaskInfo*)pDataBuffer;
	for (int i = 0; i < 7; i++)
	{
		if (signInfo->SignInTask[i].dwDay>1)
		{
			CMD_GP_SignInTask signInTask;
			signInTask.dwDay = signInfo->SignInTask[i].dwDay;
			signInTask.lScore = signInfo->SignInTask[i].lScore;
			
			vecSignInfo.push_back(signInTask);
		}
		else if (signInfo->SignInTask[i].dwDay==1)
		{
			pLSignRewardGold->setText(CCString::createWithFormat("签到即可领取%lld金币", signInfo->SignInTask[i].lScore)->getCString());
		}
	}
	
	updateListSignInfo();

	updateSignDayPanel(signInfo->dwDay);
	//updateSignDayPanel(31);
}
//签到
void PopDialogBoxSign::onSubSignIn(void * pDataBuffer, unsigned short wDataSize){
	assert(wDataSize == sizeof(CMD_GP_SignInTaskLog));
	CMD_GP_SignInTaskLog *pSignInLog = (CMD_GP_SignInTaskLog*)pDataBuffer;
	if (pSignInLog->dwRet==0)
	{
		updateSignDayPanel(pSignInLog->dwDay);
		pLSignRewardGold->setText(CCString::createWithFormat("签到即可领取%lld金币", pSignInLog->lScore)->getCString());
	}

	//保存签到记录
	saveSignRecord();

	showTipInfo(GBKToUTF8(pSignInLog->szDescribeString).c_str());
	onOutTime();
}
//保存签到记录
void PopDialogBoxSign::saveSignRecord(){
	//更新记录
	map<long, int >::iterator l_it;
	l_it = DataModel::sharedDataModel()->mapSignRecord.find(DataModel::sharedDataModel()->userInfo->dwUserID);
	if (l_it != DataModel::sharedDataModel()->mapSignRecord.end())
	{
		l_it->second = Tools::getCurDay();
	}
	else
	{
		DataModel::sharedDataModel()->mapSignRecord.insert(map<long, int>::value_type(DataModel::sharedDataModel()->userInfo->dwUserID, Tools::getCurDay()));
	}
	//保存
	std::string saveStr="{";
	std::map<long, int>::iterator iter;
	for (iter = DataModel::sharedDataModel()->mapSignRecord.begin(); iter != DataModel::sharedDataModel()->mapSignRecord.end(); iter++)
	{
		saveStr += CCString::createWithFormat("\"signData\":{\"userId\":%ld,\"signDay\":%d},",iter->first,iter->second)->getCString();
	}
	saveStr.erase(saveStr.end()-1);

	saveStr += "}";
	Tools::saveStringByRMS(RMS_SIGN_RECORD, saveStr.c_str());

	
}
void PopDialogBoxSign::onError(const char *e){
    //PopDialogBox::onError(e);
    showTipInfo(e, this);
    //移除loading
    if (this->getChildByTag(TAG_LOADING))
    {
        this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
    }
}
void PopDialogBoxSign::onCloseTipInfo(cocos2d::CCLayer *pTipInfo){
    this->removeFromParent();
}
