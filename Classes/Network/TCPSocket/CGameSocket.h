#pragma once

#include "cocos2d.h"
USING_NS_CC;
#include "../ListernerThread/Thread.h"

#ifdef WIN32
#include <windows.h>
#include <WinSock.h>
#else
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef int				SOCKET;
//#define SOCKET int
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

#endif

#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
#define NTAPI __stdcall
#else
#define _cdecl
#define __cdecl
#define NTAPI
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID||CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
#define RtlCopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#define CopyMemory RtlCopyMemory
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef char TCHAR, *PTCHAR;
#endif

#ifndef CHECKF
#define CHECKF(x) \
	do \
				{ \
	if (!(x)) { \
	CCLog("CHECKF", #x, __FILE__, __LINE__); \
	return 0; \
					} \
				} while (0)
#endif

#define _MAX_MSGSIZE 16 * 1024		// 暂定一个消息最大为16k
#define BLOCKSECONDS	30			// INIT函数阻塞时间
#define INBUFSIZE	(64*1024)		//?	具体尺寸根据剖面报告调整  接收数据的缓存
#define OUTBUFSIZE	(8*1024)		//? 具体尺寸根据剖面报告调整。 发送数据的缓存，当不超过8K时，FLUSH只需要SEND一次
#define OUT_TIME		35			//超时断开
struct IGameSocket//socket回调接口
{
	virtual ~IGameSocket(){}
	//virtual void onClose(bool fromRemote)=0;
	virtual void onError(const char* e)=0;
	//virtual void onIdle()=0;
	virtual bool onMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize) = 0;
	virtual void onOpen()=0;
};
class CGameSocket:public Thread {
public:
	CC_SYNTHESIZE(IGameSocket*, pIGameSocket, IGameSocket);
	static bool isConnect;
	enum SocketState
	{
		SOCKET_STATE_FREE=0,							//空闲
		SOCKET_STATE_CONNECT_SUCCESS,					//连接成功
		SOCKET_STATE_CONNECT_FAILURE,					//连接失败
		SOCKET_STATE_CONNECT_ING,						//连接中
		SOCKET_STATE_ERROR,								//错误
		SOCKET_,
	};
	CC_SYNTHESIZE(SocketState, eSocketState, SocketState);
private:
	unsigned long ip;
	unsigned short port;
	int iBlockSec;
	//计时
	float fTime;
public:
	CGameSocket(void);
	bool	Create(const char* pszServerIP, int nServerPort, int nBlockSec = BLOCKSECONDS, bool bKeepAlive = false);
	bool	SendMsg(void* pBuf, int nSize);
	bool	ReceiveMsg(void* pBuf, int& nSize);
	bool	Flush(void);
	bool	Check(void);
	void	Destroy(bool isActive);
	void	updateSocketData(float delta);			//更新socket收发数据
	SOCKET	GetSocket(void) const { return m_sockClient; }
private:
	void resetData();
	virtual void Run();

	bool	recvFromSock(void);		// 从网络中读取尽可能多的数据
	bool    hasError();			// 是否发生错误，注意，异步模式未完成非错误
	void    closeSocket();

	SOCKET	m_sockClient;

	// 发送数据缓冲
	char	m_bufOutput[OUTBUFSIZE];	//? 可优化为指针数组
	int		m_nOutbufLen;

	// 环形缓冲区
	char	m_bufInput[INBUFSIZE];
	int		m_nInbufLen;
	int		m_nInbufStart;				// INBUF使用循环式队列，该变量为队列起点，0 - (SIZE-1)
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
public:
	//发送函数
	virtual DWORD __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID);
	//发送函数
	virtual DWORD __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID, void * const pData, WORD wDataSize);

	//加密数据
protected:
	BYTE							m_cbSendRound;						//字节映射
	BYTE							m_cbRecvRound;						//字节映射
	DWORD							m_dwSendXorKey;						//发送密钥
	DWORD							m_dwRecvXorKey;						//接收密钥
protected:
	DWORD							m_dwSendTickCount;					//发送时间
	DWORD							m_dwRecvTickCount;					//接收时间
	DWORD							m_dwSendPacketCount;				//发送计数
	DWORD							m_dwRecvPacketCount;				//接受计数
	//加密函数
protected:
	//解密数据
	WORD CrevasseBuffer(BYTE cbDataBuffer[], WORD wDataSize);
	//加密数据
	WORD EncryptBuffer(BYTE cbDataBuffer[], WORD wDataSize, WORD wBufferSize);

	//内联函数
private:
	//字节映射
	inline WORD SeedRandMap(WORD wSeed);
	//发送映射
	inline BYTE MapSendByte(BYTE cbData);
	//接收映射
	inline BYTE MapRecvByte(BYTE cbData);
};

