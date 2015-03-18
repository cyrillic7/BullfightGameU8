
#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class CardLayer:public CCLayer {
private:
	//扑克集
	CCBatchNode *batchCard;
public:
	CardLayer();
	~CardLayer();
	CREATE_FUNC(CardLayer);
	virtual bool init();
};
