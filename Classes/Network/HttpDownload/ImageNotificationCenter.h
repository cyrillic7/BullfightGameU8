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

typedef void (CCObject::*SEL_ImageNotification)();
#define imagenotification_selector(_SELECTOR) (SEL_ImageNotification)(&_SELECTOR)

//图片结构
class imgstruct : public CCObject
{
public:
	imgstruct(const char* iName, const char* idStr, CCNode* node, bool mask)
	{
		imageName = std::string(iName);
		observerId = std::string(idStr);
		this->node = node;
		useMask = mask;
	}
	~imgstruct()
	{
	}

	std::string imageName;
	std::string observerId;
	CCNode* node;
	//是否使用遮罩
	bool useMask;
};

//图片监听，下载完成触发
class ImageNotificationCenter : public CCNotificationCenter
{
public:
	static ImageNotificationCenter* sharedImageNotificationCenter(void);

	CCString addObserver(const char *imageName, CCNode* node, bool useMask);
	void imageLoaded();
	void removeObserver(const char *name);

	void postNotification(const char *name, CCObject *object);

	void imageLoaded(CCObject *obj);

	static CCSprite* getSpriteFromWriteablePath(const char* name);
	//创建遮罩sprite
	static CCSprite* createMaskedSprite(CCSprite* src, const char* maskFile);
public:
	CCObject* m_ImageNotificationTarget;
	SEL_ImageNotification m_pImageNotificationSelector;
private:
	CCNotificationCenter        m_notificationCenter;
	int                         m_observerID;
};