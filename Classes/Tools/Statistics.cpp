

#include "Statistics.h"
#include "../Network/MD5/MD5.h"
#include "StatisticsConfig.h"
#include "../Tools/DataModel.h"
#include "../Tools/Tools.h"
#include "../Tools/GameConfig.h"
Statistics::Statistics()
	:eStatisticsType(S_INSTALL)
{
}
Statistics::~Statistics() {
}
//发送统计数据
void Statistics::sendStatisticsData(int type){
	setStatisticsType((StatisticsType)type);

	//安装统计
	CCHttpRequest *request = new CCHttpRequest();
	std::string url = k_url;
	request->setRequestType(CCHttpRequest::kHttpPost);
	request->setResponseCallback(this, callfuncND_selector(Statistics::onHttpRequestCompleted));//回调函数，处理接收到的信息
	
	std::string postData ;
	switch (eStatisticsType)
	{
	case Statistics::S_INSTALL:
	{

		url += "GameInstall";

		postData = "sessionID=";
		postData.append(k_session_id);
		postData.append("&code=");
		std::string code = "server";
		code.append(k_session_id);
		code.append(k_session_verion);
		code.append("lmyspread");

		MD5 md;
		code = md.GetMd5(code.c_str(), (int)code.length());

		postData.append(code.c_str());
		postData.append("&machineCode=");
		postData.append(Tools::getMachineID().c_str());
	}
		break;
	case Statistics::S_REGISTER:
	{
		url += "AccountsRegister";
		//////////////////////////////////////////////////////////////////////////
		postData = "gameID=";
		postData.append(CCString::createWithFormat("%d", DataModel::sharedDataModel()->userInfo->dwGameID)->getCString());
		postData.append("&sessionID=");
		postData.append(k_session_id);
		postData.append("&code=");
		std::string code = "server";
		code.append(k_session_id);
		code.append(k_session_verion);
		code.append("lmyspread");

		MD5 md;
		code = md.GetMd5(code.c_str(), (int)code.length());

		postData.append(code.c_str());
		postData.append("&machineCode=");
		postData.append(Tools::getMachineID().c_str());
	}
		break;
	case Statistics::S_ACCOUNT_LOGON:
	{
		url += "AccountsLogon";
		//////////////////////////////////////////////////////////////////////////
		postData = "gameID=";
		postData.append(CCString::createWithFormat("%d", DataModel::sharedDataModel()->userInfo->dwGameID)->getCString());
		postData.append("&sessionID=");
		postData.append(k_session_id);
		postData.append("&code=");
		std::string code = "server";
		code.append(k_session_id);
		code.append(k_session_verion);
		code.append("lmyspread");

		MD5 md;
		code = md.GetMd5(code.c_str(), (int)code.length());

		postData.append(code.c_str());
		postData.append("&machineCode=");
		postData.append(Tools::getMachineID().c_str());
		CCLOG("%s?%s <<%s>>",url.c_str(),postData.c_str(), __FUNCTION__);
	}
		break;
	default:
		break;
	}
	
	
	request->setUrl(url.c_str());
	request->setRequestData(postData.c_str(), (int)strlen(postData.c_str()));
	CCHttpClient::getInstance()->send(request);
	request->release();
}
void Statistics::onHttpRequestCompleted(cocos2d::CCNode *sender, void *data)
{
	CCHttpResponse* response = (cocos2d::extension::CCHttpResponse*)data;
	if (!response) { CCLOG("Log:response =null,plase check it."); return; }
	//请求失败
	if (!response->isSucceed())
	{
		return;
	}
	switch (eStatisticsType)
	{
	case Statistics::S_INSTALL:
	{
		Tools::saveBoolByRMS(RMS_FIRST_LOGON, false);
	}
		break;
	case Statistics::S_REGISTER:
		break;
	case Statistics::S_ACCOUNT_LOGON:
		break;
	default:
		break;
	}
}