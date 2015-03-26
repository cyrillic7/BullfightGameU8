#pragma once
#include "cocos2d.h"
#include "MainScene.h"
#include "GameLogic.h"
#include "StructLogon.h"
USING_NS_CC;
class DataModel:public CCObject{
public:
	DataModel();
	~DataModel();
	static DataModel* sharedDataModel();
private:
	void initDataModel();
public:
	static bool isSound;
	static bool isMusic;

	CCSize deviceSize;

	CC_SYNTHESIZE(MainScene *, mainScene, MainScene);
	
	GameLogic *gameLogic;

	CCArray *m_aTagGameKind;

	std::vector <tagGameServer *> tagGameServerList;

	CMD_MB_LogonSuccess *logonSuccessUserInfo;
	//tagGameServer *tagGameServerList[];
};
