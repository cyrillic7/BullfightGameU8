
#pragma once
#include "PlayerLayer.h"
class PlayerLayerSixSwap:public PlayerLayer {

public:
	PlayerLayerSixSwap();
	~PlayerLayerSixSwap();
	CREATE_FUNC(PlayerLayerSixSwap);
	virtual void onEnter();
	virtual void onExit();
private:
    
	//设置牌位置
	virtual void doSetCardMovePos(int i);
};
