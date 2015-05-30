/*
 * BaseAttributes.h
 *  基础属性
 *  Created on: 2015年3月16日
 *      Author: 恒
 */

#ifndef BASEATTRIBUTES_H_
#define BASEATTRIBUTES_H_

#include "cocos2d.h"
#include "GameConfig.h"
USING_NS_CC;
//#define GOODS_COUNT 4

class BaseAttributes :public CCObject{
public:
	std::string sSexBoyName;
	std::string sSexGirlName;
	std::string sGameFree;
	std::string sGameStart;
	std::string sGameEnd;
	std::string sLoading;

	int iCardOffsetX[MAX_PLAYER];
	int iCardOffsetY[MAX_PLAYER];
	int iCardOffsetSpace[MAX_PLAYER];
public:
	static BaseAttributes * sharedAttributes();
	virtual ~BaseAttributes();
	void initBaseAttrobutes();
	//扑克偏移
	void initCardOffset();
private:
	void initStr();
};

#endif /* BASEATTRIBUTES_H_ */
