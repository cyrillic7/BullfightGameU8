/*
 * PopDialogBoxDial.h
 *	签到
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
#include "../Network/CMD_Server/CMD_LogonServer.h"
#include "PopDialogBoxLoading.h"
#include "PopDialogBoxTipInfo.h"
#include "../MTNotificationQueue/IHornMsgAssist.h"
class PopDialogBoxDial : public PopDialogBox, public IOutTime, public IPopAssistTipInfo, public IHornMsgAssist {
private:
	UIButton *pBStart;							//开始按键
	UIImageView *pIVPointer;					//转盘指针
	int dialIndex;								//指针奖励索引
	enum DialState
	{
		DIAL_FREE=0,			//转盘空闲
		DIAL_ING,				//旋转中
		DIAL_END,				//旋转结束
	};
	CC_SYNTHESIZE(DialState, eDialState, DialState);
	CCArmature *pAStartButton;//
	CCArmature *pAEnd;//
public:
	PopDialogBoxDial();
	~PopDialogBoxDial();
	CREATE_FUNC(PopDialogBoxDial);
private:
	virtual void onEnter();
	virtual void onExit();

	//连接服务器
	void connectDialServer();
	//开始按键
	void onMenuStart(CCObject *object, TouchEventType type);
	//超时回调
	virtual void onOutTime();
	//////////////////////////////////////////////////////////////////////////
	void update(float delta);
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//连接成功
	void connectSuccess();
	//用户服务
	void onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//转盘信息
	void onSubDial(void * pDataBuffer, unsigned short wDataSize);
	//////////////////////////////////////////////////////////////////////////
    //链接失败
    virtual void onError(const char* e);
    //关闭回调
    virtual void onCloseTipInfo(CCLayer *pTipInfo);
	//动画完成
	void dialAnimationFinsh();
	//动画结束回调
	void onAnimationEventOver(CCArmature *pArmature, MovementEventType movementType, const char *movementID);
	//动画帧事件回调
	void onAnimationEventFrame(CCBone *bone, const char *evt, int originFrameIndex, int currentFrameIndex);
	//显示奖励
	void showReward(float dt);
	//////////////////////////////////////////////////////////////////////////
	//失败
	virtual void onSendFail(std::string content){}
	//确定
	virtual void onSendSure(CCLayer *layer);
};
