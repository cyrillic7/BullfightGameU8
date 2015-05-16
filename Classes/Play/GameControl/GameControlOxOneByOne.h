//
//  GameHUD.h
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//
#pragma once
#include "GameControlBase.h"
#include "GameLogic/GameLogic.h"
class GameControlOxOneByOne:public GameControlBase,public GameLogic
{
public:
	GameControlOxOneByOne();
	~GameControlOxOneByOne();
	virtual void onEnter();
	virtual void onExit();
    CREATE_FUNC(GameControlOxOneByOne);
private:
	int getChairIndex(int meChairID,int chairID);
	//开牌
	virtual void menuOpenCard(CCObject* pSender, TouchEventType type);
};
