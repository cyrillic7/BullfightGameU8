#ifndef CMD_OX_HEAD_FILE
#define CMD_OX_HEAD_FILE
#pragma pack(1)
//如果是android平台需要定义宏
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID||CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
#define RtlCopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#define CopyMemory RtlCopyMemory
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef char TCHAR, *PTCHAR;
#endif

//////////////////////////////////////////////////////////////////////////
//公共宏定义


#define KIND_ID						430									//游戏 I D
#define GAME_PLAYER					6										//游戏人数
#define GAME_NAME					TEXT(" 6人换牌牛牛 ")						//游戏名字
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)					//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)					//程序版本

#define GAME_GENRE					(GAME_GENRE_GOLD|GAME_GENRE_MATCH)		//游戏类型
#define MAXCOUNT					5										//扑克数目

//结束原因
#define GER_NO_PLAYER				0x10									//没有玩家

//游戏状态
#define GS_TK_FREE					GAME_STATUS_FREE                        //等待开始
#define GS_TK_CALL					GAME_STATUS_PLAY						//叫庄状态
#define GS_TK_SCORE					GAME_STATUS_PLAY+1						//下注状态
#define GS_TK_PLAYING				GAME_STATUS_PLAY+2						//游戏进行
#define GS_TK_CHANGE                GAME_STATUS_PLAY+3                      //换牌

//用户状态
#define USEX_NULL                   0                                       //用户状态
#define USEX_PLAYING                1                                       //用户状态
#define USEX_DYNAMIC                2                                       //用户状态   

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_START				100									//游戏开始
#define SUB_S_ADD_SCORE					101									//加注结果
#define SUB_S_PLAYER_EXIT				102									//用户强退
#define SUB_S_SEND_CARD					103									//发牌消息
#define SUB_S_GAME_END					104									//游戏结束
#define SUB_S_OPEN_CARD					105									//用户摊牌
#define SUB_S_CALL_BANKER				106									//用户叫庄
#define SUB_S_GAME_BASE					107									//发送基数
#define SUB_S_CHANGE_CARD               108                                 //用户换牌
#define SUB_S_CHANGE_OPEN               109                                 //开牌
#define SUB_S_USER_OPEN                 110									//用户开牌

//游戏状态
struct CMD_S_StatusFree
{
	long long							lCellScore;							//基础积分
	long long								lScoreTax;                          //手续费
};

//游戏状态
struct CMD_S_GameBase
{
	long long							lCellScore;							//基础积分
	long long								lScoreTax;                          //手续费
};

//游戏状态
struct CMD_S_StatusCall
{
	WORD							    	wCallBanker;						//叫庄用户
	BYTE							        cbPlayStatus[GAME_PLAYER];          //用户状态
	long long								lScoreTax;                          //手续费
};

//游戏状态
struct CMD_S_StatusScore1
{
	//下注信息
	long long								lTurnMaxScore;						//最大下注
	//long long								lTurnLessScore;						//最小下注
	long long								lTableScore[GAME_PLAYER];			//下注数目
	BYTE								    cbPlayStatus[GAME_PLAYER];          //用户状态
	WORD							    	wBankerUser;						//庄家用户
	long long								lScoreTax;                          //手续费
};

//游戏状态
struct CMD_S_StatusPlay
{
	//状态信息	
	BYTE								    cbPlayStatus[GAME_PLAYER];          //用户状态
	long long								lTurnMaxScore;						//最大下注
	//long long								lTurnLessScore;						//最小下注
	long long								lTableScore[GAME_PLAYER];			//下注数目
	WORD								    wBankerUser;						//庄家用户
	long long								lScoreTax;                          //手续费

	//扑克信息
	BYTE							    	cbHandCardData[GAME_PLAYER][MAXCOUNT];//桌面扑克
	BYTE						      		bOxCard[GAME_PLAYER];				//牛牛数据
};

//用户叫庄
struct CMD_S_CallBanker
{
	WORD							     	wCallBanker;						//叫庄用户
	bool							    	bFirstTimes;						//首次叫庄
};

//游戏开始
struct CMD_S_GameStart
{
	//下注信息
	long long								lTurnMaxScore;						//最大下注
	WORD							     	wBankerUser;						//庄家用户
	long long								lScoreTax;                          //手续费
};

//用户下注
struct CMD_S_AddScore
{
	WORD							    	wAddScoreUser;						//加注用户
	long long								lAddScoreCount;						//加注数目
};

//游戏结束
struct CMD_S_GameEnd
{
	long long								lGameTax[GAME_PLAYER];				//游戏税收
	long long								lGameScore[GAME_PLAYER];			//游戏得分
	BYTE							     	cbCardData[GAME_PLAYER][MAXCOUNT];	//用户扑克
	WORD									wWinUser;							//赢的玩家
};

//发牌数据包
struct CMD_S_SendCard
{
	BYTE								    cbCardData[GAME_PLAYER][MAXCOUNT];	//用户扑克

	BYTE									cbPlayStatus[GAME_PLAYER];			//游戏状态///////////////////
};

//用户退出
struct CMD_S_PlayerExit
{
	WORD						      		wPlayerID;							//退出用户
};

//用户摊牌
struct CMD_S_Open_Card
{
	WORD							     	wPlayerID;							//摊牌用户
	BYTE							      	bOpen;								//摊牌标志
};

//用户换牌
struct CMD_S_ChangeCard
{
	bool                                    bChange;                            //换牌标志
	WORD                                    wPlayerID;                         //换牌用户
	BYTE                                    cbCardData;                        //换牌数据
	BYTE                                    cbOldCardData;                     //旧的扑克数据
};
//换牌状态
struct CMD_S_StatusChange
{
	BYTE                                    cbPlayStatus[GAME_PLAYER];
	WORD								    wBankerUser;						//庄家用户
	long long								lTurnMaxScore;						//最大下注
	long long								lTableScore[GAME_PLAYER];			//下注数目
	bool                                    bChange;							//是否换过牌
	long long								lScoreTax;                          //手续费
	//扑克信息
	BYTE							    	cbHandCardData[GAME_PLAYER][MAXCOUNT];//桌面扑克
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构
#define SUB_C_CALL_BANKER				1									//用户叫庄
#define SUB_C_ADD_SCORE					2									//用户加注
#define SUB_C_OPEN_CARD					3									//用户摊牌
#define SUB_C_LEAVE						4									//机器人离开
#define SUB_C_CHANGE_CARD               5                                   //用户换牌
#define SUB_C_OPEN_END                  6									//开牌结束

//用户叫庄
struct CMD_C_CallBanker
{
	BYTE							    	bBanker;							//做庄标志
};

//用户加注
struct CMD_C_AddScore
{
	long long								lScore;								//加注数目
};

//用户摊牌
struct CMD_C_OxCard
{
	BYTE							    	bOX;								//牛牛标志
};

//用户换牌
struct CMD_C_ChangeCard
{
	bool                                    bChange;                            //换牌标志
	WORD                                    wPlayer;                            //换牌用户
	BYTE                                    cbChangeCard;                       //换牌数据
};
//开牌扑克
struct CMD_S_PlayerOpen
{
	BYTE							     	cbCardData[GAME_PLAYER][MAXCOUNT];	//用户扑克
};
//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
