//
//  GameHUD.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#include "GameControlOxTwo.h"
GameControlOxTwo::GameControlOxTwo()
{

}
GameControlOxTwo::~GameControlOxTwo(){

}
void GameControlOxTwo::onEnter(){
	GameControlBase::onEnter();
}
void GameControlOxTwo::onExit(){
	GameControlBase::onExit();
}
int GameControlOxTwo::getChairIndex(int meChairID,int chairID){
	if (meChairID==0)
	{
		if (chairID==0)
		{
			return 3;
		}
	}else
	{
		if (chairID==1)
		{
			return 3;
		}
	}
	return 0;
}
