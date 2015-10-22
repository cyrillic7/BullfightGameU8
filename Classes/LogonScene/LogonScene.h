//
//  LogonScene.h
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//
#ifndef __BullfightGame__LogonScene__
#define __BullfightGame__LogonScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../Tools/DataModel.h"
#include "../Tools/CStringAide.h"
#include "../PopDialogBox/PopDialogBoxRegistered.h"

#include "../Network/TCPSocket/CGameSocket.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
//更新信息
struct updateInfo
{
	//是否显示更新
	bool isShowUpdateTip;
	//更新说明
	std::string strUpdateContent;
	//更新类型（1强制更新）
	int iUpdateType;
	//更新动作(update:程序内更新 toUrl:跳转url更新)
	std::string strUpdateAction;
	//url地址
	std::string strUpdateUrl;
};
class LogonScene:public CCLayer,public CStringAide,public IPopAssistRegistered,public IGameSocket
{
private:
	enum LogonType
	{
		LOGON_ACCOUNT=1,					//帐号登录
		LOGON_QQ,							//QQ登录
		LOGON_REGISTER,						//注册
		LOGON_QUICK,						//快速登录
	};
	CC_SYNTHESIZE(LogonType, eLogonType, LogonType);
	std::string sRegisterAccount;
	std::string sRegisterNickname;
	std::string sRegisterPasswrod;
    
    UILayer *m_pWidget;

	CGameSocket gameSocket;
	//是否第一次登录游戏
	bool isFirstLogon;
public:
    static LogonScene* pLScene;
	bool isReadMessage;
	//统计类型
	enum StatisticsType
	{
		STATISTICS_REGISTER = 1,					//注册
		STATISTICS_LOGON,							//登录
		STATISTICS_QQ,								//QQ登录
	};
	CC_SYNTHESIZE(StatisticsType, eStatisticsType, StatisticsType);
public:
    LogonScene();
    ~LogonScene();
	virtual void onEnter();
	virtual void onExit();
    static CCScene* scene();
	static CCScene* scene(updateInfo uInfo);
    
    void closeWebView();
    void logonQQ(const char*id,const char*pwd);
    CREATE_FUNC(LogonScene);
private:
	void onMenuLogon(CCObject* pSender, TouchEventType type);
	//分享
	void onMenuShare(CCObject* pSender, TouchEventType type);

	void update(float delta);
	//默认登录
	void defaultLogon();
	//显示更新
	void showUpdateContent(updateInfo uInfo);
public:
	//登录游戏(帐号登录)
	void logonGameByAccount(float dt);
	//保存帐号
	void saveAccount();
private:
	//初始化签到信息
	void initSignInfo();
	//初始化更多帐号
	void initMoreAccount();
	//连接服务器
	void connectServer();
	//登录
	void logonGame();
	//注册
	void registeredGame();
	
	//网络回调////////////////////////////////////////////////////////////////////////
	virtual void onOpen();
	virtual void onError(const char* e);
	virtual bool onMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize);

	void onEventReadMessage(WORD wMainCmdID,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//socket连接成功
	//void onEventConnect(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//登录回调
	void onEventLogon(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//获取列表
	void onEventServerList(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize);
	//注册回调
	virtual void onRegistered(const char *sAccount, const char*sNickname, const char*sPassword);
	//是否是新帐号
	bool isNewAccount(const char *sAccount);
	
	//存档
	void readRMS();
	void initRSM();
	bool isHaveSaveFile();

	//快速登录
	void quickLogon();

	virtual void keyBackClicked();  //响应返回键  
	//统计安装
	void statisticsInstall();
};

#endif /* defined(__BullfightGame__LogonScene__) */
