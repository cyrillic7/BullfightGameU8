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
#include "../LogonScene/LogonScene.h"
#include "../Tools/CStringAide.h"
USING_NS_CC;
USING_NS_CC_EXT;
class LogoScene:public CCLayer,public CStringAide
{
private:
	//更新校验是否完成
	bool isCheckFinsh;
	//更新信息
	updateInfo uInfo;
public:
    LogoScene();
    ~LogoScene();
    //场景初始化
    static CCScene* scene();
    CREATE_FUNC(LogoScene);
    
private:
	virtual void onEnter();
	virtual void onExit();
	//进入主游戏场景
    void updateToMainScene(float dt);

	//动画回调
	void onAnimationEventOver(CCArmature *pArmature, MovementEventType movementType, const char *movementID);
	void onAnimationEventFrame(CCBone *bone, const char *evt, int originFrameIndex, int currentFrameIndex);

	//下载回调
	void HttpRequestComplete(CCHttpClient *sender, CCHttpResponse *response);
	//更新校验
	void checkUpdate(const char* buf);
	//比较字符
	bool strComparisonByObj(CCObject *obj1,CCObject *obj2);
};
#endif /* defined(__BullfightGame__LogoScene__) */
