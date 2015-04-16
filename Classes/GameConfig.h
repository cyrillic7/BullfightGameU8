#pragma once


#define DEBUG_TEST 0 //0调试模式、1测试模式、2发布模式
#define GAME_VERSION "Version1.1.0"//游戏版本号

#define VERSION_FRAME				16777217
#define VERSION_CLIENT				17170433
#define VERSION_PLAZA 				17235969

#define LISTENER_NAME "listener_bullfight"//监听通知名称

#define SCENE_SIZE CCSize(1136,640)			//设计尺寸
//#define CCS_PATH_SCENE(name) CCString::createWithFormat("publish/%s",(name))->getCString()
#define CCS_PATH_SCENE(name) "publish/"#name
#define CCS_PATH_ANIMATION(name) "publish/animation/"#name

#define MAX_PLAYER							6						 //最大玩家数

//存档////////////////////////////////////////////////////////////////////////
#define RMS_IS_MUSIC "isMusic"
#define RMS_IS_SOUND "isSound"

//层级////////////////////////////////////////////////////////////////////////
#define K_Z_ORDER_HUD 10//UI层
#define K_Z_ORDER_POP 3//弹出层



//#define CCS_PATH_ANIMATION(name) CCString::createWithFormat("ccs/Animation/%s",(name))->getCString()

/*#define PTM_RATIO 32//box2d单位长度

#define TAG_ENEMY 1000 //敌人TAG
#define TAG_BULLET 1001 //子弹
#define TAG_POSITIONAL 1002//阵地
#define TAG_GOODS 1003 //爆炸物品
#define TAG_SKILL 1004 //技能
#define TAG_POSITIONAL 1005//基地
#define TAG_SKILL_HAND 1006//引导技能1的手指
#define TAG_SKILL_TUOZHUAI 1007//拖拽文字

#define K_Z_ORDER_EFFECT 480+1//效果（受击飙血）
#define K_Z_ORDER_POSITIONAL 480+2//阵地
#define K_Z_ORDER_BULLET 480+3 //子弹层

#define K_Z_ORDER_SKILL 480+5//技能


#define K_Z_ORDER_GAME_OVER 480+8//游戏结算层

#define SHOP_WEAPON_COUNT 5//武器数量
#define SHOP_UPGRADE_COUNT 9 //升级技能模块数

#define RMS_GAME_COIN_COUNT "gameCoinCount"
#define RMS_UPGRADE "upgrade"
#define RMS_HAVE_WEAPON "haveWeapon"
//未使用////////////////////////////////////////////////////////////////////////

#define RMS_HAVE_PARTNER "havePartner"
#define RMS_WEAPON_GRADE "weaponGrade"
#define RMS_UNLOCK_PARTNER "unlockPartner"
#define RMS_UNLOCK_CHAPTER_0 "unlockChapter0"
#define RMS_PASS_STAR_0 "passStar0"
#define RMS_UNLOCK_CHAPTER_1 "unlockChapter1"
#define RMS_PASS_STAR_1 "passStar1"
#define RMS_UNLOCK_CHAPTER_2 "unlockChapter2"
#define RMS_PASS_STAR_2 "passStar2"
//#define RMS_UNLOCK_CHAPTER_3 "unlockChapter3"
//#define RMS_PASS_STAR_3 "passStar3"
//#define RMS_BULLET_COUNT_BY_GUN "bulletCountByGun"
//#define RMS_BULLET_COUNT_BY_MAGIC_WAND "bulletCountByMagicWand"
#define RMS_UNLOCK_TUTORIALS_INDEX "unlockTutorialsIndex"
//#define RMS_TEMP_SKILL_PROBABILITY "tempSkillProbability"*/





