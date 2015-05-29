
#pragma once
#include "PlayerLayer.h"
class PlayerLayerOneByOne:public PlayerLayer {

public:
	PlayerLayerOneByOne();
	~PlayerLayerOneByOne();
	CREATE_FUNC(PlayerLayerOneByOne);
	virtual void onEnter();
	virtual void onExit();
private:
    
	//设置牌位置
	virtual void doSetCardMovePos(int i);
};
