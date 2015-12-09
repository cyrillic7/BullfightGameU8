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
class PopDialogBoxDial : public PopDialogBox,public IOutTime,public IPopAssistTipInfo {
private:
	enum SignState
	{
		SIGN_LIST = 0,			//签到列表
		SIGN_ING				//签到
	};
	CC_SYNTHESIZE(SignState, eSignState, SignState);
    //签到奖励列表
	UIListView *pLVSign;
	//签到信息
	std::vector <CMD_GP_SignInTask> vecSignInfo;
	//是否签到
	bool isSign;
	//签到按键
	UIButton *pBSign;
	//签到奖励金币
	UILabel *pLSignRewardGold;
	//签到天数信息
	UILabel *pLSignDayInfo;
public:
	PopDialogBoxDial();
	~PopDialogBoxDial();
	CREATE_FUNC(PopDialogBoxDial);
private:
	virtual void onEnter();
	virtual void onExit();

	//连接服务器
	void connectSignServer();
	//签到按键
	void onMenuSign(CCObject *object, TouchEventType type);
	//超时回调
	virtual void onOutTime();
	//更新拍卖信息列表
	void updateListSignInfo();
	//更新签到天数
	void updateSignDayPanel(int iCurSignDay);
	//////////////////////////////////////////////////////////////////////////
	void update(float delta);
	//TCPSocket *getSocket(){ return TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_SIGN); }
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//连接成功
	void connectSuccess();
	//用户服务
	void onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//签到信息
	void onSubSignInfo(void * pDataBuffer, unsigned short wDataSize);
	//签到
	void onSubSignIn(void * pDataBuffer, unsigned short wDataSize);

	//////////////////////////////////////////////////////////////////////////
	//保存签到记录
	void saveSignRecord();
    //链接失败
    virtual void onError(const char* e);
    //关闭回调
    virtual void onCloseTipInfo(CCLayer *pTipInfo);
};
