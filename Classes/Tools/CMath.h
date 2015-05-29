#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class CMath
{
    
public:
	CMath();
	~CMath();
	//坐标点转换
	CCPoint designResolutionToFrame(CCPoint designPos);
	//筹码转贷币
	std::string formatNumber(std::string strNum);
};

