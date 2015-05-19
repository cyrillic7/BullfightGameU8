//
//  GameHUD.h
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//
#pragma once
#include "GameControlBase.h"
#include "../../GameLogic/GameLogic.h"
class GameControlOxTwo:public GameControlBase,public GameLogic
{
public:
	GameControlOxTwo();
	~GameControlOxTwo();
	virtual void onEnter();
	virtual void onExit();
    CREATE_FUNC(GameControlOxTwo);
private:
	int getChairIndex(int meChairID,int chairID);
	//开牌
	virtual void menuOpenCard(CCObject* pSender, TouchEventType type);
	//隐藏用户
	virtual void hidePlayer(CMD_GR_UserStatus *userInfo);

};
