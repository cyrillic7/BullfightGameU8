/*
 * BaseAttributes.h
 *  基础属性
 *  Created on: 2015年3月16日
 *      Author: 恒
 */

#ifndef BASEATTRIBUTES_H_
#define BASEATTRIBUTES_H_

#include "cocos2d.h"
USING_NS_CC;
//#define GOODS_COUNT 4
class BaseAttributes :public CCObject{
public:
	std::string sexBoyName;
	std::string sexGirlName;
	//物品（油桶、炸药包）////////////////////////////////////////////////////////////////////////
	//int goodsLife[GOODS_COUNT];
	//int goodsAttack[GOODS_COUNT];
	//float goodsAttackRange[GOODS_COUNT];
public:
	static BaseAttributes * sharedAttributes();
	virtual ~BaseAttributes();
	void initBaseAttrobutes();
private:
	void initStr();
};

#endif /* BASEATTRIBUTES_H_ */
