/**
*
*/
#ifndef _STRUCT_LOGON_H_
#define _STRUCT_LOGON_H_

//基本包
#ifdef WIN32
#include <winsock2.h>
typedef int				socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#endif


#pragma  pack(1)
//登录命令

#define MDM_GP_LOGON				1									//广场登录

//登录模式
#define SUB_GP_LOGON_GAMEID			1									//I D 登录
#define SUB_GP_LOGON_ACCOUNTS		2									//帐号登录
#define SUB_GP_REGISTER_ACCOUNTS	3									//注册帐号
#define SUB_GP_LOGON_LOGOUT_GAMEID  4                                   //退出大厅
#define SUB_GP_LOBBY_IP 			5									//比赛服务器ip

//登录结果
#define SUB_GP_LOGON_SUCCESS		100									//登录成功
#define SUB_GP_LOGON_FAILURE		101									//登录失败
#define SUB_GP_LOGON_FINISH			102									//登录完成
#define SUB_GP_VALIDATE_MBCARD      103                                 //登录失败

//升级提示
#define SUB_GP_UPDATE_NOTIFY		200									//升级提示


//////////////////////////////////////////////////////////////////////////
//登录命令
#define MDM_MB_LOGON				100									//广场登录

//登录模式
#define SUB_MB_LOGON_GAMEID			1									//I D 登录
#define SUB_MB_LOGON_ACCOUNTS		2									//帐号登录
#define SUB_MB_REGISTER_ACCOUNTS	3									//注册帐号

#define	LEN_MD5						33
#define	LEN_ACCOUNTS				32
#define LEN_MACHINE_ID				33
#define LEN_MOBILE_PHONE			12
#define LEN_PASS_PORT_ID			19
#define LEN_PHONE_VERIFY_ID			7

#define LEN_NICKNAME				32 									//昵称长度
#define LEN_SERVER					32 
#define LEN_KIND					32 
#define LEN_PROCESS					32 

//内核命令
#define MDM_KN_COMMAND				0									//内核命令
#define SUB_KN_DETECT_SOCKET		1									//检测命令
#define SUB_KN_VALIDATE_SOCKET		2									//验证命令

//////////////////////////////////////////////////////////////////////////////////
//列表命令

#define MDM_MB_SERVER_LIST			101									//列表信息

//列表信息
#define SUB_MB_LIST_KIND			100									//种类列表
#define SUB_MB_LIST_SERVER			101									//房间列表
#define SUB_MB_LIST_FINISH			200									//列表完成
struct TCP_Info
{
	BYTE							cbDataKind;							//数据类型
	BYTE							cbCheckCode;						//效验字段
	WORD							wPacketSize;						//数据大小
};

//网络命令
struct TCP_Command
{
	WORD							wMainCmdID;							//主命令码
	WORD							wSubCmdID;							//子命令码
};

//网络包头
struct TCP_Head
{
	TCP_Info						TCPInfo;							//基础结构
	TCP_Command						CommandInfo;						//命令信息
};


//帐号登录
struct CMD_MB_LogonAccounts
{
	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		 //身份证号（身份证绑定使用）
	TCHAR							szPhoneVerifyID[LEN_PHONE_VERIFY_ID];//手机验证号（手机绑定使用）
};
//登录失败
struct CMD_MB_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};
//登录成功
struct CMD_MB_LogonSuccess
{
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};
//游戏房间
struct tagGameServer
{
	WORD							wKindID;							//名称索引
	WORD							wNodeID;							//节点索引
	WORD							wSortID;							//排序索引
	WORD							wServerID;							//房间索引
	WORD							wServerPort;						//房间端口
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwFullCount;						//满员人数
	TCHAR							szServerAddr[32];					//房间名称
	TCHAR							szServerName[LEN_SERVER];			//房间名称
	TCHAR							szGameLevel[LEN_SERVER];			//场次
	TCHAR							szDescription[LEN_SERVER];			//房间描述
	WORD							wServerType;						//房间类型 0,默认房间，1新房间、2热门房间
};
//游戏种类
struct tagGameKind
{
	WORD							wTypeID;							//类型索引
	WORD							wJoinID;							//挂接索引
	WORD							wSortID;							//排序索引
	WORD							wKindID;							//类型索引
	WORD							wGameID;							//模块索引
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwFullCount;						//满员人数
	TCHAR							szKindName[LEN_KIND];				//游戏名字
	TCHAR							szProcessName[LEN_PROCESS];			//进程名字
};

#pragma pack()
#endif