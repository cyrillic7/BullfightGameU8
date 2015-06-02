/*
 * PopDialogBoxBank.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
#include "../MTNotificationQueue/MessageQueue.h"
#define MAX_SELECT_MONEY_COUNT				6				//最大选择款项数
class PopDialogBoxBank: public PopDialogBox,public MessageQueue {
private:
	enum BankState
	{
		BANK_STATE_CREATE = 0,				//创建密码
		BANK_STATE_ENTER,					//输入密码
		BANK_STATE_GET,						//获取银行信息
		BANK_STATE_TAKE_OUT,				//取钱
		BANK_STATE_SAVE,					//存钱
	};
	CC_SYNTHESIZE(BankState, bankState, BankState);
	//标题
	UILabel *pLTitle0;
	UILabel *pLTitle1;
	//创建密码容器
	UIPanel *pPanelCreatePassword;
	UITextField *pTFCreatePassword0;
	UITextField *pTFCreatePassword1;
	//输入密码容器
	UIPanel *pPanelInputPassword;
	UITextField *pTFInputPassword;
	//存取钱
	UIPanel *pPanelOperationMoney;
	//取出选项卡按键
	UIButton *pBTakeOut;
	//存入选项卡按键
	UIButton *pBDeposit;
	//我的存款/我的金币
	UILabel *pLBankMoney;
	//银行存款文字
	UILabel *pLInput;
	//取出金币文字
	UILabel *pLOutput;
	//存款/取款按键
	UIButton *pBOperationMoney;
	//存款/取款金币数
	UITextField *pTFInputMoney;
	//选择款项按键
	UIButton *pBSelectMoney[MAX_SELECT_MONEY_COUNT];
	bool isGetBankInfo;
	//临时密码
	std::string sTempPassword;
public:
	PopDialogBoxBank();
	~PopDialogBoxBank();
	CREATE_FUNC(PopDialogBoxBank);
private:
	virtual void onEnter();
	virtual void onExit();
	//初始化选择款按键
	void initSelectMoneyButton();
	//设置标题
	void setTitle(const char * sTitle);
	//创建密码菜单////////////////////////////////////////////////////////////////////////
	void onMenuCreatePassword(CCObject *object, TouchEventType type);
	//输入密码菜单
	void onMenuInputPassword(CCObject *object, TouchEventType type);
	//切换 取出/存入
	void onMenuChangeOperationType(CCObject *object, TouchEventType type);
	//存款/取款按键
	void onMenuOperationMoney(CCObject *object, TouchEventType type);
	//设置父结节是否读取网络消息
	void setParentReadMessage(bool isRead);
	//更新
	void update(float delta);
	//////////////////////////////////////////////////////////////////////////
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//用户服务
	void onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
};
