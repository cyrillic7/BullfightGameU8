

#include "PlayerLayerOneByOne.h"
#include "../../Tools/GameConfig.h"
/*#include "../../Tools/DataModel.h"
#include "../../Tools/Tools.h"*/
#include "../../MainScene/MainSceneBase.h"
PlayerLayerOneByOne::PlayerLayerOneByOne ()
{
}
PlayerLayerOneByOne::~PlayerLayerOneByOne() {
}
void PlayerLayerOneByOne::onEnter(){
	PlayerLayer::onEnter();
}
void PlayerLayerOneByOne::onExit(){
	PlayerLayer::onExit();
}
//ÉèÖÃÅÆÎ»ÖÃ
void PlayerLayerOneByOne::doSetCardMovePos(int i){
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
		cardPos = ccpAdd(ccp(SCENE_SIZE.width / 2, playerPos.y), ccp(0, playerPanel->getContentSize().height / 2 + 10));
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