/*
 * PopDialogBoxBank.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
#define MAX_QUICK_BUTTON_COUNT				6				//最大选择款项数
class PopDialogBoxBank: public PopDialogBox {
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
	UIImageView *pIVTitle;
	//创建密码容器
	UIPanel *pPanelCreatePassword;
	UITextField *pTFCreatePassword0;
	CCEditBox *pEBCreatePassword0;
	UITextField *pTFCreatePassword1;
	CCEditBox *pEBCreatePassword1;
	//输入密码容器
	UIPanel *pPanelInputPassword;
	UITextField *pTFInputPassword;
	CCEditBox *pEBInputPassword;
	//存取钱
	UIPanel *pPanelOperationMoney;
	//取出选项卡按键
	UIButton *pBTakeOut;
	//存入选项卡按键
	UIButton *pBDeposit;
	//我的存款/我的金币
	UILabel *pLBankMoney;
	//银行存款文字
	UIImageView *pIVInput;
	//取出金币文字
	UIImageView *pIVOutput;
	//存款/取款按键
	UIButton *pBOperationMoney;
	//存款/取款金币数
	UITextField *pTFInputMoney;
	CCEditBox *pEBInputMoney;
	//快捷选择款项按键
	UIButton *pBQuickSelectMoney[MAX_QUICK_BUTTON_COUNT];
	//快捷款项选择限制条件
	long long llQuickLimitNum[MAX_QUICK_BUTTON_COUNT];
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
	//初始化快捷选择款按键
	void initQuickSelectMoney();
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
	//快捷选择款项
	void onMenuQuickSelectMoney(CCObject *object, TouchEventType type);
	//忘记密码
	void onMenuForgetPassword(CCObject *object, TouchEventType type);
	//更新快捷款项选择键
	void updateQuickButton();
	//更新
	void update(float delta);
	//////////////////////////////////////////////////////////////////////////
	//连接成功
	void connectSuccess();
	//网络消息
	virtual void onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
	//用户服务
	void onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);
};
