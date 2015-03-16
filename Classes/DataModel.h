#pragma once
#include "cocos2d.h"
#include "MainScene.h"
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

	CC_SYNTHESIZE(MainScene *, mainScene, MainScene);
	
	int enemyBaseLife;

	CCArray *_bullet;
};
