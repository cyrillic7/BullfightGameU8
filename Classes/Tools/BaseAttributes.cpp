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
	this->initCardOffset();
}
void BaseAttributes::initStr(){
	CCString *tempStr = CCString::createWithFormat("data/str.plist");
	CCDictionary *mDic=CCDictionary::createWithContentsOfFile(tempStr->getCString());
	
	sSexBoyName = dynamic_cast<CCString *>(mDic->objectForKey("strSexBoy"))->getCString();
	sSexGirlName=dynamic_cast<CCString *>(mDic->objectForKey("strSexGirl"))->getCString();
	sGameFree = dynamic_cast<CCString *>(mDic->objectForKey("strGameFree"))->getCString();
	sGameStart = dynamic_cast<CCString *>(mDic->objectForKey("strGameStart"))->getCString();
 	sGameEnd = dynamic_cast<CCString *>(mDic->objectForKey("strGameEnd"))->getCString();
	sLoading = dynamic_cast<CCString *>(mDic->objectForKey("strLoading"))->getCString();
	sOutTime = dynamic_cast<CCString *>(mDic->objectForKey("strOutTime"))->getCString();
	sWaitCodeing = dynamic_cast<CCString *>(mDic->objectForKey("strWaitCodeing"))->getCString();
	sPasswordEmpty = dynamic_cast<CCString *>(mDic->objectForKey("strPasswordEmpty"))->getCString();
	sPasswordInconsistent = dynamic_cast<CCString *>(mDic->objectForKey("strPasswordInconsistent"))->getCString();
	sInsurePasswordLeng = dynamic_cast<CCString *>(mDic->objectForKey("strInsurePasswordLeng"))->getCString();
	sInsurePasswordNum = dynamic_cast<CCString *>(mDic->objectForKey("strInsurePasswordNum"))->getCString();
	sSaveLimit = dynamic_cast<CCString *>(mDic->objectForKey("strSaveLimit"))->getCString();
	sTakeOutLimit = dynamic_cast<CCString *>(mDic->objectForKey("strTakeOutLimit"))->getCString();
	sInsureNotEnough = dynamic_cast<CCString *>(mDic->objectForKey("strInsureNotEnough"))->getCString();
	sScoreNotEnough = dynamic_cast<CCString *>(mDic->objectForKey("strScoreNotEnough"))->getCString();
	sBuyTypeAuction = dynamic_cast<CCString *>(mDic->objectForKey("strBuyTypeAuction"))->getCString();
	sBuyTypeShop = dynamic_cast<CCString *>(mDic->objectForKey("strBuyTypeShop"))->getCString();
	sLottery = dynamic_cast<CCString *>(mDic->objectForKey("strLottery"))->getCString();
	sAllPice = dynamic_cast<CCString *>(mDic->objectForKey("strAllPice"))->getCString();

	mDic->removeAllObjects();
	CC_SAFE_RELEASE_NULL(mDic);
}
void BaseAttributes::initCardOffset(){
	CCString *tempStr = CCString::createWithFormat("data/cardOffset.plist");;
	CCArray *tempArray = CCArray::createWithContentsOfFile(tempStr->getCString());
	for (int i = 0; i < tempArray->count(); i++) {
		CCDictionary* mDic = dynamic_cast<CCDictionary *>(tempArray->objectAtIndex(i));
		iCardOffsetX[i] = dynamic_cast<CCString *>(mDic->objectForKey("offset_x"))->intValue();
		iCardOffsetY[i] = dynamic_cast<CCString *>(mDic->objectForKey("offset_y"))->intValue();
		iCardOffsetSpace[i] = dynamic_cast<CCString *>(mDic->objectForKey("offset_space"))->intValue();
	}
	tempArray->removeAllObjects();
	CC_SAFE_RELEASE_NULL(tempArray);
}
