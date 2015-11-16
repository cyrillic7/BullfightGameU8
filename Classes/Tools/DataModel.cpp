/**
 *数据模型
 *
 *
 */
#include <algorithm>
#include "DataModel.h"
#include "Tools.h"
#include "GameConfig.h"
#include "../extensions/spine/Json.h"
#include "../Network/CMD_Server/Packet.h"
bool DataModel::isSound = true;
bool DataModel::isMusic = true;
int DataModel::curLineIndex = 0;
//是否是游客
bool DataModel::isQuickUser = false;
//int DataModel::gameChapter = 0;
//int DataModel::gameCustomsPass = 0;
DataModel* _sharedContext;
DataModel::DataModel()
:isSit(false)
,m_lTurnMaxScore(0)
, isShowNewMsg(false)
, isShowNewTaskMsg(false)
, isShowNewAuctionMsg(false)
, ipaddr("")
{
    
	DataModel::isMusic = Tools::getBoolByRMS(RMS_IS_MUSIC);
	DataModel::isSound = Tools::getBoolByRMS(RMS_IS_SOUND);
	sLogonAccount=Tools::getStringByRMS(RMS_LOGON_ACCOUNT);
	sLogonPassword=Tools::getStringByRMS(RMS_LOGON_PASSWORD);

	deviceSize = CCDirector::sharedDirector()->getWinSize();
	
//	logonSuccessUserInfo=new CMD_MB_LogonSuccess();
	userInfo=new tagUserInfo();
	// pthread_mutex_init(&sResponseQueueMutex, NULL);
	//
}
DataModel::~DataModel() {
	CCLOG("~ <<%s>>",__FUNCTION__);
	m_aTagGameKind->removeAllObjects();
	m_aTagGameKind->release();
	m_aMakeText->removeAllObjects();
	m_aMakeText->release();
	urlLogon->removeAllObjects();
	urlLogon->release();
	while (readDataQueueGameIng.size()>0)
	{
		readDataQueueGameIng.pop();
	}
	while (readDataQueueLobby.size() > 0)
	{
		readDataQueueLobby.pop();
	}
	
	//CC_SAFE_DELETE(logonSuccessUserInfo);
	CC_SAFE_DELETE(userInfo);
	removeTagGameServerList(tagGameServerListOxTwo);
	removeTagGameServerList(tagGameServerListOxHundred);
	removeTagGameServerList(tagGameServerListOxOneByOne);
	removeTagGameServerList(tagGameServerListSixSwap);
	vecJettonNode.clear();
	// pthread_mutex_destroy(&sResponseQueueMutex);
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
	m_aMakeText = CCArray::create();
	m_aMakeText->retain();
	urlLogon = CCArray::create();
	urlLogon->retain();

	initNewMsgTip();
}
//初始化新消息提示
void DataModel::initNewMsgTip(){
	std::string sMore = Tools::getStringByRMS(RMS_NEW_MSG_TIP);
	Json *root = Json_create(sMore.c_str());
	if (root)
	{
		Json* _date = Json_getItem(root, "newMsgData");
		Json* _record = root->child;
		while (_record)
		{
			NewMsg newMsg;
			long userID;
			Json* _userID = Json_getItem(_record, "userId");
			if (_userID->type == Json_String)
			{
				userID = strtol(_userID->valuestring,NULL,10);
			}
			Json* _pwd = Json_getItem(_record, "msgId");
			if (_pwd->type == Json_String)
			{
				newMsg.sMsgId = _pwd->valuestring;
			}
			DataModel::sharedDataModel()->mNewMsg.insert(std::map<long, NewMsg>::value_type(userID, newMsg));
			_record = _record->next;
		}
	}
	Json_dispose(root);
}
bool lessSecond(const tagGameServer * m1, const tagGameServer * m2) {
	//CCLOG("m1:%d  m2:%d <<%s>>",m1->wSortID,m2->wSortID, __FUNCTION__);
	return m1->wSortID < m2->wSortID;
}
void DataModel::sortVector(std::vector <tagGameServer *> &vTagGameServer){
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
std::string DataModel::getLogonAddr(){
	if (urlLogon->count()==0)
	{
		return GAME_IP;
	}
	if (curLineIndex>=urlLogon->count())
	{
		curLineIndex = 0;
	}
	std::string sAddr =((CCString*)urlLogon->objectAtIndex(curLineIndex))->getCString();
	return sAddr;
}
//重设连接
void DataModel::resetCon(){
	ipaddr = "";
	curLineIndex++;
}
