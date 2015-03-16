//
//  MainScene.h
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#ifndef __BullfightGame__MainScene__
#define __BullfightGame__MainScene__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
class MainScene:public CCLayer
{
public:
    static CCScene* scene();
    
    CREATE_FUNC(MainScene);
};

#endif /* defined(__BullfightGame__MainScene__) */
