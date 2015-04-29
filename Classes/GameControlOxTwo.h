//
//  GameHUD.h
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//
#pragma once
#include "GameControlBase.h"
class GameControlOxTwo:public GameControlBase
{
public:
	GameControlOxTwo();
	~GameControlOxTwo();
	virtual void onEnter();
	virtual void onExit();
    CREATE_FUNC(GameControlOxTwo);
private:
	int getChairIndex(int meChairID,int chairID);
};
