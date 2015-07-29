/*
 * PopDialogBoxChangePassword.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxChangePassword.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../Network/MD5/MD5.h"
#include "../Tools/BaseAttributes.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxChangePassword::PopDialogBoxChangePassword()
{
	scheduleUpdate();
}
PopDialogBoxChangePassword::~PopDialogBoxChangePassword() {
	CCLOG("~ <<%s>>",__FUNCTION__);
	unscheduleUpdate();
	//TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_CHANGE_PASSWORD);
	//gameSocket.Destroy(true);
}
void PopDialogBoxChangePassword::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxChangePassword.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	//保存
	UIButton *pBSave = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonChange"));
	pBSave->addTouchEventListener(this, toucheventselector(PopDialogBoxChangePassword::onMenuChangePassword));
	//标题
	pLTitle0 = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelTitle0"));
	pLTitle1 = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelTitle1"));
	//旧密码
	pTFOldPassword = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldOldPassword"));
	addEditBox(pTFOldPassword, kEditBoxInputModeAny);
	//新密码0
	pTFNewPassword0 = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldNewPassword0"));
	addEditBox(pTFNewPassword0, kEditBoxInputModeAny);
	//新密码1
	pTFNewPassword1 = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldNewPassword1"));
	addEditBox(pTFNewPassword1, kEditBoxInputModeAny);
	//播放显示动画
	playAnimation();
	
}
void PopDialogBoxChangePassword::onExit(){
	CCLayer::onExit();
}
//设置类型
void PopDialogBoxChangePassword::setPasswordType(int type){
	setChangePasswordType((ChangePasswordType)type);
	switch (eChangePasswordType)
	{
	case PopDialogBoxChangePassword::CHANGE_LOGON_PASSWORD:
	{
		setTitle("修改登录密码");
	}
		break;
	case PopDialogBoxChangePassword::CHANGE_BANK_PASSWORD:
	{
		setTitle(" 修改保险柜密码 ");
	}
		break;
	default:
		break;
	}
}
//创建密码菜单////////////////////////////////////////////////////////////////////////
void PopDialogBoxChangePassword::onMenuChangePassword(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		CCEditBox *pEBOldPassward = (CCEditBox*)pTFOldPassword->getNodeByTag(TAG_INPUT_EDIT_BOX);
		CCEditBox *pEBNewPassword0 = (CCEditBox*)pTFNewPassword0->getNodeByTag(TAG_INPUT_EDIT_BOX);
		CCEditBox *pEBNewPassword1 = (CCEditBox*)pTFNewPassword1->getNodeByTag(TAG_INPUT_EDIT_BOX);
		string password0 = pEBNewPassword0->getText();
		if (strcmp(pEBOldPassward->getText(), "") == 0 || strcmp(pEBNewPassword0->getText(), "") == 0 || strcmp(pEBNewPassword1->getText(), "") == 0)
		{
			showTipInfo(BaseAttributes::sharedAttributes()->sPasswordEmpty.c_str());
		}
		else if (strlen(pEBNewPassword0->getText()) < 8 || strlen(pEBNewPassword1->getText()) < 8)
		{
			showTipInfo(BaseAttributes::sharedAttributes()->sInsurePasswordLeng.c_str());
		}
		else if (isAllChniese(password0))
		{
			showTipInfo("密码不能包含中文!");
		}
		else if (password0.find_first_not_of("1234567890") == string::npos){
			showTipInfo(BaseAttributes::sharedAttributes()->sInsurePasswordNum.c_str());
		}
		else if (strcmp(pEBNewPassword0->getText(), pEBNewPassword1->getText()) != 0)
		{
			showTipInfo(BaseAttributes::sharedAttributes()->sPasswordInconsistent.c_str());
		}
		else
		{
			connectServer();
			connectSuccess();
		}
	}
}
//设置标题
void PopDialogBoxChangePassword::setTitle(const char * sTitle){
	pLTitle0->setText((sTitle));
	pLTitle1->setText((sTitle));
}

//更新
void PopDialogBoxChangePassword::update(float delta){
	//MessageQueue::update(delta);
	gameSocket.updateSocketData(delta);
}
//////////////////////////////////////////////////////////////////////////
//读取网络消息回调
void PopDialogBoxChangePassword::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_MB_SOCKET://连接成功
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
		//TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_CHANGE_PASSWORD);
		gameSocket.Destroy(true);
	}
		break;
	default:
		CCLOG("other:%d   %d<<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
		break;
	}
}
//连接成功
void PopDialogBoxChangePassword::connectSuccess(){
	switch (eChangePasswordType)
	{
	case PopDialogBoxChangePassword::CHANGE_LOGON_PASSWORD:
	{
		CMD_GP_ModifyLogonPass mLogonPassword;
		mLogonPassword.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;

		//旧密码
		CCEditBox *pEBOldPassward = (CCEditBox*)pTFOldPassword->getNodeByTag(TAG_INPUT_EDIT_BOX);
		std::string szSrcPassword = pEBOldPassward->getText();
		//新密码
		CCEditBox *pEBNewPassword0 = (CCEditBox*)pTFNewPassword0->getNodeByTag(TAG_INPUT_EDIT_BOX);
		std::string szDesPassword = pEBNewPassword0->getText();

		MD5 m;
		m.ComputMd5(szSrcPassword.c_str(), szSrcPassword.length());
		std::string md5PassWord = m.GetMd5();
		strcpy(mLogonPassword.szScrPassword, md5PassWord.c_str());


		m.ComputMd5(szDesPassword.c_str(), szDesPassword.length());
		md5PassWord = m.GetMd5();
		strcpy(mLogonPassword.szDesPassword, md5PassWord.c_str());
		//发送数据
		gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_MODIFY_LOGON_PASS, &mLogonPassword, sizeof(mLogonPassword));
	}
		break;
	case PopDialogBoxChangePassword::CHANGE_BANK_PASSWORD:
	{
		CMD_GP_ModifyInsurePass mInsurePassword;
		mInsurePassword.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;

		//旧密码
		CCEditBox *pEBOldPassward = (CCEditBox*)pTFOldPassword->getNodeByTag(TAG_INPUT_EDIT_BOX);
		std::string szSrcPassword = pEBOldPassward->getText();
		//新密码
		CCEditBox *pEBNewPassword0 = (CCEditBox*)pTFNewPassword0->getNodeByTag(TAG_INPUT_EDIT_BOX);
		std::string szDesPassword = pEBNewPassword0->getText();

		MD5 m;
		m.ComputMd5(szSrcPassword.c_str(), szSrcPassword.length());
		std::string md5PassWord = m.GetMd5();
		strcpy(mInsurePassword.szScrPassword, md5PassWord.c_str());


		m.ComputMd5(szDesPassword.c_str(), szDesPassword.length());
		md5PassWord = m.GetMd5();
		strcpy(mInsurePassword.szDesPassword, md5PassWord.c_str());
		//发送数据
		gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_MODIFY_INSURE_PASS, &mInsurePassword, sizeof(mInsurePassword));
	}
		break;
	default:
		break;
	}
}
//用户服务
void PopDialogBoxChangePassword::onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_OPERATE_SUCCESS:
	{
		CMD_GP_OperateSuccess * pOperateSuccess = (CMD_GP_OperateSuccess *)pDataBuffer;
		//效验数据
		assert(wDataSize >= (sizeof(CMD_GP_OperateSuccess) - sizeof(pOperateSuccess->szDescribeString)));
		if (wDataSize < (sizeof(CMD_GP_OperateSuccess) - sizeof(pOperateSuccess->szDescribeString))) return;
		switch (eChangePasswordType)
		{
		case PopDialogBoxChangePassword::CHANGE_LOGON_PASSWORD:
		{
			CCEditBox *pEBNewPassword0 = (CCEditBox*)pTFNewPassword0->getNodeByTag(TAG_INPUT_EDIT_BOX);
			DataModel::sharedDataModel()->sLogonPassword = pEBNewPassword0->getText();
		}
			break;
		case PopDialogBoxChangePassword::CHANGE_BANK_PASSWORD:
			break;
		default:
			break;
		}
		showTipInfo(GBKToUTF8(pOperateSuccess->szDescribeString).c_str());
	}
		break;
	case SUB_GP_OPERATE_FAILURE:
	{
		//效验参数
		CMD_GP_OperateFailure * pOperateFailure = (CMD_GP_OperateFailure *)pDataBuffer;
		assert(wDataSize >= (sizeof(CMD_GP_OperateFailure) - sizeof(pOperateFailure->szDescribeString)));
		if (wDataSize < (sizeof(CMD_GP_OperateFailure) - sizeof(pOperateFailure->szDescribeString))) return;

		showTipInfo(GBKToUTF8(pOperateFailure->szDescribeString).c_str());
	}
		break;
	default:
		CCLOG("--------------sub:%d <<%s>>", wSubCmdID, __FUNCTION__);
		break;
	}
}