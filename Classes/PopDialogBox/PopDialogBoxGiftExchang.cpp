/*
 * PopDialogBoxGiftExchang.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxGiftExchang.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxGiftExchang::PopDialogBoxGiftExchang()
{
	scheduleUpdate();
    
}
PopDialogBoxGiftExchang::~PopDialogBoxGiftExchang() {
	unscheduleUpdate();
	CCLOG("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxGiftExchang::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIGiftExchang.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	
	//兑换码
	pTFExchangCode = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldCode"));
	addEditBox(pTFExchangCode, kEditBoxInputModeSingleLine);
	//兑换
	UIButton *pBExchang = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonExchang"));
	pBExchang->addTouchEventListener(this, toucheventselector(PopDialogBoxGiftExchang::onMenuExchang));
	//帐号
	//播放动画
	playAnimation();
}
void PopDialogBoxGiftExchang::onExit(){
	CCLayer::onExit();
}
//兑换
void PopDialogBoxGiftExchang::onMenuExchang(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		CCEditBox *pEBPassword = (CCEditBox*)pTFExchangCode->getNodeByTag(TAG_INPUT_EDIT_BOX);
		std::string password0 = pEBPassword->getText();
		if (strlen(pEBPassword->getText())<8)
		{
			showTipInfo("请输入有效兑换码");
		}
		else if (isAllChniese(pEBPassword->getText()))
		{
			showTipInfo("兑换码不能包含中文!");
		}
		else
		{
			connectServer();
			connectSuccess();
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void PopDialogBoxGiftExchang::update(float delta){
	if (isReadMessage)
	{
		//MessageQueue::update(delta);
	}
	gameSocket.updateSocketData(delta);
}
//读取网络消息回调
void PopDialogBoxGiftExchang::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_MB_SOCKET://socket连接成功
	{
		//connectSuccess();
	}
	break;
	case MDM_GP_USER_SERVICE://用户服务
	{
		onEventUserService(wSubCmdID, pDataBuffer, wDataSize);
		//移除loading
		this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
		//关闭网络
		gameSocket.Destroy(true);
	}
	break;
	default:
		CCLOG("other:%d   %d<<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
		break;
	}
}
//连接成功
void PopDialogBoxGiftExchang::connectSuccess(){
	
	CMD_GP_GiftChange giftChange;
	strcpy(giftChange.szAccounts, DataModel::sharedDataModel()->sLogonAccount.c_str());

	CCEditBox *pEBExchang = (CCEditBox*)pTFExchangCode->getNodeByTag(TAG_INPUT_EDIT_BOX);
	std::string exchang = pEBExchang->getText();
	strcpy(giftChange.szGiftChangeOrder, exchang.c_str());
	giftChange.dwOpTerminal = 2;
	gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_GIFT_CHANGE, &giftChange, sizeof(giftChange));
}
//用户服务
void PopDialogBoxGiftExchang::onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_GIFT_CHANGE:
	{
		CMD_GP_GiftChangeRet *gitfChangeRet = (CMD_GP_GiftChangeRet*)pDataBuffer;
		std::string tipContent = GBKToUTF8(gitfChangeRet->szDescribeString).c_str();
		//tipContent.erase("");
		
		//std::string::iterator new_end = remove_if(tipContent.begin(), tipContent.end(), bind2nd(std::equal_to<char>(), '<br/>'));
		//tipContent.erase(new_end, tipContent.end());
		//tipContent = tipContent.erase(tipContent.find_last_not_of("<br")+1);
		/*while (true)   {
			std::string::size_type   pos(0);
			if ((pos = tipContent.find("br")) != std::string::npos)
				tipContent.replace(pos, 2, "");
			        else   break;
			
		}*/
		replace_all(tipContent,"<br/>","");


		showTipInfo(tipContent.c_str());	
	}
	break;
	default:
		CCLOG("   %d<<%s>>", wSubCmdID, __FUNCTION__);
		break;
	}
}