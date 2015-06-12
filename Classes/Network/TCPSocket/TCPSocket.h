/*
 *
 */
#pragma once
#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
#define NTAPI __stdcall
#else
#define _cdecl
#define __cdecl
#define NTAPI
#endif
#include "../CMD_Server/Define.h"
//如果是android平台需要定义宏
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID||CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
#define RtlCopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#define CopyMemory RtlCopyMemory
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef char TCHAR, *PTCHAR;
#endif


//长度定义
#define SOCKET_TCP_BUFFER			16384								//网络缓冲
#define SOCKET_TCP_PACKET			(SOCKET_TCP_BUFFER-sizeof(TCP_Head))//网络缓冲
//#include "ITCPSocket.h"
class SocketListerner;
class TCPSocket
{
public:
    TCPSocket(SOCKET sock = INVALID_SOCKET);
    ~TCPSocket();
    
    // Create socket object for snd/recv data
    bool Create(int af, int type, int protocol = 0);
    
    // Connect socket
    bool Connect(const char* ip, unsigned short port);
    //#region server
    // Bind socket
    bool Bind(unsigned short port);
    
    // Listen socket
    bool Listen(int backlog = 5);
    
    // Accept socket
    bool Accept(TCPSocket& s, char* fromip = NULL);
    //#endregion
    int Select();
    // Send socket
    int Send(const char* buf, int len, int flags = 0);
    
    // Recv socket
    int Recv(char* buf, int len, int flags = 0);
    
    // Close socket
    int Close();
    
    // Get errno
    int GetError();
    
    //#pragma region just for win32
    // Init winsock DLL
    static int Init();
    // Clean winsock DLL
    static int Clean();
    //#pragma endregion
    
    // Domain parse
    static bool DnsParse(const char* domain, char* ip);
    
    TCPSocket& operator = (SOCKET s);
    
    operator SOCKET ();
    
public:
	SOCKET m_sock;
	enum SocketState
	{
		SOCKET_STATE_FREE=0,					//空闲
		SOCKET_STATE_CONNECT_SUCCESS,			//连接成功
		SOCKET_STATE_CONNECT_FAILURE,			//连接失败
		SOCKET_STATE_ERROR,						//错误
		SOCKET_STATE_CLOSE,						//关闭
	}eSocketState;
protected:
   
    fd_set  fdR;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	//ITCPSocket *				m_pITCPSocket;					//回调接口
	//辅助变量
protected:
	WORD							m_wSocketID;						//网络标识
	BYTE							m_cbSocketStatus;					//网络状态
	int                             m_nRecHit;
	//接收变量
protected:
	WORD							m_wRecvSize;						//接收长度
	BYTE							m_cbRecvBuf[SOCKET_TCP_BUFFER * 10];		//接收缓冲
	//加密数据
protected:
	BYTE							m_cbSendRound;						//字节映射
	BYTE							m_cbRecvRound;						//字节映射
	DWORD							m_dwSendXorKey;						//发送密钥
	DWORD							m_dwRecvXorKey;						//接收密钥

	//BYTE static						g_SendByteMap[];
	//BYTE static						g_RecvByteMap[];
	//计数变量
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
	//监听
	 SocketListerner* listerner;


	//操作接口
public:
	//发送函数
	virtual DWORD __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID);
	//发送函数
	virtual DWORD __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID, void * const pData, WORD wDataSize);

	//辅助函数
protected:
	//关闭连接
	//VOID CloseSocket(BYTE cbShutReason);
	//缓冲数据
	//VOID AmortizeBuffer(VOID * pData, WORD wDataSize);
	//发送数据
	DWORD SendDataBuffer(void * pBuffer, WORD wSendSize);

private:
	const char* ip;
	unsigned short port;
	//消息函数
public:
	//网络读取
	//long OnSocketNotifyRead(WPARAM wParam, LPARAM lParam);
	long OnSocketNotifyRead(unsigned int wParam, long lParam);
	//监听
	void SetListerner(SocketListerner* listerner);
	SocketListerner *getListerner();
	void deleteListerner();

	void createSocket(const char* ip,unsigned short port,SocketListerner *listerner);
	int startSendThread();
	static void* socketNetworkThread(void* object);
	void initNetwork();
};
