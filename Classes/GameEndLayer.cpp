

#include "GameEndLayer.h"
#include "GameConfig.h"
#include "DataModel.h"
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
}
void GameEndLayer::onExit(){
	CCLayer::onExit();
}
void GameEndLayer::showEnd(bool isWin){
	if (isWin)
	{
		pAGameEnd->getAnimation()->play("win");
	}else
	{
		pAGameEnd->getAnimation()->play("lost");
	}
}