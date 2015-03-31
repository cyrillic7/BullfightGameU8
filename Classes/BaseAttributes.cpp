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
 	this->initStr();
}
void BaseAttributes::initStr(){
	CCString *tempStr = CCString::createWithFormat("data/str.plist");
	CCDictionary *mDic=CCDictionary::createWithContentsOfFile(tempStr->getCString());
	
	sexBoyName = dynamic_cast<CCString *>(mDic->objectForKey("strSexBoy"))->getCString();
	sexGirlName=dynamic_cast<CCString *>(mDic->objectForKey("strSexGirl"))->getCString();

	mDic->removeAllObjects();
	CC_SAFE_RELEASE_NULL(mDic);
}
