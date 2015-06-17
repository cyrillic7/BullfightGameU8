
#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
#include "../../Network/CMD_Server/Struct.h"
#include "../../Tools/CStringAide.h"
class ChairData:public CCNode ,public CStringAide{
public:
	UIButton *pBDonw;
	UILabel *pLUserName;
	UIImageView *pIVUserIcon;
public:
	ChairData();
	~ChairData();
	CREATE_FUNC(ChairData);
public:
	//用户坐下
	void setUserSitState(tagUserInfo userInfo,bool isSit);
};
