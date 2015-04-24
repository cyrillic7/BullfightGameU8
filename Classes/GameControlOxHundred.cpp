//
//
//
//

#include "GameControlOxHundred.h"
GameControlOxHundred::GameControlOxHundred()
{

}
GameControlOxHundred::~GameControlOxHundred(){

}
void GameControlOxHundred::onEnter(){
	BaseGameControl::onEnter();
}
void GameControlOxHundred::onExit(){
	BaseGameControl::onExit();
}
int GameControlOxHundred::getChairIndex(int meChairID,int chairID){
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
