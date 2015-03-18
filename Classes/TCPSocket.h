
#pragma once
#include "WinSock2.h"
class TCPSocket{
public:
	virtual DWORD __cdecl Connect(DWORD dwServerIP, WORD wPort);
	//连接操作
	virtual DWORD __cdecl Connect(const char* szServerIP, WORD wPort);
	//地址解释
	DWORD TranslateAddr(const char* pszServerAddr);
	//辅助变量
protected:
	//WORD							m_wSocketID;						//网络标识
	BYTE							m_cbSocketStatus;					//网络状态
	//内核变量
protected:
	SOCKET							m_hSocket;							//连接句柄
};
