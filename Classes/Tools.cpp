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