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
