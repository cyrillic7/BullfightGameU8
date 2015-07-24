/*
 * PopDialogBoxInputExchange.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxInputExchange.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../Tools/BaseAttributes.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxInputExchange::PopDialogBoxInputExchange()
	:pIPopDialogBoxExchange(NULL)
	, lBuyNum(0)
{
	
    
}
PopDialogBoxInputExchange::~PopDialogBoxInputExchange() {
	CCLOG("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxInputExchange::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxInputExchange.ExportJson)));
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
	//兑换说明
	pLExchangeContent = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelInfoContent"));
	pLExchangeContent->setText("");
	//购买类型
	pLBuyType = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelCount"));
	//输入数量
	pTFInputCount = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldCount")); 
	pTFInputCount->addEventListenerTextField(this, SEL_TextFieldEvent(&PopDialogBoxInputExchange::onTextFieldAccount));
	addEditBox(pTFInputCount, kEditBoxInputModeNumeric);
	//确定按键
	UIButton *pBSure = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonSure"));
	pBSure->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxInputExchange::onMenuSure));

	playAnimation();
}
void PopDialogBoxInputExchange::onExit(){
	CCLayer::onExit();
}
//设置数据
void PopDialogBoxInputExchange::setInputData(ExchangeType eTExchangeType, const char* cPropName, const char* cPropImagePuth, long lMaxNum, long long lPice){
	setExchangeType(eTExchangeType);
	
	lMaxPropsNum = lMaxNum;
	lBuyNum = lMaxNum;


	pLPropName->setText(cPropName);
	if (strcmp(cPropImagePuth,"")!=0)
	{
		addDownloadImage(pIVPropIcon, cPropImagePuth, CCPointZero, 1, 0, false);
	}
	//设置标题////////////////////////////////////////////////////////////////////////
	UILabel *pLTempTitle0 = static_cast<UILabel*>(pUILayer->getWidgetByName("Label_3"));
	UILabel *pLTempTitle1 = static_cast<UILabel*>(pUILayer->getWidgetByName("Label_4"));
	switch (eTExchangeType)
	{
	case PopDialogBox::EXCHANGE_PHONE_COST:
	{
		pLBuyType->setText("手机号码:");
		pLExchangeContent->setText("请确认手机号码输入无误!");
		pLTempTitle0->setText("使用");
		pLTempTitle1->setText("使用");
	}
			break;
	case PopDialogBox::EXCHNAGE_QQ:
	{
		pLBuyType->setText("QQ号码:");
		pLExchangeContent->setText("请确认QQ号码输入无误!");
		pLTempTitle0->setText("使用");
		pLTempTitle1->setText("使用");
	}
		break;
	default:
	{
		pLBuyType->setText("兑换数量:");
		pLExchangeContent->setText("");
		pLTempTitle0->setText("兑换");
		pLTempTitle1->setText("兑换");
	}
		break;
	}
	updateAllPice();
}
//更新总价
void PopDialogBoxInputExchange::updateAllPice(){
	CCEditBox *pEBInputCount = (CCEditBox *)pTFInputCount->getNodeByTag(TAG_INPUT_EDIT_BOX);
	switch (getExchangeType())
	{
	case PopDialogBox::EXCHNAGE_QQ:
	case PopDialogBox::EXCHANGE_PHONE_COST:
		break;
	default:
	{
		pEBInputCount->setText(CCString::createWithFormat("%ld", lBuyNum)->getCString());
	}
		
		break;
	}
	
}
//确定按键
void PopDialogBoxInputExchange::onMenuSure(CCObject *object, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		CCEditBox *pEBInputCount = (CCEditBox *)pTFInputCount->getNodeByTag(TAG_INPUT_EDIT_BOX);
		switch (getExchangeType())
		{
		case PopDialogBox::EXCHNAGE_QQ:
		{
			if (strcmp(pEBInputCount->getText(), "") == 0)
			{
				showTipInfo("QQ号不能为空!");
			}
			else
			{
				if (getIPopDialogBoxExchange())
				{
					getIPopDialogBoxExchange()->onExchangeNumWithContent(getExchangeType(), pEBInputCount->getText());
				}
				this->removeFromParentAndCleanup(true);
			}
		}
			break;
		case PopDialogBox::EXCHANGE_PHONE_COST:
		{
			if (strcmp(pEBInputCount->getText(), "") == 0)
			{
				showTipInfo("手机号码不能为空!");
			}
			else
			{
				if (getIPopDialogBoxExchange())
				{
					getIPopDialogBoxExchange()->onExchangeNumWithContent(getExchangeType(), pEBInputCount->getText());
				}
				this->removeFromParentAndCleanup(true);
			}
		}
			break;
		default:
		{
			if (lBuyNum <= 0)
			{
				showTipInfo(BaseAttributes::sharedAttributes()->sInputAuctionGoodsNum.c_str());
			}
			else
			{
				if (getIPopDialogBoxExchange())
				{
					getIPopDialogBoxExchange()->onExchangeNumWithContent(getExchangeType(), pEBInputCount->getText());
				}
				this->removeFromParentAndCleanup(true);
			}
		}
			break;
		}
		/*if (lBuyNum<=-10)
		{
			showTipInfo(BaseAttributes::sharedAttributes()->sInputAuctionGoodsNum.c_str());
		}
		else
		{
			if (getIPopDialogBoxExchange())
			{
				getIPopDialogBoxExchange()->onExchangeNumWithContent(lBuyNum+1,"测试");
			}
			this->removeFromParentAndCleanup(true);
		}*/
	}
		break;
	default:
		break;
	}
}
//输入框回调
void PopDialogBoxInputExchange::onTextFieldAccount(CCObject* obj, TextFiledEventType type){
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
void PopDialogBoxInputExchange::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
	lBuyNum = strtol(text.c_str(), NULL, 10);
	updateAllPice();
}