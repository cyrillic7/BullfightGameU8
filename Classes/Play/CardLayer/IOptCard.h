
#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

struct IOptCard//弹出扑克
{
	virtual ~IOptCard(){}
	//
	virtual void onUpCard() = 0;
};
