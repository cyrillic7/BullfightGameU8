#include "U8SDKInterface.h"
USING_NS_CC;

U8SDKInterface* U8SDKInterface::_instance = NULL;

U8SDKInterface* U8SDKInterface::getInstance()
{
	if (NULL == _instance)
	{
		_instance = new U8SDKInterface();
	}

	return _instance;
}


void U8SDKInterface::initSDK(U8SDKCallback* callback)
{
	nativeCallback = new U8SDKNativeCallback(callback);

	NDKHelper::AddSelector("U8Selectors", "OnU8InitSuc", callfuncND_selector(U8SDKNativeCallback::OnU8InitSuc), nativeCallback);
	NDKHelper::AddSelector("U8Selectors", "OnU8LoginSuc", callfuncND_selector(U8SDKNativeCallback::OnU8LoginSuc), nativeCallback);
	NDKHelper::AddSelector("U8Selectors", "OnU8SwitchLogin", callfuncND_selector(U8SDKNativeCallback::OnU8SwitchLogin), nativeCallback);
	NDKHelper::AddSelector("U8Selectors", "OnU8Logout", callfuncND_selector(U8SDKNativeCallback::OnU8Logout), nativeCallback);
	NDKHelper::AddSelector("U8Selectors", "OnU8PaySuc", callfuncND_selector(U8SDKNativeCallback::OnU8PaySuc), nativeCallback);

}

void U8SDKInterface::login()
{
	/*Value v;*/
	CCDictionary* prms = CCDictionary::create();
	SendMessageWithParams("login", prms);
}

std::string U8SDKInterface::getchannelId()
{
	CCDictionary* prms = CCDictionary::create();
	std::string channel;
	channel = getMessageWithParams("getChannel", prms);
	CCLog("U8SDKInterface::getchannelId()  %s", channel.c_str());
	return channel;
}

void U8SDKInterface::loginCustom(const std::string& customData)
{
	CCDictionary* prms = CCDictionary::create();
	prms->setObject(CCString::create("customData"), customData);

	/*ValueMap params;
	params["customData"] = customData;
	Value v = Value(params);*/
	SendMessageWithParams("loginCustom", prms);
}

void U8SDKInterface::switchLogin()
{
	//Value v;
	CCDictionary* prms = CCDictionary::create();
	SendMessageWithParams("switchLogin", prms);
}

void U8SDKInterface::logout()
{
	//Value v;
	CCDictionary* prms = CCDictionary::create();
	SendMessageWithParams("logout", prms);
}

void U8SDKInterface::showAccoutCenter()
{
	//Value v;
	CCDictionary* prms = CCDictionary::create();
	SendMessageWithParams("showAccountCenter", prms);
}

void U8SDKInterface::submitGameData(U8ExtraGameData* data)
{
	/*ValueMap params;
	params["dataType"] = data->dataType;
	params["roleID"] = data->roleId;
	params["roleName"] = data->roleName;
	params["roleLevel"] = data->roleLevel;
	params["serverID"] = data->serverId;
	params["serverName"] = data->serverName;
	params["moneyNum"] = data->moneyNum;
	Value v = Value(params);*/
	CCDictionary* prms = CCDictionary::create();
	prms->setObject(CCString::create("dataType"), data->dataType);
	prms->setObject(CCString::create("roleID"), data->roleId);
	prms->setObject(CCString::create("roleName"), data->roleName);
	prms->setObject(CCString::create("roleLevel"), data->roleLevel);
	prms->setObject(CCString::create("serverID"), data->serverId);
	prms->setObject(CCString::create("serverName"), data->serverName);
	prms->setObject(CCString::create("moneyNum"), data->moneyNum);

	SendMessageWithParams("submitExtraData", prms);
}

void U8SDKInterface::sdkExit()
{
	//Value v;
	CCDictionary* prms = CCDictionary::create();
	SendMessageWithParams("exit", prms);
}

void U8SDKInterface::pay(U8PayParams* data)
{
	if (data == NULL)
	{
		CCLOG("U8SDKInterface::pay data is null.");
		return;
	}

	/*ValueMap params;
	params["productId"] = data->productId;
	params["productName"] = data->productName;
	params["productDesc"] = data->productDesc;
	params["price"] = data->price;
	params["buyNum"] = data->buyNum;
	params["coinNum"] = data->coinNum;
	params["serverId"] = data->serverId;
	params["serverName"] = data->serverName;
	params["roleId"] = data->roleId;
	params["roleName"] = data->roleName;
	params["roleLevel"] = data->roleLevel;
	params["vip"] = data->vip;
	params["orderID"] = data->roleId;
	params["extension"] = data->extension;

	Value v = Value(params);*/
	CCDictionary* prms = CCDictionary::create();
	prms->setObject(CCString::create(data->productId),"productId");
	prms->setObject(CCString::create(data->productName), "productName");
	prms->setObject(CCString::create(data->productDesc), "productDesc");
	prms->setObject(CCInteger::create(data->price), "price");
	prms->setObject(CCInteger::create(data->buyNum), "buyNum");
	prms->setObject(CCInteger::create(data->coinNum), "coinNum");
	prms->setObject(CCString::create(data->serverId), "serverId");
	prms->setObject(CCString::create(data->serverName), "serverName");
	prms->setObject(CCString::create(data->roleId), "roleId");
	prms->setObject(CCString::create(data->roleName), "roleName");
	prms->setObject(CCInteger::create(data->roleLevel), "roleLevel");
	prms->setObject(CCString::create(data->vip), "vip");
	prms->setObject(CCString::create(data->orderID), "orderID");
	prms->setObject(CCString::create(data->extension), "extension");

	/*CCLog("U8SDKInterface::pay productId==%s,productName==%s,productDesc==%s,price==%d,buyNum==%d,coinNum==%d,serverId==%s,serverName==%s,roleId==%s,roleName==%s,roleLevel==%d,vip==%s,orderID==%s,extension==%s"
		, ((CCString*)prms->objectForKey("productId"))->getCString(),
		((CCString*)prms->objectForKey("productName"))->getCString(),
		((CCString*)prms->objectForKey("productDesc"))->getCString(),
		((CCInteger*)prms->objectForKey("price"))->getValue(),
		((CCInteger*)prms->objectForKey("buyNum"))->getValue(),
		((CCInteger*)prms->objectForKey("coinNum"))->getValue(),
		((CCString*)prms->objectForKey("serverId"))->getCString(),
		((CCString*)prms->objectForKey("serverName"))->getCString(),
		((CCString*)prms->objectForKey("roleId"))->getCString(),
		((CCString*)prms->objectForKey("roleName"))->getCString(),
		((CCInteger*)prms->objectForKey("roleLevel"))->getValue(),
		((CCString*)prms->objectForKey("vip"))->getCString(),
		((CCString*)prms->objectForKey("orderID"))->getCString(),
		((CCString*)prms->objectForKey("extension"))->getCString());*/

	SendMessageWithParams("pay", prms);
}

void U8SDKInterface::setSupports(bool exit, bool accountCenter, bool logout)
{
	this->supportExit = exit;
	this->supportAccountCenter = accountCenter;
	this->supportLogout = logout;
}

bool U8SDKInterface::isSupportExit()
{
	return this->supportExit;
}

bool U8SDKInterface::isSupportAccountCenter()
{
	return this->supportAccountCenter;
}

bool U8SDKInterface::isSupportLogout()
{
	return this->supportLogout;
}


U8SDKInterface::~U8SDKInterface()
{
	NDKHelper::RemoveSelectorsInGroup("U8Selectors");

	do
	{
		delete (this->nativeCallback);
		this->nativeCallback = NULL;

	} while(0);

}
