/*
 * PopDialogBoxTrend.h
 *百人牛牛趋势图
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once
#include "PopDialogBox.h"
#include "DataModel.h"
class PopDialogBoxTrend: public PopDialogBox {
private:
	//趋势图
	UIPanel *pPTrend;
	//趋势列表
	UIListView *pLGameRecord;
public:
	PopDialogBoxTrend();
	~PopDialogBoxTrend();
	CREATE_FUNC(PopDialogBoxTrend);
private:
	virtual void onEnter();
	virtual void onExit();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	//移除自己
	void removSelf();
	//刷新列表
	void updateTrendList(CCObject *obj);
};
