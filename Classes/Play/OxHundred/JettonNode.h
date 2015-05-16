/*
 * JettonNode.h
 *
 *  Created on: 2015年4月3日
 *      Author: xw007
 */
#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class JettonNode:public CCNode {
public:
	bool isReuse;
private:
	UIImageView *pIJetton;
	//移动速度
	float fMoveSpeed;
public:
	JettonNode();
	virtual ~JettonNode();
	CREATE_FUNC(JettonNode);
public:
	void resetData();
	//设置筹码类型并移动
	void setJettonTypeWithMove(long long lJettonValue,CCPoint posBegin,CCPoint posEnd);
	//
	void hideJetton();
};
