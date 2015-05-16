#include "Tools.h"
#include "SimpleAudioEngine.h"
//#include "DataModel.h"
const char* Tools::s_cMusicPath = NULL;
using namespace CocosDenshion;
void Tools::setTransitionAnimation(int type, float time, CCScene *scene){
	switch (type) {
	case 0:
		CCDirector::sharedDirector()->replaceScene(scene);
		break;
	case 1:
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(time, scene));
		break;
	case 2:
		CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(time, scene));
		break;
	case 3:
		CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInL::create(time, scene));
		break;
	default:
		break;
	}
}
#pragma mark-
#pragma mark暂停游戏
void Tools::revursivelyPauseAllChildren(CCNode * node) {
	node->pauseSchedulerAndActions();
	CCObject * obj;
	CCARRAY_FOREACH(node -> getChildren(), obj) {
		CCNode * n = (CCNode *)obj;
		revursivelyPauseAllChildren(n);
	}
}
#pragma mark恢复游戏
void Tools::revursivelyResumeAllChildren(CCNode * node) {
	node->resumeSchedulerAndActions();
	CCObject * obj;
	CCARRAY_FOREACH(node -> getChildren(), obj) {
		CCNode * n = (CCNode *)obj;
		revursivelyResumeAllChildren(n);
	}
}
#pragma mark-
#pragma mark 存档
void Tools::saveBoolByRMS(const char *name, bool isBool){
	CCUserDefault::sharedUserDefault()->setBoolForKey(name, isBool);
	CCUserDefault::sharedUserDefault()->flush();
}
bool Tools::getBoolByRMS(const char *name){
	return CCUserDefault::sharedUserDefault()->getBoolForKey(name);
}
void Tools::saveIntByRMS(const char *name, int num){
	CCUserDefault::sharedUserDefault()->setIntegerForKey(name, num);
	CCUserDefault::sharedUserDefault()->flush();
}
int Tools::getIntByRMS(const char *name){
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(name);
}
void Tools::saveStringByRMS(const char *name, std::string str){
	CCUserDefault::sharedUserDefault()->setStringForKey(name, str);
	CCUserDefault::sharedUserDefault()->flush();
}
std::string Tools::getStringByRMS(const char *name){
	return CCUserDefault::sharedUserDefault()->getStringForKey(name);
}
void Tools::playMusic(const char *path){
	s_cMusicPath = path;
//	if (DataModel::isMusic) {
		if (!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic(path, true);
		}
		else if (s_cMusicPath != path)
		{
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic(path, true);
		}
	//}
}
void Tools::playSound(const char *path){
	/*if (DataModel::isSound) {
		if (DataModel::sharedDataModel()->_pDicSoundName)
		{
			CCObject *object = DataModel::sharedDataModel()->_pDicSoundName->objectForKey(path);
			if (!object)
			{
				DataModel::sharedDataModel()->_pDicSoundName->setObject(CCString::createWithFormat("%s", path), path);
			}
			if (DataModel::sharedDataModel()->_pDicSoundName->count() >= 20)
			{
				CCDictElement *el = NULL;
				CCDictionary *dic = DataModel::sharedDataModel()->_pDicSoundName;
				CCDICT_FOREACH(dic, el)
				{
					CCString * name = (CCString *)el->getObject();
					SimpleAudioEngine::sharedEngine()->unloadEffect(name->getCString());
				}

				DataModel::sharedDataModel()->_pDicSoundName->removeAllObjects();
			}
		}

		SimpleAudioEngine::sharedEngine()->playEffect(path);
	}*/
}
std::string Tools::get_date_now()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	struct cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now, NULL);
	struct tm *tm;
	time_t tp = now.tv_sec;
	tm = localtime(&tp);
	char date[64] = { 0 };
	sprintf(date, "%d,%02d,%02d,%02d,%02d,%02d", (int)tm->tm_year + 1900,
		(int)tm->tm_mon + 1, (int)tm->tm_mday,(int)tm->tm_hour,(int)tm->tm_min,(int)tm->tm_sec);
	return std::string(date);
#endif
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	struct tm *tm;
	time_t timep;
	time(&timep);
	tm = localtime(&timep);
	//int year = tm->tm_year + 1900;  
	//int month = tm->tm_mon + 1;  
	//int day = tm->tm_mday;  
	//int hour=tm->tm_hour;  
	//int minute=tm->tm_min;  
	//int second=tm->tm_sec;
	char time[64] = { 0 };
	sprintf(time, "%d,%02d,%02d,%02d,%02d,%02d", (int)tm->tm_year + 1900,
		(int)tm->tm_mon + 1, (int)tm->tm_mday, (int)tm->tm_hour,
		(int)tm->tm_min, (int)tm->tm_sec);
	return std::string(time);
#endif
}
int Tools::getTime(){
	int sec = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	struct cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now, NULL);
	struct tm *tm;
	time_t tp = now.tv_sec;
	tm = localtime(&tp);

	int tYDay = tm->tm_yday * 24 * 60 * 60;
	int tHour = tm->tm_hour * 60 * 60;
	int tMin = tm->tm_min * 60;
	int tSce = tm->tm_sec;

	sec = tYDay + tHour + tMin + tSce;
	//char date[64] = { 0 };
	//sprintf(date, "%d,%02d,%02d,%02d,%02d,%02d", (int)tm->tm_year + 1900,
	//(int)tm->tm_mon + 1, (int)tm->tm_mday, (int)tm->tm_hour, (int)tm->tm_min, (int)tm->tm_sec);
	//return std::string(date);
#endif
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	struct tm *tm;
	time_t timep;
	time(&timep);
	tm = localtime(&timep);
	//int year = tm->tm_year + 1900;  
	//int month = tm->tm_mon + 1;  
	//int day = tm->tm_mday;  
	//int hour=tm->tm_hour;  
	//int minute=tm->tm_min;  
	//int second=tm->tm_sec;
	//char time[64] = { 0 };
	//sprintf(time, "%d,%02d,%02d,%02d,%02d,%02d", (int)tm->tm_year + 1900,
	//(int)tm->tm_mon + 1, (int)tm->tm_mday, (int)tm->tm_hour,
	//(int)tm->tm_min, (int)tm->tm_sec);
	//return std::string(time);
	int tYDay = tm->tm_yday * 24 * 60 * 60;
	int tHour = tm->tm_hour * 60 * 60;
	int tMin = tm->tm_min * 60;
	int tSce = tm->tm_sec;


	sec = tYDay + tHour + tMin + tSce;
#endif

	return sec;
}
long Tools::getMicroSeconds(){
	struct cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now, NULL);
	//inline t= (now.tv_sec * 1000 + now.tv_usec / 1000);
	return now.tv_sec * 1000 + now.tv_usec;
}
/*#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "iconv\iconv.h"
#else
#include "../../../../libiconv/include/iconv.h"
#endif*/


#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "iconv\iconv.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include <iconv.h>
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../../../../libiconv/include/iconv.h"
#endif
static char g_GBKConvUTF8Buf[5000] = { 0 };


const char * Tools::GBKToUTF8(const char * strChar){
/*#ifdef WIN32*/
	iconv_t iconvH;
	//iconvH = iconv_open("unicode","ascii");
	iconvH = iconv_open("utf-8","gb2312");
	//iconvH = iconv_open("gbk","utf-8");
	if (iconvH == 0)
	{
		return NULL;
	}
	size_t strLength = strlen(strChar);
	size_t outLength = strLength<<2;
	size_t copyLength = outLength;
	memset(g_GBKConvUTF8Buf, 0, 5000);

	char* outbuf = (char*) malloc(outLength);
	char* pBuff = outbuf;
	memset( outbuf, 0, outLength);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (-1 == iconv(iconvH,&strChar, &strLength, &outbuf, &outLength))
	{
		free(pBuff);
		iconv_close(iconvH);
		return NULL;
	}
#else
	if (-1 == iconv(iconvH, (char **)&strChar, &strLength, &outbuf, &outLength))
	{
		free(pBuff);
		iconv_close(iconvH);
		return NULL;
	}
#endif
	
	memcpy(g_GBKConvUTF8Buf,pBuff,copyLength);
	free(pBuff);
	iconv_close(iconvH);
	return g_GBKConvUTF8Buf;
/*#else
	if (ucnv_convert == NULL)
	{
		openIcuuc();
	}
	if (ucnv_convert)
	{
		int err_code = 0;
		int len = strlen(strChar);
		char* str = new char[len * 2 + 10];
		memset(str, 0, len * 2 + 10);
		ucnv_convert("utf-8", "gb2312", str, len * 2 + 10, strChar, len, &err_code);
		//ucnv_convert("gb2312","utf-8", str, len * 2 + 10, strChar, len, &err_code);
		if (err_code == 0)
		{
			return str;
		}
	}
	char test[256] = "";
	char* str = new char[30];
	strcpy(str, test);
	return str;
#endif*/
}
/*
int Tools::strLength(const std::string &str)
{   
	if (typeid(str) == typeid(std::string) && str.length() > 0) {
		int len = str.length();
		std::vector <std::string> dump;
		int i = 0;
		while(i < len) {
			if (~(str.at(i) >> 8) == 0) {
				dump.push_back(str.substr(i, 3));
				i = i + 3;
			} else {
				dump.push_back(str.substr(i, 1));
				i = i + 1;
			}
		}
		return dump.size();
	} else {
		printf("str is not string\n");
		return 0;
	}
}
std::string Tools::createStringToLength(const std::string &str,int begin,int length)
{
	if (typeid(str) == typeid(std::string) && str.length() > 0) {
		int len = str.length();
		std::vector <std::string> dump;
		int i = 0;
		while(i < len) {
			if (~(str.at(i) >> 8) == 0) {
				dump.push_back(str.substr(i, 3));
				i = i + 3;
			} else {
				dump.push_back(str.substr(i, 1));
				i = i + 1;
			}
		}
		CCString *str=ccs("");
		for (int i = begin; i < begin+length; i++)
		{
			if (i>=dump.size())
			{
				continue;
			}
			CCLog("%s<<%s>>", dump[i].c_str(), __FUNCTION__);
			str=CCString::createWithFormat("%s%s",str->getCString(),dump[i].c_str());
		}
		if (dump.size()>length)
		{
			str=CCString::createWithFormat("%s...",str->getCString());
		}
		CCLog("lengStr:%d<<%s>>",dump.size(),__FUNCTION__);
		return str->getCString();
	} else {
		printf("str is not string\n");
		return "";
	}
}*/

//////////////////////////////////////////////////////////////////////////
std::vector<std::string> Tools::parseUTF8(const std::string &str)
{
	int l = str.length();
	std::vector<std::string> ret;
	ret.clear();
	for (int p = 0; p < l;)
	{
		int size = 0;
		unsigned char c = str[p];
		if (c < 0x80) {
			size = 1;
		}
		else if (c < 0xc2)
		{
			size = 2;
		}
		else if (c < 0xe0)
		{
			size = 2;
		}
		else if (c < 0xf0)
		{
			size = 3;
		}
		else if (c < 0xf8)
		{
			size = 4;
		}
		else if (c < 0xfc)
		{
			size = 5;
		}
		else if (c < 0xfe)
		{
			size = 6;
		}
		else
			size = 7;
		std::string temp = "";
		temp = str.substr(p, size);
		ret.push_back(temp);
		p += size;
	}
	return ret;
}

std::string Tools::subUTF8(const std::string &str, int from, int to)
{
	if (from > to) return "";
	if (str.length() < to) return "";
	std::vector<std::string> vstr = parseUTF8(str);
	if (to>vstr.size())
	{
		to = vstr.size();
	}
	std::vector<std::string>::iterator iter = vstr.begin();
	std::string res;
	std::string result;
	for (iter = (vstr.begin() + from); iter != (vstr.begin() + to); iter++)
	{
		res += *iter;
	}
	if (vstr.size()>from+to)
	{
		res += "...";
	}
	return res;
}

