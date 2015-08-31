/*
 * PopDialogBoxMsg.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxMsg: public PopDialogBox {
private:
	enum MsgType
	{
		MSG_SYSTEM=1,				//系统消息
		MSG_MY,						//我的消息
	};
	CC_SYNTHESIZE(MsgType, eMsgType, MsgType);
	//游标
	//UIImageView *pIVCursor;
	//消息名称
	//UILabel *pLMsgName;
	//没有消息
	UIImageView *pIVNothing;
	//消息列表
	UIListView *pLVMsgList;
	//详细消息返回键
	UIButton *pBBackMsg;
	//详细内容
	UIScrollView *pSVMsgContent;
	//标题
	//UIImageView *pIVTitle;
	//内容标题
	UIImageView *pIVTitleContent;
	//系统消息按键
	UIButton *pBSystemMsg;
	//我的消息
	UIButton *pBSystemMy;
public:
	PopDialogBoxMsg();
	~PopDialogBoxMsg();
	CREATE_FUNC(PopDialogBoxMsg);
private:
	virtual void onEnter();
	virtual void onExit();
	//消息按键回调
	void onMenuMsg(CCObject *object, TouchEventType type);
	//消息返回按键回调
	void onMenuBackMsg(CCObject *object, TouchEventType type);
	//选择消息项
	void onMenuSelectMsgItem(CCObject *object, TouchEventType type);
	//更新消息列表
	void updateListMsg(std::vector<std::string> qMsg);
};
