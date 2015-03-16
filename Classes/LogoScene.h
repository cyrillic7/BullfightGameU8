//
//  LogoScene.h
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#ifndef __BullfightGame__LogoScene__
#define __BullfightGame__LogoScene__
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
class LogoScene:public CCLayer
{
public:
    LogoScene();
    ~LogoScene();
    
    static CCScene* scene();
    CREATE_FUNC(LogoScene);
private:
    void updateToMainScene(float dt);
};
#endif /* defined(__BullfightGame__LogoScene__) */
