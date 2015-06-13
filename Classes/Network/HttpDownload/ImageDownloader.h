/*
* HttpDownload.h
*
*  Created on: 2015年6月13日
*      Author: 恒
*/
#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
#include "ImageNotificationCenter.h"
class ImageDownloader : public cocos2d::CCObject
{

public:

	virtual bool init();

	void SendHttpRequest(CCObject* pTarget, SEL_ImageNotification pSelector, const char* url, CCNode* node, const char* filename);
	void HttpRequestComplete(cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response);

	CREATE_FUNC(ImageDownloader);
public:
	//观察者ID
	CCString    observerID;
	//下载的图片加载到当前layer层上
	CCNode*    node;
	//是否使用遮罩
	bool        useMask;
	//图片文件名
	//const char* filename;
	std::string filename;
};