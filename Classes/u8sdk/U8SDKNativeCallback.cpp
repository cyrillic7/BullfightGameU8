#include "U8SDKInterface.h"



U8SDKNativeCallback::U8SDKNativeCallback(U8SDKCallback* callback)
{
	this->gameCallback = callback;
}

U8SDKNativeCallback::~U8SDKNativeCallback()
{
}


void U8SDKNativeCallback::OnU8InitSuc(cocos2d::CCNode *sender, void * data)
{
	if (data == NULL)
	{
		CCLog("OnU8InitSuc Data Type Error:: curr data type is not map.");
		return;
	}
	/*if (data.isNull() || data.getType() != cocos2d::Value::Type::MAP)
	{
		CCLOG("OnU8InitSuc Data Type Error:: curr data type is not map.");
		return;
	}*/
	CCLog("OnU8InitSuc Data Type Success:: curr data type is  map.");
	CCDictionary *convertedData = (CCDictionary *)data;

	bool exit = convertedData->valueForKey("isSupportExit")->boolValue();
	bool accountCenter = convertedData->valueForKey("isSupportAccountCenter")->boolValue();
	bool logout = convertedData->valueForKey("isSupportLogout")->boolValue();
	//cocos2d::ValueMap json = data.asValueMap();
	//bool exit = json["isSupportExit"].asBool();
	//bool accountCenter = json["isSupportAccountCenter"].asBool();
	//bool logout = json["isSupportLogout"].asBool();

	U8SDKInterface::getInstance()->setSupports(exit, accountCenter, logout);

	if (this->gameCallback != NULL)
	{
		this->gameCallback->OnInitSuc();
	}
}

void U8SDKNativeCallback::OnU8LoginSuc(cocos2d::CCNode *sender, void * data)
{
	if (data == NULL)
	{
		CCLog("OnU8LoginSuc Data Type Error:: curr data type is not map.");
		return;
	}
	/*if (data.isNull() || data.getType() != cocos2d::Value::Type::MAP)
	{
		CCLOG("OnU8LoginSuc Data Type Error:: curr data type is not map.");
		return;
	}*/
	CCLog("OnU8LoginSuc Data Type Success:: curr data type is not NULL.");

	CCDictionary *convertedData = (CCDictionary *)data;

	U8LoginResult* result = U8LoginResult::create();
	CCLog("convertedData->valueForKey('isSuc')->boolValue().");
	result->isSuc = convertedData->valueForKey("isSuc")->boolValue();
	CCLog("convertedData->valueForKey('isSwitchAccount')->boolValue().");
	result->isSwitchAccount = convertedData->valueForKey("isSwitchAccount")->boolValue(); 
	CCLog("convertedData->valueForKey('isSwitchAccount')->boolValue().");
	result->userId = ((CCString*)convertedData->objectForKey("userID"))->getCString();
	result->sdkUserId = ((CCString*)convertedData->objectForKey("sdkUserID"))->getCString();
	result->username = ((CCString*)convertedData->objectForKey("username"))->getCString();
	result->sdkUsername = ((CCString*)convertedData->objectForKey("sdkUsername"))->getCString();
	result->token = ((CCString*)convertedData->objectForKey("token"))->getCString();
	CCLog("result->isSuc = %d ", result->isSuc);
	CCLog("result->isSwitchAccount = %d", result->isSwitchAccount);
	CCLog("result->userId = %s", result->userId.c_str());
	CCLog("result->sdkUserId = %s", result->sdkUserId.c_str());
	CCLog("result->username = %s", result->username.c_str());
	CCLog("result->sdkUsername = %s", result->sdkUsername.c_str());
	CCLog("result->token = %s", result->token.c_str());
	//cocos2d::ValueMap json = data.asValueMap();
	/*result->isSuc = json["isSuc"].asBool();
	result->isSwitchAccount = json["isSwitchAccount"].asBool();
	result->userId = json["userID"].asString();
	result->sdkUserId = json["sdkUserID"].asString();
	result->username = json["username"].asString();
	result->sdkUsername = json["sdkUsername"].asString();
	result->token = json["token"].asString();*/

	if (this->gameCallback != NULL)
	{
		this->gameCallback->OnLoginSuc(result);
	}
}

void U8SDKNativeCallback::OnU8SwitchLogin(cocos2d::CCNode *sender, void * data)
{
	if (this->gameCallback != NULL)
	{
		this->gameCallback->OnSwitchLogin();
	}
}

void U8SDKNativeCallback::OnU8Logout(cocos2d::CCNode *sender, void * data)
{
	if (this->gameCallback != NULL)
	{
		this->gameCallback->OnLogout();
	}
}

void U8SDKNativeCallback::OnU8PaySuc(cocos2d::CCNode *sender, void * data)
{
	//单机支付使用，网游不使用该回调
}
