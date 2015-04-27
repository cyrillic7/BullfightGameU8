//
//
//
#pragma once
#include "BaseGameControl.h"
class GameControlOxHundred:public BaseGameControl
{
public:
	GameControlOxHundred();
	~GameControlOxHundred();
	virtual void onEnter();
	virtual void onExit();
    CREATE_FUNC(GameControlOxHundred);
private:
	int getChairIndex(int meChairID,int chairID);
	//²Ëµ¥////////////////////////////////////////////////////////////////////////
	void onMenuBack(CCObject* pSender, TouchEventType type);
};
