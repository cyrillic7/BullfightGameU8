//
//  LogonScene.h
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//
#ifndef __BullfightGame__LogonScene__
#define __BullfightGame__LogonScene__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
class LogonScene:public CCLayer
{
public:
    LogonScene();
    ~LogonScene();
	virtual void onEnter();
	virtual void onExit();
    static CCScene* scene();
    
    CREATE_FUNC(LogonScene);
private:
};

#endif /* defined(__BullfightGame__LogonScene__) */
