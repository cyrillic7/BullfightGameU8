/*
 * PopDialogBoxOnLine.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once
#include "PopDialogBox.h"
#include "DataModel.h"
class PopDialogBoxOnLine: public PopDialogBox {
private:
	//在线列表
	UIListView *pLOnLine;
	float fPosX;
public:
	PopDialogBoxOnLine();
	~PopDialogBoxOnLine();
	CREATE_FUNC(PopDialogBoxOnLine);
private:
	virtual void onEnter();
	virtual void onExit();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	//设置在线用户信息
	void setOnLineUserInfo(bool isInsert,tagUserInfo userInfo);
	//移除自己
	void removSelf();
};
