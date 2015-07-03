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
	int iBetArea;					//下注区域
	static int kMaxMoveJettonNum;		//最大移动筹码数
	static int kCurMoveJettonNum;		//当前移动筹码数
	long long llJettonScore;				//筹码数
private:
	ImageView *pIJetton;
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

	//设置筹码数、位置
	void setJettonTypeWithPos(long long lJettonValue, CCPoint posEnd);
	
	//庄家收币
	void moveJettonBankIn(CCPoint posEnd);
	//庄家收币回调
	void onMoveJettonBankIn(CCNode *node);

	//庄家出币
	void bankOutJetton(long long lJettonValue, CCPoint posBegin, CCPoint posEnd);
	//庄家出币回调
	void onBankOutJetton(CCNode *node);

	//玩家收币
	void moveJettonPlayerIn(CCPoint posEnd);
	//玩家收币回调
	void onMoveJettonPlayerIn(CCNode *node);
};
