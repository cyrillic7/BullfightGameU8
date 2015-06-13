/*
* HttpDownload.cpp
*
*  Created on: 2015年6月13日
*      Author: 恒
*/
#include "ImageNotificationCenter.h"
ImageNotificationCenter* ImageNotificationCenter::sharedImageNotificationCenter(void)
{
	static ImageNotificationCenter imageNotificationCenter;
	return &imageNotificationCenter;
}

CCString ImageNotificationCenter::addObserver(const char *imageName, CCNode* node, bool useMask)
{
	CCString* observerIDstr = CCString::createWithFormat("%d", m_observerID);

	m_notificationCenter.addObserver(this, callfuncO_selector(ImageNotificationCenter::imageLoaded), observerIDstr->getCString(),
		new imgstruct(imageName, observerIDstr->getCString(), node, useMask));

	m_observerID++;
	return observerIDstr->getCString();
}

void ImageNotificationCenter::removeObserver(const char *name)
{
	m_notificationCenter.removeObserver(this, name);
}

void ImageNotificationCenter::postNotification(const char *name, CCObject *object)
{
	m_notificationCenter.postNotification(name, object);
}

void ImageNotificationCenter::imageLoaded(CCObject *obj)
{
	imgstruct* img = (imgstruct*)obj;
	CCLOG("imageLoaded success,imageName:%s", img->imageName.c_str());
	CCSprite* sprite = ImageNotificationCenter::getSpriteFromWriteablePath(img->imageName.c_str());
	CCLOG("got sprite 0x%X", sprite);
	if (img->useMask)
	{
		img->node->addChild(ImageNotificationCenter::createMaskedSprite(sprite, "mask.png"));
	}
	else
	{
		float scale_ = (float)img->node->getContentSize().width / (float)sprite->getContentSize().width;
		sprite->setAnchorPoint(ccp(0.5f, 0.5f));
		//sprite->setScale(0.5f);
		img->node->addChild(sprite);
	}
	this->removeObserver(img->observerId.c_str());
	img->release();

	(m_ImageNotificationTarget->*m_pImageNotificationSelector)();
}


CCSprite* ImageNotificationCenter::getSpriteFromWriteablePath(const char* name)
{
	std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
	path += name;
	FILE* fp = fopen(path.c_str(), "rb");
	if (!fp)
	{
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buf = (char*)malloc(len);
	fread(buf, len, 1, fp);
	fclose(fp);
	CCImage* img = new CCImage;
	img->initWithImageData(buf, len);
	free(buf);
	cocos2d::CCTexture2D* texture = new cocos2d::CCTexture2D();
	bool isImg = texture->initWithImage(img);
	img->release();
	if (!isImg)
	{
		delete texture;
		return CCSprite::create("default.png");//加载资源并非图片时返回的默认图
	}
	CCSprite* sprite = CCSprite::createWithTexture(texture);
	texture->release();
	return sprite;
}

CCSprite* ImageNotificationCenter::createMaskedSprite(CCSprite* src, const char* maskFile)
{
	//创建遮罩 讲的还不错原理参考：//http://blog.csdn.net/azymq/article/details/11110019

	CCSprite * mask = CCSprite::create(maskFile);

	assert(src);
	assert(mask);

	CCSize srcContent = src->getContentSize();
	CCSize maskContent = mask->getContentSize();

	CCRenderTexture * rt = CCRenderTexture::create(srcContent.width, srcContent.height, kTexture2DPixelFormat_RGBA8888);

	float ratiow = srcContent.width / maskContent.width;
	float ratioh = srcContent.height / maskContent.height;
	mask->setScaleX(ratiow);
	mask->setScaleY(ratioh);

	mask->setPosition(ccp(srcContent.width / 2, srcContent.height / 2));
	src->setPosition(ccp(srcContent.width / 2, srcContent.height / 2));

	ccBlendFunc blendFunc2 = { GL_ONE, GL_ZERO };
	mask->setBlendFunc(blendFunc2);
	ccBlendFunc blendFunc3 = { GL_DST_ALPHA, GL_ZERO };
	src->setBlendFunc(blendFunc3);

	rt->begin();
	mask->visit();
	src->visit();
	rt->end();

	CCSprite * retval = CCSprite::createWithTexture(rt->getSprite()->getTexture());
	retval->setFlipY(true);

	return retval;
}
