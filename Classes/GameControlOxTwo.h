//
//  GameHUD.h
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//
#pragma once
#include "BaseGameControl.h"
class GameControlOxTwo:public BaseGameControl
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
