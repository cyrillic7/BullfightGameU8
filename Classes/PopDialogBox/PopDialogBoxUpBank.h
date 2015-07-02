/*
 * PopDialogBoxUpBank.h
 *庄家列表
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
#include "../MainScene/MainSceneOxHundred.h"
class PopDialogBoxUpBank: public PopDialogBox {
private:
    
	//庄家列表
	UIListView *pLBank;
public:
	PopDialogBoxUpBank();
	~PopDialogBoxUpBank();
	CREATE_FUNC(PopDialogBoxUpBank);

	//更新上庄状态
	void updateUpBankState();
private:
	virtual void onEnter();
	virtual void onExit();
	void initListBank();
	void insertBank(bool isInsert,int index,tagApplyUser applyUser);
	//申请庄家////////////////////////////////////////////////////////////////////////
	void onMenuApplyBanker(CCObject *object, TouchEventType type);
	//刷新列表
	void updateBankList(CCObject *obj);
	//更新内容
	void updateListContent();
};
