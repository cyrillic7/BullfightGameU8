/*
 * BaseAttributes.cpp
 *
 *  Created on: 2015年3月16日
 *      Author: 恒
 */

#include "BaseAttributes.h"
#include "GameConfig.h"
static BaseAttributes *_sharedAttributes = NULL;
BaseAttributes *  BaseAttributes::sharedAttributes(){
	if (!_sharedAttributes) {
		_sharedAttributes = new BaseAttributes();
		_sharedAttributes->initBaseAttrobutes();
	}
	return _sharedAttributes;
}
BaseAttributes::~BaseAttributes(){
	_sharedAttributes = NULL;
	CCLog("~ <<%s>>", __FUNCTION__);
}
void BaseAttributes::initBaseAttrobutes(){
 	this->initGoodsProperty();
}
void BaseAttributes::initGoodsProperty(){
	CCString *tempStr = CCString::createWithFormat("data/goodsProperty.plist");;
	/*if (DEBUG_TEST) {
		if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		{
			tempStr = CCString::createWithFormat("/sdcard/data/DevilsComing2/data/goodsProperty.plist");
		}
	}*/
	CCArray *goodsArray = CCArray::createWithContentsOfFile(tempStr->getCString());
	for (int i = 0; i < goodsArray->count(); i++) {
		CCDictionary* mDic = dynamic_cast<CCDictionary *>(goodsArray->objectAtIndex(i));
		goodsLife[i] = dynamic_cast<CCString *>(mDic->objectForKey("life"))->intValue();
		goodsAttack[i] = dynamic_cast<CCString *>(mDic->objectForKey("attackValue"))->intValue();
		goodsAttackRange[i] = dynamic_cast<CCString *>(mDic->objectForKey("attackRange"))->floatValue();
	}
	goodsArray->removeAllObjects();
	CC_SAFE_RELEASE_NULL(goodsArray);
}
