/*
 * PopDialogBoxForgetPassword.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxForgetPassword.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../LogonScene/LogonScene.h"
#include "../Network/MD5/MD5.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxForgetPassword::PopDialogBoxForgetPassword()
	:isInputId(true)
	, eForgetPwdType(FORGET_GET_ID)
	, dwUserID(0)
{
	scheduleUpdate();
}
PopDialogBoxForgetPassword::~PopDialogBoxForgetPassword() {
	CCLOG("~ <<%s>>",__FUNCTION__);
	//
	
	unscheduleUpdate();
	//TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_FORGET_PWD);
}
void PopDialogBoxForgetPassword::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxForgetPassword.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	//下一步按键
	pBNext = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonForgetPwd"));
	pBNext->addTouchEventListener(this, toucheventselector(PopDialogBoxForgetPassword::onMenuForgetPassword));
	//获取验证码
	pBGetCode = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonGetCode"));
	pBGetCode->addTouchEventListener(this, toucheventselector(PopDialogBoxForgetPassword::onMenuGetCode));
	//输入帐号容器
	pPInputId = static_cast<UIPanel*>(pUILayer->getWidgetByName("PanelInputId"));
	pPInputId->setEnabled(true);
	//找到密码容器
	pPForgetPwd = static_cast<UIPanel*>(pUILayer->getWidgetByName("PanelChangePwd"));
	pPForgetPwd->setEnabled(false);
	//输入帐号框
	pTFInputId = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldId")); 
	addEditBox(pTFInputId, kEditBoxInputModeAny);
	//输入手机
	UITextField *pTFPhone = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldPhone"));
	addEditBox(pTFPhone, kEditBoxInputModePhoneNumber);
	pEBInputPhone = (CCEditBox*)pTFPhone->getNodeByTag(TAG_INPUT_EDIT_BOX);
	pEBInputPhone->setEnabled(false);
	//输入新密码
	UITextField *pTFNewPwd = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldNewPwd"));
	addEditBox(pTFNewPwd, kEditBoxInputModeAny);
	pEBInputNewPwd = (CCEditBox*)pTFNewPwd->getNodeByTag(TAG_INPUT_EDIT_BOX);
	pEBInputNewPwd->setEnabled(false);
	//输入验证码
	UITextField *pTFCode = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldCode"));
	addEditBox(pTFCode, kEditBoxInputModePhoneNumber);
	pEBInputCode = (CCEditBox*)pTFCode->getNodeByTag(TAG_INPUT_EDIT_BOX);
	pEBInputCode->setEnabled(false);
	

	playAnimation();
}
void PopDialogBoxForgetPassword::onExit(){
	((LogonScene*)this->getParent())->isReadMessage = true;
	CCLayer::onExit();
}
//找回密码////////////////////////////////////////////////////////////////////////
void PopDialogBoxForgetPassword::onMenuForgetPassword(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		if (isInputId)
		{
			CCEditBox *pEBInputID = (CCEditBox*)pTFInputId->getNodeByTag(TAG_INPUT_EDIT_BOX);
			if (strcmp(pEBInputID->getText(), "") == 0)
			{
				showTipInfo("帐号不能为空");
			}
			else
			{
				connectServer();
				connectSuccess();
				//connectServer(SOCKET_FORGET_PWD);
			}
		}
		else
		{
			if (strcmp(pEBInputNewPwd->getText(), "") == 0)
			{
				showTipInfo(" 新密码不能为空 ");
			}
			else if (strlen(pEBInputNewPwd->getText()) < 8)
			{
				showTipInfo("新密码必须大于8位以上!");
			}
			else if (strcmp(pEBInputCode->getText(), "") == 0)
			{
				showTipInfo(" 验证码不能为空 ");
			}
			
			else
			{
				setForgetPwdType(FORGET_CHANGE_PWD);
				connectServer();
				connectSuccess();
				//connectServer(SOCKET_FORGET_PWD);
			}
			
		}
	}
}
//获取验证码
void PopDialogBoxForgetPassword::onMenuGetCode(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		setForgetPwdType(FORGET_CODE);
		//connectServer(SOCKET_FORGET_PWD);
		connectServer();
		connectSuccess();

		iCurTimeCount = 60;
		pBGetCode->setBright(false);
		pBGetCode->setTouchEnabled(false);
		pBGetCode->setTitleColor(ccc3(100, 100, 100));
		pBGetCode->setTitleText(CCString::createWithFormat("重新获取(%d秒)", iCurTimeCount)->getCString());
		schedule(SEL_SCHEDULE(&PopDialogBoxForgetPassword::updateResetGetCode), 1);
	}
}
void PopDialogBoxForgetPassword::updateResetGetCode(float dt){
	iCurTimeCount--;
	if (iCurTimeCount <= 0)
	{
		pBGetCode->setBright(true);
		pBGetCode->setTouchEnabled(true);
		pBGetCode->setTitleColor(ccc3(255, 255, 255));
		pBGetCode->setTitleText(" 获取验证码 ");

		unschedule(SEL_SCHEDULE(&PopDialogBoxForgetPassword::updateResetGetCode));
	}
	else
	{
		pBGetCode->setTitleText(CCString::createWithFormat("重新获取(%d秒)", iCurTimeCount)->getCString());
	}
}

//更新
void PopDialogBoxForgetPassword::update(float delta){
	gameSocket.updateSocketData(delta);
}
//////////////////////////////////////////////////////////////////////////
//读取网络消息回调
void PopDialogBoxForgetPassword::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_MB_SOCKET://连接成功
	{
		//connectSuccess();
	}
	break;
	case MDM_GP_USER_SERVICE://用户服务
	{
		onEventUserService(wSubCmdID, pDataBuffer, wDataSize);
		//移除loading
		if (this->getChildByTag(TAG_LOADING))
		{
			this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
		}
		gameSocket.Destroy(true);
		//关闭网络
		//TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_FORGET_PWD);
	}
	break;
	default:
		CCLOG("other:%d   %d<<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
		break;
	}
}
//连接成功
void PopDialogBoxForgetPassword::connectSuccess(){
	switch (eForgetPwdType)
	{
	case PopDialogBoxForgetPassword::FORGET_GET_ID://获取ID
	{
		CMD_GP_Accounts acc;
		CCEditBox *pEBInputID = (CCEditBox*)pTFInputId->getNodeByTag(TAG_INPUT_EDIT_BOX);
		strcpy(acc.szAccounts, pEBInputID->getText());
		//发送数据
		gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_CHECK_ACCOUNT, &acc, sizeof(acc));
	}
		break;
	case PopDialogBoxForgetPassword::FORGET_CODE://获取验证码
	{
		CMD_GP_GetCaptchaByUserID captchaByUserID;
		captchaByUserID.dwUserID = dwUserID;
		//发送数据
		bool isSend = gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_GET_CAPTCHA_BY_ID, &captchaByUserID, sizeof(captchaByUserID));
		if (isSend)
		{
			//移除loading
			//this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
			//关闭网络
			//TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_FORGET_PWD);
		}
	}
		break;
	case PopDialogBoxForgetPassword::FORGET_CHANGE_PWD://修改密码
	{
		CMD_GP_SetPass setPwd;
		setPwd.dwUserID = dwUserID;

		std::string sNewPwd = pEBInputNewPwd->getText();
		MD5 m;
		m.ComputMd5(sNewPwd.c_str(), sNewPwd.length());
		std::string md5PassWord = m.GetMd5();

		strcpy(setPwd.szLogonPass, md5PassWord.c_str());
		setPwd.dwCaptcha = strtol(pEBInputCode->getText(),NULL,10);
		//发送数据
		gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_SET_LOGIN_PASS, &setPwd, sizeof(setPwd));
	}
		break;
	default:
		break;
	}
}
//用户服务
void PopDialogBoxForgetPassword::onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_CHECK_ACCOUNT:
	{
		onSubGetID(pDataBuffer, wDataSize);
	}
	break;
	case SUB_GP_GET_CAPTCHA_BY_ID:
	{
		onSubGetCode(pDataBuffer, wDataSize);
	}
		break;
	case SUB_GP_SET_LOGIN_PASS:
	{
		onSubChangePwd(pDataBuffer, wDataSize);
	}
		break;
	default:
		CCLOG("--------------sub:%d <<%s>>", wSubCmdID, __FUNCTION__);
		break;
	}
}
//获取ID
void PopDialogBoxForgetPassword::onSubGetID(void * pDataBuffer, unsigned short wDataSize){
	CMD_GP_AccountsRet *accRet = (CMD_GP_AccountsRet*)pDataBuffer;
	if (accRet->lResultCode == 0)
	{
		pPInputId->setEnabled(false);
		pPForgetPwd->setEnabled(true);
		pPForgetPwd->setVisible(true);
		pBNext->setTitleText(" 修 改 ");
		CCEditBox *pEBInputID = (CCEditBox*)pTFInputId->getNodeByTag(TAG_INPUT_EDIT_BOX);
		pEBInputID->setEnabled(false);
		isInputId = false;
		pEBInputNewPwd->setEnabled(true);
		pEBInputCode->setEnabled(true);
		pEBInputPhone->setText(accRet->szPhone);
		dwUserID = accRet->dwUserID;
	}
	else
	{
		showTipInfo(GBKToUTF8(accRet->szDescribeString).c_str());
	}
}
//获取验证码
void PopDialogBoxForgetPassword::onSubGetCode(void * pDataBuffer, unsigned short wDataSize){

}
//修改密码
void PopDialogBoxForgetPassword::onSubChangePwd(void * pDataBuffer, unsigned short wDataSize){
	CMD_GP_SetPassRet*setPassRet = (CMD_GP_SetPassRet*)pDataBuffer;
	if (setPassRet->lResultCode==0)
	{
		showTipInfo(GBKToUTF8(setPassRet->szDescribeString).c_str(), this);
	}
	else
	{
		showTipInfo(GBKToUTF8(setPassRet->szDescribeString).c_str());
	}
	
}
//关闭提示回调
void PopDialogBoxForgetPassword::onCloseTipInfo(CCLayer *pTipInfo){
	//this->removeAllChildrenWithCleanup(true);
	
	this->removeFromParentAndCleanup(true);
	CCLOG("==============================11111111111111 <<%s>>", __FUNCTION__);
}