/*
 * IHornMsgAssist.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

struct IHornMsgAssist//
{
	virtual ~IHornMsgAssist(){}
	//发送失败
	virtual void onSendFail(std::string content) = 0;
	//发送确定
	virtual void onSendSure(CCLayer *layer) = 0;
};