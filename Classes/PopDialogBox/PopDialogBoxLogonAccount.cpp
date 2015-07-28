/*
 * PopDialogBoxLogonAccount.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxLogonAccount.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../Tools/Tools.h"
#include "../LogonScene/LogonScene.h"
#include "PopDialogBoxForgetPassword.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxLogonAccount::PopDialogBoxLogonAccount()
{
	DataModel::sharedDataModel()->sLogonAccount = Tools::getStringByRMS(RMS_LOGON_ACCOUNT);
	DataModel::sharedDataModel()->sLogonPassword = Tools::getStringByRMS(RMS_LOGON_PASSWORD);
}
PopDialogBoxLogonAccount::~PopDialogBoxLogonAccount() {
	CCLOG("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxLogonAccount::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxLogonAccount.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	//获取背景图片容器
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	//设置初始大小
	pWidgetBg->setScale(0.8);
	//绑定关闭按键
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	//找回密码
	UILabel *pLRetrievePwd = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelRetrievePwd"));
	pLRetrievePwd->addTouchEventListener(this, toucheventselector(PopDialogBoxLogonAccount::onMenuRetrievePwd));
	//绑定登录按键
	backButton=static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonLogonAccount"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBoxLogonAccount::onMenuLogon));
	//绑定帐号文本输入框
	pTAccount=static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldAccount"));
	pTAccount->addEventListenerTextField(this,SEL_TextFieldEvent(&PopDialogBoxLogonAccount::onTextFieldAccount));
	pTAccount->setText(DataModel::sharedDataModel()->sLogonAccount);
	addEditBox(pTAccount, kEditBoxInputModeSingleLine);
	//绑定密码文本输入框
	pTPassword=static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldPassword"));
	pTPassword->addEventListenerTextField(this,SEL_TextFieldEvent(&PopDialogBoxLogonAccount::onTextFieldAccount));
	pTPassword->setText(DataModel::sharedDataModel()->sLogonPassword);
	addEditBox(pTPassword, kEditBoxInputModeSingleLine);
	//更多帐号
	UIButton *pBMoreAccount = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonMoreAccount"));
	pBMoreAccount->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxLogonAccount::onMenuMoreAccount));
	//更多帐号列表
	pLVMoreAccount = static_cast<UIListView*>(pUILayer->getWidgetByName("ListViewMoreAccount"));
	pLVMoreAccount->setEnabled(false);
	pLVMoreAccount->setItemModel(pLVMoreAccount->getItem(0));
	pLVMoreAccount->removeAllItems();

	updateListViewMoreAccount();
	//播放显示动画
	playAnimation();
}
void PopDialogBoxLogonAccount::onExit(){
	CCLayer::onExit();
}
void PopDialogBoxLogonAccount::onTextFieldAccount(CCObject* obj, TextFiledEventType type){
	switch (type)
	{
	case TEXTFIELD_EVENT_ATTACH_WITH_IME:
		//CCLOG("attach<<%s>>",__FUNCTION__);
		break;
	case TEXTFIELD_EVENT_INSERT_TEXT:
	case TEXTFIELD_EVENT_DETACH_WITH_IME:
	case TEXTFIELD_EVENT_DELETE_BACKWARD:
		{
			UITextField *pTF=(UITextField*)obj;
			if (strcmp(pTF->getName(),"TextFieldAccount")==0)
			{
				DataModel::sharedDataModel()->sLogonAccount=pTF->getStringValue();
			}else if(strcmp(pTF->getName(),"TextFieldPassword")==0)
			{
				DataModel::sharedDataModel()->sLogonPassword=pTF->getStringValue();
			}
		}
		CCLOG("detach<<%s>>",__FUNCTION__);
		break;
	default:
		break;
	}
}
void PopDialogBoxLogonAccount::onMenuLogon(CCObject *object, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			CCEditBox *pEBAccount = (CCEditBox*)pTAccount->getNodeByTag(TAG_INPUT_EDIT_BOX);
			CCEditBox *pEBPassword = (CCEditBox*)pTPassword->getNodeByTag(TAG_INPUT_EDIT_BOX);
			if (strcmp(pEBAccount->getText(), "") == 0 || strcmp(pEBPassword->getText(), "") == 0)
			{
				showTipInfo(" 帐号、密码不能为空 ");
			}else
			{
				DataModel::sharedDataModel()->sLogonAccount = pEBAccount->getText();
				DataModel::sharedDataModel()->sLogonPassword = pEBPassword->getText();

				((LogonScene*)this->getParent())->logonGameByAccount(0);
			}
		}
		break;
	default:
		break;
	}
}
//更多帐号
void PopDialogBoxLogonAccount::onMenuMoreAccount(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		showMoreAccount(true);
	}
}
//选择帐号
void PopDialogBoxLogonAccount::onMenuSelectAccount(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		UIButton *pBTemp = (UIButton*)object;
		MoreAccount moreAccount = DataModel::sharedDataModel()->vecMoreAccount[pBTemp->getTag()];
		
		CCEditBox *pEBAccount = (CCEditBox*)pTAccount->getNodeByTag(TAG_INPUT_EDIT_BOX);
		CCEditBox *pEBPassword = (CCEditBox*)pTPassword->getNodeByTag(TAG_INPUT_EDIT_BOX);

		pEBAccount->setText(moreAccount.userAccount.c_str());
		pEBPassword->setText(moreAccount.userPwd.c_str());
		
		showMoreAccount(false);
	}
}
//删除更多帐号
void PopDialogBoxLogonAccount::onMenuDeleteAccount(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		UIButton *pBTemp = (UIButton*)object;
		int iTag = pBTemp->getTag();
		std::vector<MoreAccount>::iterator it = DataModel::sharedDataModel()->vecMoreAccount.begin() + iTag;
		DataModel::sharedDataModel()->vecMoreAccount.erase(it);
		
		pLVMoreAccount->removeItem(iTag);
		//更新tag
		for (int i = iTag; i < DataModel::sharedDataModel()->vecMoreAccount.size(); i++)
		{
			//容器单元
			UIPanel *pPanelCell = static_cast<UIPanel*>(pLVMoreAccount->getItem(i));
			pPanelCell->setTag(i);
			//删除
			UIButton *pBDelete = static_cast<UIButton*>(pLVMoreAccount->getItem(i)->getChildByName("ButtonDelete"));
			pBDelete->setTag(i);
		}
		((LogonScene*)this->getParent())->saveAccount();
	}
}
//显示更多帐号
void PopDialogBoxLogonAccount::showMoreAccount(bool isShow){
	CCEditBox *pEBAccount = (CCEditBox*)pTAccount->getNodeByTag(TAG_INPUT_EDIT_BOX);
	CCEditBox *pEBPassword = (CCEditBox*)pTPassword->getNodeByTag(TAG_INPUT_EDIT_BOX);
	//登录按键
	UIButton *pBLogon = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonLogonAccount"));
	//找回密码
	UILabel *pLRetrievePwd = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelRetrievePwd"));
	if (isShow)
	{
		pLVMoreAccount->setEnabled(true);

		
		pEBAccount->setTouchEnabled(false);
		pEBPassword->setTouchEnabled(false);
		pBLogon->setTouchEnabled(false);
		pLRetrievePwd->setTouchEnabled(false);
		//重设置下，否则会被覆盖事件
		this->setTouchEnabled(true);
		this->setTouchPriority(0);
		this->setTouchMode(kCCTouchesOneByOne);
	}
	else
	{
		pEBAccount->setTouchEnabled(true);
		pEBPassword->setTouchEnabled(true);
		pBLogon->setTouchEnabled(true);
		pLRetrievePwd->setTouchEnabled(true);

		pLVMoreAccount->setEnabled(false);
		this->setTouchEnabled(false);
	}
}
bool PopDialogBoxLogonAccount::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	CCPoint touchPoint = this->convertTouchToNodeSpace(pTouch);
	CCRect rect = CCRect(pLVMoreAccount->getWorldPosition().x, pLVMoreAccount->getWorldPosition().y, pLVMoreAccount->getSize().width, pLVMoreAccount->getSize().height);
	if (rect.containsPoint(touchPoint))
	{
		return false;
	}
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	CCRect rectClose = CCRect(backButton->getWorldPosition().x - backButton->getSize().width/2,
		backButton->getWorldPosition().y - backButton->getSize().height/2,
		backButton->getSize().width, 
		backButton->getSize().height);
	if (rectClose.containsPoint(touchPoint))
	{
		return false;
	}
	showMoreAccount(false);
	return true;
}
//找回密码
void PopDialogBoxLogonAccount::onMenuRetrievePwd(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		PopDialogBoxForgetPassword *pFPwd = PopDialogBoxForgetPassword::create();
		((LogonScene*)this->getParent())->isReadMessage = false;
		((LogonScene*)this->getParent())->addChild(pFPwd);
	}
}
//输入框回调
void PopDialogBoxLogonAccount::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
	CCEditBox *pEBPassword = (CCEditBox*)pTPassword->getNodeByTag(TAG_INPUT_EDIT_BOX);
	if (editBox==pEBPassword)
	{
		pEBPassword->setText("");
	}
}
void PopDialogBoxLogonAccount::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
	CCEditBox *pEBPassword = (CCEditBox*)pTPassword->getNodeByTag(TAG_INPUT_EDIT_BOX);
	if (editBox == pEBPassword)
	{
		if(strcmp(pEBPassword->getText(),"")==0)
		{
			pEBPassword->setText(DataModel::sharedDataModel()->sLogonPassword.c_str());
		}
	}
}
void PopDialogBoxLogonAccount::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text){
 	/*if (isAllChniese(text))
 	{
		editBox->setText(sTempAccount.c_str());
	}
	else
	{
		sTempAccount = text;
	}*/
}

//更新更多帐号列表
void PopDialogBoxLogonAccount::updateListViewMoreAccount(){
	UIListView *pLVTemp = pLVMoreAccount;
	pLVTemp->removeAllItems();

	int tempSize = DataModel::sharedDataModel()->vecMoreAccount.size();
	if (tempSize == 0)
	{
		return;
	}



	for (int i = 0; i < tempSize; i++)
	{
		int inserterPos = pLVTemp->getItems()->count();
		pLVTemp->insertDefaultItem(inserterPos);
		//容器单元
		UIPanel *pPanelCell = static_cast<UIPanel*>(pLVTemp->getItem(inserterPos));
		pPanelCell->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxLogonAccount::onMenuSelectAccount));
		pPanelCell->setTag(i);
		//帐号
		UILabel *pGoodsName = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("LabelAccountCell"));
		pGoodsName->setText(DataModel::sharedDataModel()->vecMoreAccount[i].userAccount.c_str());
		//删除
		UIButton *pBDelete = static_cast<UIButton*>(pLVTemp->getItem(inserterPos)->getChildByName("ButtonDelete"));
		pBDelete->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxLogonAccount::onMenuDeleteAccount));
		pBDelete->setTag(i);
		
	}
}