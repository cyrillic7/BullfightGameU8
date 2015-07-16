/*
* HttpDownload.cpp
*
*  Created on: 2015年6月13日
*      Author: 恒
*/
#include "ImageDownloader.h"

bool ImageDownloader::init()
{
	useMask = false;
	return true;
}

void ImageDownloader::SendHttpRequest(CCObject* pTarget, SEL_ImageNotification pSelector, const char* url, CCNode* node, const char* filename)
{
	std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
	path += filename;
	if (CCFileUtils::sharedFileUtils()->isFileExist(CCFileUtils::sharedFileUtils()->fullPathForFilename(path.c_str())))
	{
		CCSprite* spr = CCSprite::create(path.c_str());
		node->addChild(spr);
		return;
	}

	CCHttpRequest* request = new CCHttpRequest();
	this->node = node;
	this->filename = filename;
	if (this->node)
	{
		this->observerID = ImageNotificationCenter::sharedImageNotificationCenter()->addObserver(filename, node, useMask);
	}
	request->setUrl(url);
	request->setRequestType(CCHttpRequest::kHttpGet);
	request->setResponseCallback(this, httpresponse_selector(ImageDownloader::HttpRequestComplete));
	ImageNotificationCenter::sharedImageNotificationCenter()->m_ImageNotificationTarget = pTarget;
	ImageNotificationCenter::sharedImageNotificationCenter()->m_pImageNotificationSelector = pSelector;
	request->setTag("GET IMAGE");
	CCHttpClient::getInstance()->send(request);
	request->release();
}

void ImageDownloader::HttpRequestComplete(CCHttpClient *sender, CCHttpResponse *response)
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
		return;
	}

	// dump data
	std::vector<char> *buffer = response->getResponseData();
	std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
	std::string bufffff(buffer->begin(), buffer->end());

	//保存到本地文件
	path += this->filename;
	CCLOG("path: %s", path.c_str());
	FILE *fp = fopen(path.c_str(), "wb+");
	fwrite(bufffff.c_str(), 1, buffer->size(), fp);
	fclose(fp);

	//传入node的下载请求会添加侦听，待下载完毕自动添加到node上
	if (this->node)
	{
		// node 是一个CCLayer ，用来显示动态加载的资源
		ImageNotificationCenter::sharedImageNotificationCenter()->postNotification(this->observerID.getCString(), NULL);
	}
}

