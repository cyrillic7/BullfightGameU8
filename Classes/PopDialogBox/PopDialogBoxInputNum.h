/*
 * PopDialogBoxInputNum.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxInputNum: public PopDialogBox {
private:
	//最大商品数
	long lMaxPropsNum;
    //单价
	long long lPropPice;
	//购买数量
	long lBuyNum;
	//道具名
	UILabel *pLPropName;
	//购买类型
	UILabel *pLBuyType;
	//代币图标
	UIImageView *pICurrencyIcon;
	//贷币名称
	UILabel *pLCurrencyName;
	//总价
	UILabel *pAllPice;
	UITextField *pTFInputCount;
public:
	PopDialogBoxInputNum();
	~PopDialogBoxInputNum();
	CREATE_FUNC(PopDialogBoxInputNum);
public:
	//设置数据
	void setInputData(BuyType eBuyType, const char* cPropName, const char* cPropImagePuth, long lMaxNum, long long lPice);
private:
	virtual void onEnter();
	virtual void onExit();
	//确定////////////////////////////////////////////////////////////////////////
	void onMenuSure(CCObject *object, TouchEventType type);
	//更新总价
	void updateAllPice();
};
