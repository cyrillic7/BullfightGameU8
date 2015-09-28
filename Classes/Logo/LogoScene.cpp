//
//  LogoScene.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//
#include "LogoScene.h"
#include "../Tools/Tools.h"
#include "../Tools/DataModel.h"
#include "../extensions/spine/Json.h"
#include "../Platform/coPlatform.h"
LogoScene::LogoScene()
	:isCheckFinsh(false){
	/*std::string str = "恭喜，您已经成功充值10元宝。#30#300#";
	int s=str.find("#");
	std::string tmp_s = str.substr(0, s);
 	CCLOG("ss-- :%s<<%s>>",tmp_s.c_str(), __FUNCTION__);*/
	std::string str = "sss。#30#300#";
	//int s = str.find("#");
	//std::string tmp_s = str.substr(0, s);

}
LogoScene::~LogoScene(){
	CCLog("~ <<%s>>", __FUNCTION__);
	unschedule(SEL_SCHEDULE(&LogoScene::updateToMainScene));
}
//场景初始化
CCScene* LogoScene::scene()
{
    CCScene *scene = CCScene::create();
    LogoScene *layer = LogoScene::create();
    scene->addChild(layer);
    return scene;
}
void LogoScene::onEnter(){
	CCLayer::onEnter();
	CCLayerColor *pLayerColor = CCLayerColor::create(ccc4(255, 255, 255, 255));
	this->addChild(pLayerColor);
	//标题动画
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(CCS_PATH_SCENE(AnimationCompanyLogo.ExportJson));
	CCArmature *pAnimateTitle = CCArmature::create("AnimationCompanyLogo");
	pAnimateTitle->setAnchorPoint(ccp(0.5, 0.5));
	pAnimateTitle->setPosition(DataModel::sharedDataModel()->deviceSize.width/2-220,
		DataModel::sharedDataModel()->deviceSize.height / 2+pAnimateTitle->getContentSize().height / 2);
	pAnimateTitle->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(LogoScene::onAnimationEventOver));//动画播完回调用
	pAnimateTitle->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(LogoScene::onAnimationEventFrame));
	pAnimateTitle->getAnimation()->play("Animation1");
	this->addChild(pAnimateTitle);

	std::string url = "http://download.qicainiu.com/download/Mobile/BullfightGame/updateConfigQCN.txt";

	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl(url.c_str());
	request->setRequestType(CCHttpRequest::kHttpGet);
	request->setResponseCallback(this, httpresponse_selector(LogoScene::HttpRequestComplete));
	//ImageNotificationCenter::sharedImageNotificationCenter()->m_ImageNotificationTarget = pTarget;
	//ImageNotificationCenter::sharedImageNotificationCenter()->m_pImageNotificationSelector = pSelector;
	request->setTag("GET UPDATE CONFIG");
	CCHttpClient::getInstance()->send(request);
	request->release();
}
void LogoScene::onExit(){
	CCLayer::onExit();
}
//动画回调
void LogoScene::onAnimationEventOver(CCArmature *pArmature, MovementEventType movementType, const char *movementID){
	switch (movementType)
	{
	case cocos2d::extension::COMPLETE:
	case cocos2d::extension::LOOP_COMPLETE:
	{
		if (strcmp(movementID, "Animation1") == 0)
		{
			schedule(SEL_SCHEDULE(&LogoScene::updateToMainScene), 0.1);
		}
	}
	break;
	default:
		break;
	}
}
void LogoScene::onAnimationEventFrame(CCBone *bone, const char *evt, int originFrameIndex, int currentFrameIndex){
	if (strcmp(evt, "userEnter") == 0)
	{

		//pPlayerData[bone->getArmature()->getTag()]->pIPlayerIcon->setVisible(true);
	}
}
//进入主游戏场景
void LogoScene::updateToMainScene(float dt){
	if (isCheckFinsh)
	{
		Tools::setTransitionAnimation(0, 0, LogonScene::scene(uInfo));
	}
}
//下载回调
void LogoScene::HttpRequestComplete(CCHttpClient *sender, CCHttpResponse *response)
{
	if (!response)
	{
		return;
	}

	// You can get original request type from: response->request->reqType
	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		CCLOG("%s completed", response->getHttpRequest()->getTag());
	}

	int statusCode = response->getResponseCode();
	char statusString[64] = {};
	sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
	CCLOG("response code: %d", statusCode);

	if (!response->isSucceed())
	{
		CCLOG("response failed");
		CCLOG("error buffer: %s", response->getErrorBuffer());
		isCheckFinsh = true;
		uInfo.isShowUpdateTip = false;
		return;
	}
	// dump data
	std::vector<char> *buffer = response->getResponseData();
	std::string bufffff(buffer->begin(), buffer->end());
	CCLOG("-:%s <<%s>>", bufffff.c_str(), __FUNCTION__);
	checkUpdate(bufffff.c_str());
	
	/*
	//保存到本地文件
	path += this->filename;
	CCLOG("path: %s", path.c_str());
	FILE *fp = fopen(path.c_str(), "wb+");
	fwrite(bufffff.c_str(), 1, buffer->size(), fp);
	fclose(fp);

	*/
}
//更新校验
void LogoScene::checkUpdate(const char* buf){
	//获取当前版本号
	std::string sCurVersion = platformAction("{\"act\":600}");
	//服务器版本号
	std::string sServiceVersion;
	//adnroid下载url地址
	std::string urlAndroid;
	//iphone下载url地址
	std::string urlIphone;
	//anroid更新动作
	std::string strAndroidAction;
	//iphone更新动作
	std::string strIphoneAction;
	Json *root = Json_create(buf);
	if (root)
	{
		Json* _date = Json_getItem(root, "updateConfig");
		
		Json* _strVar = Json_getItem(_date, "var");
		if (_strVar->type == Json_String)
		{
			sServiceVersion = _strVar->valuestring;
			CCLOG("var:%s <<%s>>", sServiceVersion.c_str(), __FUNCTION__);
		}
		Json* _strAndroidAction = Json_getItem(_date, "androidAction");
		if (_strAndroidAction->type == Json_String)
		{
			strAndroidAction = _strAndroidAction->valuestring;
			CCLOG("androidAction:%s <<%s>>", strAndroidAction.c_str(), __FUNCTION__);
		}
		Json* _strIphoneAction = Json_getItem(_date, "iphoneAction");
		if (_strIphoneAction->type == Json_String)
		{
			strIphoneAction = _strIphoneAction->valuestring;
			CCLOG("iphoneAction:%s <<%s>>", strIphoneAction.c_str(), __FUNCTION__);
		}
		Json* _strUpdateType = Json_getItem(_date, "updateType");
		if (_strUpdateType->type == Json_Number)
		{
			uInfo.iUpdateType = _strUpdateType->valueint;
			CCLOG("updateType:%d <<%s>>", uInfo.iUpdateType, __FUNCTION__);
		}
		Json* _strUpdateContent = Json_getItem(_date, "updateContent");
		if (_strUpdateContent->type == Json_String)
		{
			std::string sUpdateContent = _strUpdateContent->valuestring;
			uInfo.strUpdateContent = sUpdateContent;
			CCLOG("upcateContent:%s <<%s>>", sUpdateContent.c_str(), __FUNCTION__);
		}
		Json* _strUrlAndroid = Json_getItem(_date, "url_android");
		if (_strUrlAndroid->type == Json_String)
		{
			urlAndroid = _strUrlAndroid->valuestring;
			CCLOG("android:%s <<%s>>", urlAndroid.c_str(), __FUNCTION__);
		}
		Json* _strUrlIphone = Json_getItem(_date, "url_iphone");
		if (_strUrlIphone->type == Json_String)
		{
			urlIphone = _strUrlIphone->valuestring;
			CCLOG("iphone:%s <<%s>>", urlIphone.c_str(), __FUNCTION__);
		}
	}
	Json_dispose(root);
	if (sCurVersion==sServiceVersion)
	{
		uInfo.isShowUpdateTip = false;
	}
	else
	{
		uInfo.isShowUpdateTip = true;
	}
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	uInfo.strUpdateUrl = urlAndroid;
	uInfo.strUpdateAction = strAndroidAction;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	uInfo.strUpdateUrl=urlIphone;
	uInfo.strUpdateAction = strIphoneAction;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	uInfo.strUpdateUrl=urlAndroid;
	uInfo.strUpdateAction = strAndroidAction;
#endif

	isCheckFinsh = true;
}