//
//  LobbyHornLayer.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//
#include "LobbyHornLayer.h"
#include "../Tools/DataModel.h"
#include "GameLobbyScene.h"
#include "../MTNotificationQueue/LobbyMsgHandler.h"
LobbyHornLayer::LobbyHornLayer()
{
}
LobbyHornLayer::~LobbyHornLayer(){
	CCLOG("~ <<%s>>", __FUNCTION__);
}
void LobbyHornLayer::onEnter(){
	CCLayer::onEnter();
	UILayer *m_pWidget = UILayer::create();
	this->addChild(m_pWidget);


	UILayout *pWidget = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UILobbyHorn.ExportJson)));
	m_pWidget->addWidget(pWidget);

	pIVMsg  = static_cast<UIImageView*>(m_pWidget->getWidgetByName("ImageMsg"));
	//延时0.2秒弹出消息框
	CCCallFunc *call = CCCallFunc::create(this, SEL_CallFunc(&LobbyHornLayer::onShowMsg));
	CCSequence *seq = CCSequence::create(CCDelayTime::create(0.2), call, NULL);
	pIVMsg->runAction(seq);
	//输入框
	pTFHornMsgContent = static_cast<UITextField*>(m_pWidget->getWidgetByName("TextFieldInput"));
	addEditBox(pTFHornMsgContent, kEditBoxInputModeSingleLine);
	//发送消息按键
	UIImageView *pIVSendMsg = static_cast<UIImageView*>(m_pWidget->getWidgetByName("ImageSendHornMsg"));
	pIVSendMsg->addTouchEventListener(this, SEL_TouchEvent(&LobbyHornLayer::onMenuSendMsg));;
	//喇叭消息滚动容器
	pSVMsg = static_cast<UIScrollView*>(m_pWidget->getWidgetByName("ScrollViewMsg"));
	setScrollViewData();
	//重设置下，否则会被覆盖事件
	this->setTouchEnabled(true);
	this->setTouchPriority(0);
	this->setTouchMode(kCCTouchesOneByOne);
}
void LobbyHornLayer::onExit(){
   
	CCLayer::onExit();
}
//菜单发送消息
void LobbyHornLayer::onMenuSendMsg(CCObject* pSender, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		CCEditBox *pEBInputMsgContent = (CCEditBox*)pTFHornMsgContent->getNodeByTag(TAG_INPUT_EDIT_BOX);
		if (strcmp(pEBInputMsgContent->getText(), "") == 0)
		{
			showTipInfo("内容不能为空");
		}
		else
		{
			CMD_GL_Laba HornMsg;
			
			HornMsg.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;
			strcmp(HornMsg.szNickName,DataModel::sharedDataModel()->userInfo->szNickName);
			strcmp(HornMsg.szLabaText, pEBInputMsgContent->getText());
			//lstrcpyn(HornMsg.szNickName, pGlobalUserData->szNickName, CountArray(HornMsg.szNickName));
			//lstrcpyn(HornMsg.szLabaText, sHornMsg, CountArray(HornMsg.szLabaText));

			 LobbyMsgHandler::sharedLobbyMsgHandler()->gameSocket.SendData(MDM_GL_C_DATA, SUB_GL_C_LABA, &HornMsg, sizeof(HornMsg));
			//LobbyMsgHandler::sharedLobbyMsgHandler()->gameSocket.SendData();
		}
	}
}
bool LobbyHornLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	CCPoint touchPoint = this->convertTouchToNodeSpace(pTouch);
	CCRect rect = CCRect(pIVMsg->getPositionX() - pIVMsg->getSize().width/2, pIVMsg->getPositionY(), pIVMsg->getSize().width, pIVMsg->getSize().height);
	if (!rect.containsPoint(touchPoint))
	{
		CCEaseExponentialIn  *out = CCEaseExponentialIn::create(CCMoveTo::create(0.2, ccp(pIVMsg->getPositionX(), -pIVMsg->getSize().height)));
		CCCallFunc *call = CCCallFunc::create(this, SEL_CallFunc(&LobbyHornLayer::removSelf));
		CCSequence *seq = CCSequence::create(out, call, NULL);
		pIVMsg->runAction(seq);
	}
	else
	{
		return false;
	}
	return true;
}
//移除自己
void LobbyHornLayer::removSelf(){
	((GameLobbyScene*)this->getParent())->showHorn();
	this->removeFromParentAndCleanup(true);
}
//显示消息框
void LobbyHornLayer::onShowMsg(){
	pIVMsg->stopAllActions();
	float moveY = DataModel::sharedDataModel()->deviceSize.height*0.02;
	CCEaseExponentialOut  *out = CCEaseExponentialOut::create(CCMoveTo::create(0.2, ccp(pIVMsg->getPositionX(),moveY)));
	CCSequence *seq = CCSequence::create(out, NULL);
	pIVMsg->runAction(seq);
}
//隐藏消息框
void LobbyHornLayer::hideMsg(){
}
//设置滚动容器数据
void LobbyHornLayer::setScrollViewData(){
	for (int i = 0; i < 10; i++)
	{
		std::string sTemp = "测试数据:";
		if (i%2==0)
		{
			sTemp += "多天呆要要要要要要大在在一一在在在在在在在在在在在在在在在在要要要要要要要要本年数据";
		}
		sContentList.push_back(sTemp);
	}
	int height = 0;
	for (int i = 0; i<sContentList.size(); i++)
	{
		UILabel *pLContent = createMsgContent(sContentList[i]);
		height += pLContent->getSize().height;
		pLContent->setPosition(ccp(0,height));
		pSVMsg->addChild(pLContent);
		pSVMsg->setInnerContainerSize(CCSize(pLContent->getContentSize().width, height));
	}
	pSVMsg->scrollToBottom(0,false);
}
//创建消息内容
UILabel *LobbyHornLayer::createMsgContent(std::string content){
	UILabel *pLMsgContent = UILabel::create();
	pLMsgContent->setFontSize(20);
	pLMsgContent->ignoreContentAdaptWithSize(true);
	pLMsgContent->setTextAreaSize(CCSize(pSVMsg->getSize().width, 0));
	pLMsgContent->setText(content.c_str());
	return pLMsgContent;
}
//添加EditBox
void LobbyHornLayer::addEditBox(UITextField *pTextField, EditBoxInputMode eInputMode){
	pTextField->setTouchEnabled(false);
	//CCEditBox* m_pEditName = CCEditBox::create(pTextField->getContentSize(), CCScale9Sprite::createWithSpriteFrameName("u_info_change_bg.png"));
	CCEditBox* pEditBox = CCEditBox::create(pTextField->getSize(), CCScale9Sprite::create("res/u_input_horn.png"));
	pEditBox->setPosition(CCPointZero);
	pEditBox->setFontSize(pTextField->getFontSize());
	pEditBox->setFontColor(pTextField->getColor());//设置文字颜色


	pEditBox->setPlaceHolder(pTextField->getPlaceHolder());//占位符
	pEditBox->setPlaceholderFontSize(pTextField->getFontSize());

	if (pTextField->getMaxLength() > 0) {
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
void LobbyHornLayer::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
	CCLOG("editBox %p DidBegin !", editBox);
}

void LobbyHornLayer::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
	CCLOG("editBox %p DidEnd !", editBox);
}

void LobbyHornLayer::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
	CCLOG("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void LobbyHornLayer::editBoxReturn(CCEditBox* editBox)
{
	CCLOG("editBox %p was returned !", editBox);
}
////显示提示语
void LobbyHornLayer::showTipInfo(const char* sInfo){
	PopDialogBoxTipInfo *pTipInfo = PopDialogBoxTipInfo::create();
	this->addChild(pTipInfo, 10);
	pTipInfo->setTipInfoContent(sInfo);
}