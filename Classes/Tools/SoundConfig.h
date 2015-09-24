#pragma once

#define kMusicHundred "music/BACK_GROUND.m4a"                                 //百人牛牛背音乐
#define kMusicOtherOx "music/ox_backbround.m4a"                                 //其它牛牛背音乐


#define kSoundHundredAddGold "sound/ADD_GOLD.m4a"                                 //百人牛牛下注


#define kSoundOx(name) CCString::createWithFormat("sound/otherOx/ox%d.m4a",(name))->getCString() //牛牛类型

#define kSoundLost "sound/otherOx/GAME_LOST.m4a"							//失败
#define kSoundWin "sound/otherOx/GAME_WIN.m4a"								//胜利
#define kSoundEnd "sound/otherOx/GAME_END.m4a"								//他人游戏结束
#define kSoundWarn "sound/otherOx/GAME_WARN.m4a"							//倒计时警告
#define kSoundStart "sound/otherOx/GAME_START.m4a"							//游戏开始
#define kSoundSendCard "sound/otherOx/SEND_CARD.m4a"						//发牌

#define kSoundHundredWarn "sound/TIME_WARIMG.m4a"							//百人牛牛倒计时警告
#define kSoundHundredSendCard "sound//OUT_CARD.m4a"							//百人牛牛发牌
#define kSoundHundredAddGold "sound/ADD_GOLD.m4a"							//百人牛牛加注	
#define kSoundHundredStart "sound/GAME_START.m4a"							//百人牛牛开始
#define kSoundHundredEndDraw "sound/END_DRAW.m4a"							//百人结束未下注
#define kSoundHundredEndLost "sound/END_LOST.m4a"							//百人结束输
#define kSoundHundredEndWin "sound/END_WIN.m4a"								//百人结束赢
/*
#define kMusicGameMenu "music/m_menu.mp3"
#define kMusicGameIn "music/m_gameIn.mp3"

//主菜单////////////////////////////////////////////////////////////////////////
#define kSoundPlane "sound/s_plane.mp3"//飞机
#define kSoundPlaneBomb "sound/s_plane_bomb.mp3"

//剧情////////////////////////////////////////////////////////////////////////

#define kSoundKillPeople "sound/s_kill_people.wav"

#define kSoundBomb(name) name==0?"sound/s_gun_bomb0.mp3" : "sound/s_gun_bomb1.wav"

#define kSoundEnemyDeath(name) CCString::createWithFormat("sound/s_enemy_death%d.mp3",(name))->getCString()
#define kSoundBaseUnderAttack(name) CCString::createWithFormat("sound/s_base_under_attack%d.wav",(name))->getCString()

#define kSoundBuySucceed "sound/s_buy_succeed.mp3"
#define kSoundSwitchGun "sound/s_switch_gun.mp3"

#define kSoundShootBomb "sound/s_shoot_bomb.mp3"
#define kSoundSelect "sound/s_select.mp3"
#define kSoundBack "sound/s_back.mp3"
#define kSoundSkill1 "sound/s_skill_1.mp3"*/