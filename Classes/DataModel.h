#pragma once
#include "cocos2d.h"
#include "MainScene.h"
#include "CMD_LogonServer.h"
USING_NS_CC;
class DataModel:public CCObject{
public:
	//音效开关
	static bool isSound;
	//音乐开关
	static bool isMusic;
	//是否坐下
	bool isSit;
	CCSize deviceSize;
	//登录帐号
	std::string sLogonAccount;
	//登录密码
	std::string sLogonPassword;
	//主场景对象
	CC_SYNTHESIZE(MainScene *, mainScene, MainScene);

	CCArray *m_aTagGameKind;

	std::vector <tagGameServer *> tagGameServerListOxTwo;
	std::vector <tagGameServer *> tagGameServerListOxHundred;
	//用户信息数组
	std::map<long ,tagUserInfo>mTagUserInfo;
	//登录成功信息
	//CMD_MB_LogonSuccess *logonSuccessUserInfo;
	//最大加注数
	long long m_lTurnMaxScore;
	//牌数组
	BYTE card[2][5];
	//变量定义
	tagUserInfo *userInfo;
	//消息队列
	std::queue<ReadData>readDataQueue;
public:
	DataModel();
	~DataModel();
	static DataModel* sharedDataModel();
private:
	void initDataModel();
public:
	//排序vector
	void sortVector(std::vector <tagGameServer *> vTagGameServer);
	//移除服务列表
	void removeTagGameServerList(std::vector <tagGameServer *> vTagGameServer);
};
