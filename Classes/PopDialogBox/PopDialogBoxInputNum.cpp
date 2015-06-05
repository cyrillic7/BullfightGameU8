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
{
	
    
}
PopDialogBoxInputNum::~PopDialogBoxInputNum() {
	CCLog("~ <<%s>>",__FUNCTION__);
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
	playAnimation();
}
void PopDialogBoxInputNum::onExit(){
	CCLayer::onExit();
}
//设置数据
void PopDialogBoxInputNum::setInputData(BuyType eBuyType, const char* cPropName, const char* cPropImagePuth, long lMaxNum, long long lPice){
	lMaxPropsNum = lMaxNum;
	lBuyNum = lMaxNum;
	lPropPice = lPice;
	pLPropName->setText(cPropName);
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
	pAllPice->setText(CCString::createWithFormat("%lld", lPropPice*lBuyNum)->getCString());
}