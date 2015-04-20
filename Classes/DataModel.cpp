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

	deviceSize = CCDirector::sharedDirector()->getWinSize();
	
	logonSuccessUserInfo=new CMD_MB_LogonSuccess();
	userInfo=new tagUserInfo();
	// pthread_mutex_init(&sResponseQueueMutex, NULL);
}
DataModel::~DataModel() {
	CCLog("~ <<%s>>",__FUNCTION__);
	m_aTagGameKind->removeAllObjects();
	m_aTagGameKind->release();

	CC_SAFE_DELETE(logonSuccessUserInfo);
	CC_SAFE_DELETE(userInfo);
	removeTagGameServerList();

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
void DataModel::sortVector(){
	std::sort(tagGameServerList.begin(),tagGameServerList.end(),lessSecond);
}
void DataModel::removeTagGameServerList(){
	for (int i=0;i<tagGameServerList.size();i++)
	{
		CC_SAFE_DELETE(tagGameServerList[i]);
	}
	tagGameServerList.clear();
	tagGameServerList.resize(0);
}
