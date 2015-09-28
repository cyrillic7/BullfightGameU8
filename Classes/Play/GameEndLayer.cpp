

#include "GameEndLayer.h"
#include "../Tools/GameConfig.h"
#include "../Tools/DataModel.h"
GameEndLayer::GameEndLayer()
{
}
GameEndLayer::~GameEndLayer() {
}
bool GameEndLayer::init(){
	if (!CCLayer::init())
	{
		return false;
	}
    
	//加载扑克动画文件
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(CCS_PATH_SCENE(AnimationGameEnd.ExportJson));
	return true;
}
void GameEndLayer::onEnter(){
	CCLayer::onEnter();
	pAGameEnd=CCArmature::create("AnimationGameEnd");
	this->addChild(pAGameEnd);
	pAGameEnd->setPosition(DataModel::sharedDataModel()->deviceSize.width/2,DataModel::sharedDataModel()->deviceSize.height/2);
	initAnimationEventListener();
}
void GameEndLayer::onExit(){
	CCLayer::onExit();
}
void GameEndLayer::initAnimationEventListener(){
	pAGameEnd->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(GameEndLayer::onAnimationEventOver));//动画播完回调用
	pAGameEnd->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(GameEndLayer::onAnimationEventFrame));
}
void GameEndLayer::onAnimationEventOver(CCArmature *pArmature, MovementEventType movementType, const char *movementID){
	switch (movementType)
	{
	case cocos2d::extension::COMPLETE:
	case cocos2d::extension::LOOP_COMPLETE:
		{
			/*if (strcmp(movementID, "winO") == 0)
			{
				pArmature->getAnimation()->play("win");
			}else if (strcmp(movementID, "lostO") == 0)
			{
				pArmature->getAnimation()->play("lost");
			}*/
			pArmature->setVisible(false);
		}
		break; 
	default:
		break;
	}
}
void GameEndLayer::onAnimationEventFrame(CCBone *bone, const char *evt, int originFrameIndex, int currentFrameIndex){
	/*if (strcmp(evt, "bomb1") == 0)
	{

	}*/
}
void GameEndLayer::showEnd(bool isWin){
	pAGameEnd->setVisible(true);
	if (isWin)
	{
		pAGameEnd->getAnimation()->play("Animation1");
	}else
	{
		pAGameEnd->getAnimation()->play("Animation2");
	}
}