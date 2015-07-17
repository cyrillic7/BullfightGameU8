/*
 * PopDialogBoxBindingPhone.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxBindingPhone.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../Network/MD5/MD5.h"
#include "../Platform/coPlatform.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxBindingPhone::PopDialogBoxBindingPhone()
	:pIPopAssistBindingPhone(NULL)
	, eBindingPhoneType(BINDING_GET_CODE)
{
	scheduleUpdate();
}
PopDialogBoxBindingPhone::~PopDialogBoxBindingPhone() {
	CCLOG("~ <<%s>>",__FUNCTION__);
	unscheduleUpdate();
	//TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_BINDING_PHONE);
	//gameSocket.Destroy(true);
}
void PopDialogBoxBindingPhone::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxBindingPhone.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	//获取验证码
	pBGetCode = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonGetCode"));
	pBGetCode->addTouchEventListener(this, toucheventselector(PopDialogBoxBindingPhone::onMenuGetCode));
	
	//绑定
	UIButton *pBSendBinding = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonBindingPhone"));
	pBSendBinding->addTouchEventListener(this, toucheventselector(PopDialogBoxBindingPhone::onMenuBinding));
	//输入手机号
	pTFPhone = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldPhone"));
	addEditBox(pTFPhone, kEditBoxInputModePhoneNumber);
	//输入验证码
	pTFCode = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldCode"));
	addEditBox(pTFCode, kEditBoxInputModeNumeric);
	

	playAnimation();
}
void PopDialogBoxBindingPhone::onExit(){
	if (getIPopAssistBindingPhone())
	{
		getIPopAssistBindingPhone()->onCloseBindingPhone();
	}
	CCLayer::onExit();
}
void PopDialogBoxBindingPhone::updateResetGetCode(float dt){
	iCurTimeCount--;
	if (iCurTimeCount<=0)
	{
		pBGetCode->setBright(true);
		pBGetCode->setTouchEnabled(true);
		pBGetCode->setTitleColor(ccc3(255, 255, 255));
		pBGetCode->setTitleText(" 获取验证码 ");

		unschedule(SEL_SCHEDULE(&PopDialogBoxBindingPhone::updateResetGetCode));
	}
	else
	{
		pBGetCode->setTitleText(CCString::createWithFormat("重新获取(%d秒)", iCurTimeCount)->getCString());
	}
}
//获取验证码按键
void PopDialogBoxBindingPhone::onMenuGetCode(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		CCEditBox *pEBPhone = (CCEditBox*)pTFPhone->getNodeByTag(TAG_INPUT_EDIT_BOX);
		
		std::string sPhone = pEBPhone->getText();
		if (sPhone.length()<11)
		{
			showTipInfo("请输入正确的手机号码!");
		}
		else
		{
			iCurTimeCount = 60;
			pBGetCode->setBright(false);
			pBGetCode->setTouchEnabled(false);
			pBGetCode->setTitleColor(ccc3(100,100,100));
			pBGetCode->setTitleText(CCString::createWithFormat("重新获取(%d秒)",iCurTimeCount)->getCString());
			
			schedule(SEL_SCHEDULE(&PopDialogBoxBindingPhone::updateResetGetCode),1);
			setBindingPhoneType(BINDING_GET_CODE);
			connectServer();
			connectSuccess();
		}
		
	}
}
//绑定按键
void PopDialogBoxBindingPhone::onMenuBinding(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		CCEditBox *pEBCode= (CCEditBox*)pTFCode->getNodeByTag(TAG_INPUT_EDIT_BOX);
		std::string sCode = pEBCode->getText();
		if (sCode.length()==0)
		{
			showTipInfo("请输入正确的验证码!");
		}
		else
		{
			setBindingPhoneType(BINDING_PHONE);
			connectServer();
			connectSuccess();
		}
		
	}
}
//////////////////////////////////////////////////////////////////////////
void PopDialogBoxBindingPhone::update(float delta){
	if (isReadMessage)
	{
		//MessageQueue::update(delta);
	}
	gameSocket.updateSocketData();
}
//读取网络消息回调
void PopDialogBoxBindingPhone::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
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
		//TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_BINDING_PHONE);
		gameSocket.Destroy(true);
	}
	break;
	default:
		CCLOG("other:%d   %d<<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
		break;
	}
}
//连接成功
void PopDialogBoxBindingPhone::connectSuccess(){
	switch (eBindingPhoneType)
	{
	case PopDialogBoxBindingPhone::BINDING_GET_CODE:	//获取验证码
	{
		CMD_GP_Get_Captcha getCaptcha;
		getCaptcha.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;

		
		std::string password = DataModel::sharedDataModel()->sLogonPassword;
		MD5 m;
		m.ComputMd5(password.c_str(), password.length());
		std::string md5PassWord = m.GetMd5();
		strcpy(getCaptcha.szLogonPass, md5PassWord.c_str());

		CCLOG("pass:%s <<%s>>",getCaptcha.szLogonPass, __FUNCTION__);//5afb7b5360ab76ae55734560b574845d

		CCEditBox *pEBPhone = (CCEditBox*)pTFPhone->getNodeByTag(TAG_INPUT_EDIT_BOX);
		sPhone = pEBPhone->getText();
		strcpy(getCaptcha.szPhone, sPhone.c_str());

		strcpy(getCaptcha.szMachineID, platformAction("{\"act\":100}").c_str());

		bool isSend=gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_GET_CAPTCHA, &getCaptcha, sizeof(getCaptcha));
		if (isSend)
		{
			//移除loading
			//this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
			//关闭网络
			//TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_BINDING_PHONE);
		}
	}
	break;
	case BINDING_PHONE:
	{
		CCEditBox *pEBCode = (CCEditBox*)pTFCode->getNodeByTag(TAG_INPUT_EDIT_BOX);
		std::string sCode = pEBCode->getText();

		CMD_GP_Send_Captcha sendCaptcha;
		sendCaptcha.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;
		sendCaptcha.dwCaptcha = strtoll(sCode.c_str(), NULL, 10);
		gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_SEND_CAPTCHA, &sendCaptcha, sizeof(sendCaptcha));
	}
		break;
	default:
		break;
	}
}
//用户服务
void PopDialogBoxBindingPhone::onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_SEND_CAPTCHA://验证
	{
		assert(wDataSize <= sizeof(CMD_GP_CaptchaRet));
		CMD_GP_CaptchaRet *catchaRet=(CMD_GP_CaptchaRet*)pDataBuffer;
		if (catchaRet->lResultCode==0)
		{
			DataModel::sharedDataModel()->sPhone = sPhone;
		}
		showTipInfo(GBKToUTF8(catchaRet->szDescribeString));
	}
	break;
	case SUB_GP_GET_CAPTCHA://获取验证码
	{
		assert(wDataSize <= sizeof(CMD_GP_CaptchaRet));
		CMD_GP_CaptchaRet *catchaRet = (CMD_GP_CaptchaRet*)pDataBuffer;
		if (catchaRet->lResultCode!=0)
		{
			showTipInfo(GBKToUTF8(catchaRet->szDescribeString));
		}
	}
		break;
	default:
		CCLOG("   %d<<%s>>", wSubCmdID, __FUNCTION__);
		break;
	}
}