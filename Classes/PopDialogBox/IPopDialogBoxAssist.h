/*
 * PopDialogBoxVip.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

struct IPopDialogBoxAssist//
{
	virtual ~IPopDialogBoxAssist(){}
	//关闭跳转到商城
	virtual void onCloseViewToShop() = 0;
};
//关闭回调接口
struct IPopDialogBoxAssistCloseView
{
	virtual ~IPopDialogBoxAssistCloseView(){}
	//关闭窗口
	virtual void onCloseView() = 0;
};
//兑换
struct IPopDialogBoxExchange
{
	virtual ~IPopDialogBoxExchange(){}
	
	virtual void onExchangeNumWithContent(int type,std::string sContent) = 0;
};