/*
 * GameLogic.h
 *	二人牛牛 logic
 */
#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
//如果是android平台需要定义宏
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID||CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
#define RtlCopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#define CopyMemory RtlCopyMemory
typedef unsigned char       BYTE;
#endif
//宏定义


#define MAX_COUNT					5									//最大数目

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//扑克类型
#define OX_VALUE0					0									//混合牌型
#define OX_THREE_SAME				12									//三条牌型
#define OX_FOUR_SAME				13									//四条牌型
#define OX_FOURKING					14									//天王牌型
#define OX_FIVEKING					15									//天王牌型
class GameLogic {
	
private:
	static BYTE 							m_cbCardListData[54];

public:
	GameLogic();
	~GameLogic();
public:
	//获取类型
	BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
	//获取牛牛
	bool GetOxCard(BYTE cbCardData[], BYTE cbCardCount);
	//获取数值
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//获取花色
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }
public:
	//排列扑克
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
public:
	//逻辑数值
	BYTE GetCardLogicValue(BYTE cbCardData);
	//对比扑克
	bool CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount, bool FirstOX, bool NextOX);
	bool CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);
};
