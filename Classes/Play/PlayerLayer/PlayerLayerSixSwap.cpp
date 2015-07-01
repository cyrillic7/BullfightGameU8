

#include "PlayerLayerSixSwap.h"
#include "../../Tools/GameConfig.h"
#include "../../Tools/DataModel.h"
/*
#include "../../Tools/Tools.h"*/
#include "../../MainScene/MainSceneBase.h"
PlayerLayerSixSwap::PlayerLayerSixSwap ()
{
}
PlayerLayerSixSwap::~PlayerLayerSixSwap() {
}
void PlayerLayerSixSwap::onEnter(){
	PlayerLayer::onEnter();
}
void PlayerLayerSixSwap::onExit(){
	PlayerLayer::onExit();
    
}
//设置牌位置
void PlayerLayerSixSwap::doSetCardMovePos(int i){
	UIPanel *playerPanel = pPlayerData[i]->pPlayerPanel;
	CCPoint playerPos = playerPanel->getPosition();
	CCPoint cardPos = playerPos;
	switch (i)
	{
	case 0:
	{
		cardPos = ccpAdd(playerPos, ccp(playerPanel->getContentSize().width/2, -50));
	}
		break;
	case 1:
	case 2:
	{
		cardPos = ccpAdd(playerPos, ccp(playerPanel->getContentSize().width+90,playerPanel->getContentSize().height/2));

	}
		break;
	case 3:
	{
		cardPos = ccpAdd(ccp(DataModel::sharedDataModel()->deviceSize.width / 2, playerPos.y), ccp(0, playerPanel->getContentSize().height / 2 + 90));
	}
		break;
	case 4:
	case 5:
	{
		cardPos = ccpAdd(playerPos, ccp(-90, playerPanel->getContentSize().height / 2));
	}
		break;
	default:
		break;
	}
	getMainScene()->posChair[i] = cardPos;
}