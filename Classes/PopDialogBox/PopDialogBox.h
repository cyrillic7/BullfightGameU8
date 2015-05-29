/*
 * PopDialogBox.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../Tools/GameConfig.h"
#include "../Tools/CStringAide.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class PopDialogBox: public CCLayer,public CStringAide {
public:
	UILayer * pUILayer;
	UIImageView *pWidgetBg;
public:
	PopDialogBox();
	~PopDialogBox();
    

	//菜单////////////////////////////////////////////////////////////////////////
	void menuBack(CCObject *object, TouchEventType type);

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){return true; }
	//播放放大动画
	void playAnimation();

	
};
