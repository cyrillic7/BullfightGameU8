/*
 * PopDialogBoxFeedback.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
#include "../PopDialogBox/PopDialogBoxTipInfo.h"
class PopDialogBoxFeedback: public PopDialogBox,public IPopAssistTipInfo {
private:
	//联系方式
	CCEditBox *pEBFeedbackQQ;
	//内容
	std::string sContent;
	//QQ
	std::string sQQ;
public:
	PopDialogBoxFeedback();
	~PopDialogBoxFeedback();
	CREATE_FUNC(PopDialogBoxFeedback);
private:
	virtual void onEnter();
	virtual void onExit();
	//反馈////////////////////////////////////////////////////////////////////////
	void onMenuFeedback(CCObject *object, TouchEventType type);
    //输入回调
    void onTextFeild(CCObject* object, TextFiledEventType type);

	//更新
	void update(float delta);
	//////////////////////////////////////////////////////////////////////////
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//连接成功
	void connectSuccess();
	//用户服务
	void onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//意见反馈
	void onSubFeedback(void * pDataBuffer, unsigned short wDataSize);


	//关闭回调
	virtual void onCloseTipInfo(CCLayer *pTipInfo);
};
