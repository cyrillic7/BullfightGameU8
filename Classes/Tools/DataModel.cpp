/**
 *数据模型
 *
 *
 */
#include <algorithm>
#include "DataModel.h"
#include "Tools.h"
#include "GameConfig.h"
bool DataModel::isSound = true;
bool DataModel::isMusic = true;
//int DataModel::gameChapter = 0;
//int DataModel::gameCustomsPass = 0;
DataModel* _sharedContext;
DataModel::DataModel()
:isSit(false)
,m_lTurnMaxScore(0)
{
    
	DataModel::isMusic = Tools::getBoolByRMS(RMS_IS_MUSIC);
	DataModel::isSound = Tools::getBoolByRMS(RMS_IS_SOUND);
	sLogonAccount=Tools::getStringByRMS(RMS_LOGON_ACCOUNT);
	sLogonPassword=Tools::getStringByRMS(RMS_LOGON_PASSWORD);

	deviceSize = CCDirector::sharedDirector()->getWinSize();
	
//	logonSuccessUserInfo=new CMD_MB_LogonSuccess();
	userInfo=new tagUserInfo();
	// pthread_mutex_init(&sResponseQueueMutex, NULL);
}
DataModel::~DataModel() {
	CCLOG("~ <<%s>>",__FUNCTION__);
	m_aTagGameKind->removeAllObjects();
	m_aTagGameKind->release();

	//CC_SAFE_DELETE(logonSuccessUserInfo);
	CC_SAFE_DELETE(userInfo);
	removeTagGameServerList(tagGameServerListOxTwo);
	removeTagGameServerList(tagGameServerListOxHundred);
	removeTagGameServerList(tagGameServerListOxOneByOne);
	removeTagGameServerList(tagGameServerListSixSwap);
	vecJettonNode.clear();
	// pthread_mutex_destroy(&sResponseQueueMutex);
	/*
	_outputEnemysData->removeAllObjects();
	_outputEnemysData->release();
	_enemys->removeAllObjects();
	_enemys->release();
	_lineDatas->removeAllObjects();
	_lineDatas->release();
	_tMapTilesetInfo->removeAllObjects();
	_tMapTilesetInfo->release();
	_goods->removeAllObjects();
	_goods->release();
	_effectSprite->removeAllObjects();
	_effectSprite->release();
	

	_pDicSoundName->removeAllObjects();
	_pDicSoundName->release();
	_pDicSoundName = NULL;

	_pDicSound->removeAllObjects();
	_pDicSound->release();
	_pDicSound = NULL;*/
	/*
	_partners->removeAllObjects();
	_partners->release();
	
	_bulletMagicWands->removeAllObjects();
	_bulletMagicWands->release();

	
	*/
	_sharedContext = NULL;
}
DataModel* DataModel::sharedDataModel()
{
	if (_sharedContext == NULL) {
		_sharedContext = new DataModel();
		_sharedContext->initDataModel();
		return _sharedContext;
	}
	return _sharedContext;
}
void DataModel::initDataModel(){

	m_aTagGameKind = CCArray::create();
	m_aTagGameKind->retain();
	/*
	
	_outputEnemysData = CCArray::create();
	_outputEnemysData->retain();
	_enemys = CCArray::create();
	_enemys->retain();
	_lineDatas = CCArray::create();
	_lineDatas->retain();
	_tMapTilesetInfo = CCArray::create();
	_tMapTilesetInfo->retain();
	_goods = CCArray::create();
	_goods->retain();
	_effectSprite = CCArray::create();
	_effectSprite->retain();
	
	
	_pDicSoundName = CCDictionary::create();
	_pDicSoundName->retain();

	_pDicSound = CCDictionary::create();
	_pDicSound->retain();
	//_partners = CCArray::create();
	//_partners->retain();
	//
	//_bulletMagicWands = CCArray::create();
	//_bulletMagicWands->retain();*/
}
bool lessSecond(const tagGameServer * m1, const tagGameServer * m2) {
	return m1->wSortID < m2->wSortID;
}
void DataModel::sortVector(std::vector <tagGameServer *> vTagGameServer){
	std::sort(vTagGameServer.begin(),vTagGameServer.end(),lessSecond);
}
void DataModel::removeTagGameServerList(std::vector <tagGameServer *> vTagGameServer){
	for (int i=0;i<vTagGameServer.size();i++)
	{
		if (vTagGameServer[i])
		{
			CC_SAFE_DELETE(vTagGameServer[i]);
		}
	}
	vTagGameServer.clear();
	vTagGameServer.resize(0);
}
