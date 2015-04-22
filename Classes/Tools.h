#pragma 
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class Tools
{
public:
	static const char *s_cMusicPath;
public:
	static void setTransitionAnimation(int type, float time, CCScene *scene);
	//存档////////////////////////////////////////////////////////////////////////
	static void saveBoolByRMS(const char *name, bool isBool);
	static bool getBoolByRMS(const char *name);
	static void saveIntByRMS(const char *name, int num);
	static int getIntByRMS(const char *name);
	static void saveStringByRMS(const char *name, std::string str);
	static std::string getStringByRMS(const char *name);
	//暂停、恢复游戏////////////////////////////////////////////////////////////////////////
	static void revursivelyPauseAllChildren(CCNode * node);
	static void revursivelyResumeAllChildren(CCNode * node);

	//音乐////////////////////////////////////////////////////////////////////////
	static void playMusic(const char *path);
	static void playSound(const char *path);
	//GBK转UTF-8////////////////////////////////////////////////////////////////////////
	static const char * GBKToUTF8(const char *strChar);
	/*
	static void stopMusic();
	

	//循环音效
	static void playSoundLoop(const char *path);
	static void stopSoundLoop(const char *path);
	static void pauseSoundLoop();
	static void resumeSoundLoop();

	static void playRandAttack();

	static void releseSound();

	static float myRand_0_1();*/
	static std::string get_date_now();
	static int getTime();
	static long getMicroSeconds();
	//获取字符串长度（中英混合）
	static int strLength(const std::string &str);
	//
	static std::string createStringToLength(const std::string &str,int begin,int length);
};

