//
//  LogoScene.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//
#include "LogoScene.h"
#include "../Tools/Tools.h"
#include "../Tools/DataModel.h"
#include "../LogonScene/LogonScene.h"
LogoScene::LogoScene(){
	//0.5秒后进入MainScene
	//scheduleOnce(SEL_SCHEDULE(&LogoScene::updateToMainScene), 0.5);
}
LogoScene::~LogoScene(){
	CCLog("~ <<%s>>", __FUNCTION__);
    
}
//场景初始化
CCScene* LogoScene::scene()
{
    CCScene *scene = CCScene::create();
    LogoScene *layer = LogoScene::create();
    scene->addChild(layer);
    return scene;
}
void LogoScene::onEnter(){
	CCLayer::onEnter();
	CCLayerColor *pLayerColor = CCLayerColor::create(ccc4(255, 255, 255, 255));
	this->addChild(pLayerColor);
	//标题动画
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(CCS_PATH_SCENE(AnimationCompanyLogo.ExportJson));
	CCArmature *pAnimateTitle = CCArmature::create("AnimationCompanyLogo");
	pAnimateTitle->setAnchorPoint(ccp(0.5, 0.5));
	pAnimateTitle->setPosition(DataModel::sharedDataModel()->deviceSize.width/2-220,
		DataModel::sharedDataModel()->deviceSize.height / 2+pAnimateTitle->getContentSize().height / 2);
	pAnimateTitle->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(LogoScene::onAnimationEventOver));//动画播完回调用
	pAnimateTitle->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(LogoScene::onAnimationEventFrame));
	pAnimateTitle->getAnimation()->play("Animation1");
	this->addChild(pAnimateTitle);
}
void LogoScene::onExit(){
	CCLayer::onExit();
}
//动画回调
void LogoScene::onAnimationEventOver(CCArmature *pArmature, MovementEventType movementType, const char *movementID){
	switch (movementType)
	{
	case cocos2d::extension::COMPLETE:
	case cocos2d::extension::LOOP_COMPLETE:
	{
		if (strcmp(movementID, "Animation1") == 0)
		{
			updateToMainScene(0);
		}
	}
	break;
	default:
		break;
	}
}
void LogoScene::onAnimationEventFrame(CCBone *bone, const char *evt, int originFrameIndex, int currentFrameIndex){
	if (strcmp(evt, "userEnter") == 0)
	{

		//pPlayerData[bone->getArmature()->getTag()]->pIPlayerIcon->setVisible(true);
	}
}
//进入主游戏场景
void LogoScene::updateToMainScene(float dt){
	Tools::setTransitionAnimation(0, 0, LogonScene::scene());
}