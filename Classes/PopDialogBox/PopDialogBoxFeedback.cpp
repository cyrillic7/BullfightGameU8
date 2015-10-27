/*
 * PopDialogBoxFeedback.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxFeedback.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxFeedback::PopDialogBoxFeedback()
{
	scheduleUpdate();
}
PopDialogBoxFeedback::~PopDialogBoxFeedback() {
	CCLOG("~ <<%s>>",__FUNCTION__);
	unscheduleUpdate();
}
void PopDialogBoxFeedback::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxFeedback.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));

	//反馈按键
	UIButton *pBFeedback = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonFeedback"));
	pBFeedback->addTouchEventListener(this, toucheventselector(PopDialogBoxFeedback::onMenuFeedback));
	
	//反馈内容输入框
	UITextField *pTFFeedbackContent = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldContent"));
    pTFFeedbackContent->addEventListenerTextField(this, SEL_TextFieldEvent(&PopDialogBoxFeedback::onTextFeild));
    //addEditBox(pTFFeedbackContent,kEditBoxInputModeSingleLine);
	//pEBFeedbackContent = (CCEditBox*)pTFFeedbackContent->getNodeByTag(TAG_INPUT_EDIT_BOX);

	//pEBFeedbackContent->setEnabled(false);
	//pEBFeedbackContent->setVisible(false);
	
	
	

	//联系方式
	UITextField *pTFFeedbackQQ = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldQQ"));
	addEditBox(pTFFeedbackQQ, kEditBoxInputModePhoneNumber);
	pEBFeedbackQQ = (CCEditBox*)pTFFeedbackQQ->getNodeByTag(TAG_INPUT_EDIT_BOX);
	//pEBFeedbackQQ->setEnabled(false);
	//pEBFeedbackQQ->setVisible(false);

	//UIImageView *pIVInputContact = static_cast<UIImageView*>(pTFFeedbackQQ->getParent());
	//pIVInputContact->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxFeedback::onMenuInputContact));

	playAnimation();
}
void PopDialogBoxFeedback::onExit(){
	CCLayer::onExit();
}
//反馈////////////////////////////////////////////////////////////////////////
void PopDialogBoxFeedback::onMenuFeedback(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		sQQ = pEBFeedbackQQ->getText();
		if (sContent.length()<=0)
		{
			showTipInfo("反馈内容不能为空");
		}
		else if (sQQ.length()<=0)
		{
			showTipInfo("QQ号码不能为空");
		}
		else
		{
			//连接服务器
			connectServer();
			connectSuccess();
		}
		
		//
	}
}
//输入回调
void PopDialogBoxFeedback::onTextFeild(CCObject* object, TextFiledEventType type){
    switch (type) {
        case TEXTFIELD_EVENT_ATTACH_WITH_IME:
        {
            float posY=CCDirector::sharedDirector()->getRunningScene()->getPositionY();
            if (posY==0) {
                 CCDirector::sharedDirector()->getRunningScene()->runAction(CCMoveTo::create(0.2, ccp(0,200)));
            }
        }
            break;
        case TEXTFIELD_EVENT_DETACH_WITH_IME:
        {
            float posY=CCDirector::sharedDirector()->getRunningScene()->getPositionY();
            if (posY!=0) {
                CCDirector::sharedDirector()->getRunningScene()->runAction(CCMoveTo::create(0.2, ccp(0,0)));
            }
        }
            break;
		case TEXTFIELD_EVENT_INSERT_TEXT:
		case TEXTFIELD_EVENT_DELETE_BACKWARD:
		{
			UITextField *pTextField = (UITextField*)object;
			sContent = UTF8ToGBK(pTextField->getStringValue());
		}
				break;
            
        default:
            break;
    }
}
//更新
void PopDialogBoxFeedback::update(float delta){
	gameSocket.updateSocketData(delta);
}
//////////////////////////////////////////////////////////////////////////
//网络消息
void PopDialogBoxFeedback::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_MB_SOCKET://socket连接成功
	{

	}
	break;
	case MDM_GP_USER_SERVICE://用户服务
	{
		onEventUserService(wSubCmdID, pDataBuffer, wDataSize);
		//移除loading
		this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
		gameSocket.Destroy(true);
	}
	break;
	default:
		CCLOG("other:%d   %d<<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
		break;
	}
}
//连接成功
void PopDialogBoxFeedback::connectSuccess(){
	CMD_GP_Feedback feedback;
	strcpy(feedback.szAccounts, DataModel::sharedDataModel()->sLogonAccount.c_str());
	strcpy(feedback.szContent, sContent.c_str());
	strcpy(feedback.szQQ, sQQ.c_str());
	strcpy(feedback.szTitle,"");

	gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_FEEDBACK, &feedback, sizeof(CMD_GP_Feedback));
}
//用户服务
void PopDialogBoxFeedback::onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_FEEDBACK://反馈
		onSubFeedback(pDataBuffer, wDataSize);
		break;
	default:
		CCLOG("sub:%d <<%s>>", wSubCmdID, __FUNCTION__);
		break;
	}
}
//反馈
void PopDialogBoxFeedback::onSubFeedback(void * pDataBuffer, unsigned short wDataSize){
	CMD_GP_FeedbackLog *pFeedback = (CMD_GP_FeedbackLog*)pDataBuffer;
	if (pFeedback->dwRet==0)
	{
		showTipInfo(GBKToUTF8(pFeedback->szDescribeString).c_str(), this);
	}
	else
	{
		showTipInfo(GBKToUTF8(pFeedback->szDescribeString).c_str());
	}
	
}
//关闭回调
void PopDialogBoxFeedback::onCloseTipInfo(CCLayer *pTipInfo){
	this->removeFromParentAndCleanup(true);
}