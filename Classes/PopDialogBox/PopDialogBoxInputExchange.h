/*
 * PopDialogBoxInputExchange.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxInputExchange: public PopDialogBox {
private:
	CC_SYNTHESIZE(IPopDialogBoxExchange*, pIPopDialogBoxExchange, IPopDialogBoxExchange);
	//最大商品数
	long lMaxPropsNum;
	//购买数量
	long lBuyNum;
	//商品图片框
	UIImageView *pIVPropIcon;
	//道具名
	UILabel *pLExchangePropName;
	//购买类型
	UILabel *pLBuyType;
	//兑换说明
	UILabel *pLExchangeContent;
	
	UITextField *pTFInputCount;
	//最大数量
	UIButton *pBMaxCount;
public:
	PopDialogBoxInputExchange();
	~PopDialogBoxInputExchange();
	CREATE_FUNC(PopDialogBoxInputExchange);
public:
	//设置数据
	void setInputExchangeData(UseType eBuyType, const char* cPropName, const char* cPropImagePuth, long lMaxNum, const char* sInfoContent);
private:
	virtual void onEnter();
	virtual void onExit();
	//确定////////////////////////////////////////////////////////////////////////
	void onMenuSure(CCObject *object, TouchEventType type);
	//最大数量
	void onMenuMax(CCObject *object, TouchEventType type);
	//输入框回调
	void onTextFieldAccount(CCObject* obj, TextFiledEventType type);
	//更新总价
	void updateAllPice();
	//输入改变
	virtual void editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text);
};
