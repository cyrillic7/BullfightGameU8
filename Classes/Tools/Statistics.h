//统计
#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
class Statistics:public CCNode {
public:
	enum StatisticsType
	{
		S_INSTALL=0,			//安装
		S_REGISTER,				//注册
		S_ACCOUNT_LOGON,		//帐号登录
	};
	CC_SYNTHESIZE(StatisticsType, eStatisticsType, StatisticsType);
public:
	Statistics();
	~Statistics();
	CREATE_FUNC(Statistics);
public:
	//发送统计数据
	void sendStatisticsData(int type);
private:
	//网络请求回调
	void onHttpRequestCompleted(cocos2d::CCNode *sender, void *data);
};
