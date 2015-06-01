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
#include "../Network/TCPSocket/TCPSocketControl.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class PopDialogBox: public CCLayer,public CStringAide {
public:
	UILayer * pUILayer;
	UIImageView *pWidgetBg;
	static std::string sSocketName;
public:
	PopDialogBox();
	~PopDialogBox();
    

	//菜单////////////////////////////////////////////////////////////////////////
	void menuBack(CCObject *object, TouchEventType type);

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){return true; }
	//播放放大动画
	void playAnimation();

	TCPSocket *getSocket(){ 
		CCLog("soceketname::%s <<%s>>",sSocketName.c_str(), __FUNCTION__);
		return TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(sSocketName); }
	//连接服务器
	void connectServer(std::string socketName);
	void setSocketName(std::string sName);
};
