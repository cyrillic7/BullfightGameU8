/*
 * PopDialogBoxInputNum.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxInputNum.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../Tools/BaseAttributes.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxInputNum::PopDialogBoxInputNum()
	:isShowVipDiscount(true)
	
{
	
    
}
PopDialogBoxInputNum::~PopDialogBoxInputNum() {
	CCLOG("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxInputNum::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxBuyCount.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	//商品图片框
	pIVPropIcon = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImagePropIcon"));
	//道具名
	pLPropName = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelPropName"));
	//购买类型
	pLBuyType = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelCount"));
	//贷币图标
	pICurrencyIcon = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImagePiceIcon"));
	//贷币名称
	pLCurrencyName = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelPice"));
	//总价
	pAllPice = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelAllPice"));
	//vip折扣
	pLVipDiscount = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelVip"));
	pLVipDiscount->setVisible(isShowVipDiscount);
	pLVipDiscount->setEnabled(isShowVipDiscount);
	pLVipDiscount->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxInputNum::onMenuToVip));
	//输入数量
	pTFInputCount = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldCount")); 
	pTFInputCount->addEventListenerTextField(this, SEL_TextFieldEvent(&PopDialogBoxInputNum::onTextFieldAccount));
	addEditBox(pTFInputCount, kEditBoxInputModeNumeric);
	//确定按键
	UIButton *pBSure = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonSure"));
	pBSure->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxInputNum::onMenuSure));

	playAnimation();
}
void PopDialogBoxInputNum::onExit(){
	CCLayer::onExit();
}
//设置数据
void PopDialogBoxInputNum::setInputData(BuyType eBuyType, const char* cPropName, const char* cPropImagePuth, long lMaxNum, long long lPice, long vipDiscount, int vipLevel){
	lMaxPropsNum = lMaxNum;
	lBuyNum = lMaxNum;
	lPropPice = lPice;
	pLPropName->setText(cPropName);
	pLVipDiscount->setText(CCString::createWithFormat(" 开通VIP享%ld折 ",vipDiscount)->getCString());
	pLVipDiscount->setEnabled(vipLevel == 0);
	if (strcmp(cPropImagePuth,"")!=0)
	{
		addDownloadImage(pIVPropIcon, cPropImagePuth, CCPointZero, 1, 0, false);
	}
	
	switch (eBuyType)
	{
	case PopDialogBox::BUY_AUCTION:
	{
		pLBuyType->setText(BaseAttributes::sharedAttributes()->sBuyTypeAuction.c_str());
		pICurrencyIcon->loadTexture("u_u_info_gold_icon.png", UI_TEX_TYPE_PLIST);
		pLCurrencyName->setText(BaseAttributes::sharedAttributes()->sAllPice.c_str());
	}
		break;
	default:
		break;
	}
	updateAllPice();
}
//更新总价
void PopDialogBoxInputNum::updateAllPice(){
	CCEditBox *pEBInputCount = (CCEditBox *)pTFInputCount->getNodeByTag(TAG_INPUT_EDIT_BOX);
	pEBInputCount->setText(CCString::createWithFormat("%ld", lBuyNum)->getCString());
	pAllPice->setText(CCString::createWithFormat("%lld", lPropPice*lBuyNum)->getCString());
}
//确定按键
void PopDialogBoxInputNum::onMenuSure(CCObject *object, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		if (lBuyNum<=0)
		{
			showTipInfo(BaseAttributes::sharedAttributes()->sInputAuctionGoodsNum.c_str());
		}
		else
		{
			getIPopAssist()->onBuyNum(lBuyNum);
			this->removeFromParentAndCleanup(true);
		}
	}
		break;
	default:
		break;
	}
}
//跳转到vip
void PopDialogBoxInputNum::onMenuToVip(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		getIPopAssist()->onToVip();
	}
}
//输入框回调
void PopDialogBoxInputNum::onTextFieldAccount(CCObject* obj, TextFiledEventType type){
	switch (type)
	{
	case TEXTFIELD_EVENT_ATTACH_WITH_IME:
		//CCLOG("attach<<%s>>",__FUNCTION__);
		break;
	case TEXTFIELD_EVENT_INSERT_TEXT:
	case TEXTFIELD_EVENT_DETACH_WITH_IME:
	case TEXTFIELD_EVENT_DELETE_BACKWARD:
	{
		UITextField *pTF = (UITextField*)obj;
		lBuyNum = strtol(pTF->getStringValue(), NULL, 10);
		updateAllPice();
	}
	break;
	default:
		break;
	}
}
//输入改变
void PopDialogBoxInputNum::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
	lBuyNum = strtol(text.c_str(), NULL, 10);
	updateAllPice();
}