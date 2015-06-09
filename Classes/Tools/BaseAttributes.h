/*
 * BaseAttributes.h
 *  基础属性
 *  Created on: 2015年3月16日
 *      Author: 恒
 */

#ifndef BASEATTRIBUTES_H_
#define BASEATTRIBUTES_H_

#include "cocos2d.h"
#include "GameConfig.h"
USING_NS_CC;
//#define GOODS_COUNT 4

class BaseAttributes :public CCObject{
public:
	std::string sSexBoyName;
	std::string sSexGirlName;
	std::string sGameFree;
	std::string sGameStart;
	std::string sGameEnd;
	std::string sLoading;
	std::string sOutTime;
	std::string sWaitCodeing;
	std::string sPasswordEmpty;					//密码不能为空！
	std::string sPasswordInconsistent;			//两次密码输入不一致，请重新输入！
	std::string sInsurePasswordLeng;			//密码必须大于8位以上
	std::string sInsurePasswordNum;				//密码不能全是数字	
	std::string sSaveLimit;						//存入的游戏币数量不能为空，请重新输入游戏币数量
	std::string sTakeOutLimit;					//取出的游戏币数量不能为空，请重新输入游戏币数量
	std::string sInsureNotEnough;				//您当前保险柜的游戏币余额不足，游戏币提取失败
	std::string sScoreNotEnough;				//您当前游戏币的可用余额不足，游戏币存入失败！
	std::string sBuyTypeAuction;				//购买数量
	std::string	sBuyTypeShop;					//兑换数量
	std::string sLottery;						//奖券
	std::string sAllPice;						//总价
	std::string sAuctionSell;					//卖出
	std::string sAuctionBuy;					//买入
	std::string sAuctionSeller;					//[卖家]
	std::string sAuctionBuyers;					//[买家]
	std::string sSelectAuctionGoods;			//请选择要拍卖的道具
	std::string	sInputAuctionGoodsPice;			//请正确填写拍卖单价
	std::string sInputAuctionGoodsNum;			//请正确填写拍卖数量

	int iCardOffsetX[MAX_PLAYER];
	int iCardOffsetY[MAX_PLAYER];
	int iCardOffsetSpace[MAX_PLAYER];
public:
	static BaseAttributes * sharedAttributes();
	virtual ~BaseAttributes();
	void initBaseAttrobutes();
	//扑克偏移
	void initCardOffset();
private:
	void initStr();
};

#endif /* BASEATTRIBUTES_H_ */
