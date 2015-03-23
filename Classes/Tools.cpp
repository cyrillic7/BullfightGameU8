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