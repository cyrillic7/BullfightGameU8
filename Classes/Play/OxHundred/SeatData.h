
#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
class SeatData:public CCNode {
public:
	//所有筹码数字标签
	UILabelAtlas *pLAllJetton;
	//所有筹码文字“万”
	ImageView *pIFontWan;
	//用户筹码数字标签
	UILabelAtlas *pLUserJetton;
	//用户筹码文字“万”
	ImageView *pIUserFontWan;
	//未下注
	ImageView *pINotAddJetton;
	//结算数字
	UILabelAtlas *pLResult;
	//中心点
	CCPoint posCenter;
    
	//座位大小
	CCSize seatSize;
private:
	//所有筹码数
	long long allJettonCount;
public:
	SeatData();
	~SeatData();
	CREATE_FUNC(SeatData);
public:
	//重置
	void resetData();
	//设置所有筹码
	void setAllJettonByAdd(long long lValue);
	//隐藏所有筹码标签
	void hideAllJettonLabel();
	//设置用户筹码
	void setUserJetton(long long lValue);
	//隐藏用户筹码标签
	void hideUserJettonLabel();
	//设置结算
	void setResult(long long lResult);
};
