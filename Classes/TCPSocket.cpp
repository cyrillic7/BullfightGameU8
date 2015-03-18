

#include "TCPSocket.h"
#pragma comment(lib,"ws2_32.lib")


//连接服务器
DWORD __cdecl TCPSocket::Connect(DWORD dwServerIP, WORD wPort)
{
	
		//建立 SOCKET
		m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_hSocket == INVALID_SOCKET) throw TEXT("SOCKET 创建失败");
	// 创建主套接字
	//m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//if (m_hSocket == INVALID_SOCKET) {
		//closeSocket();
		//return false;
	//}


	return 1;
}
//连接服务器
DWORD __cdecl TCPSocket::Connect(const char* szServerIP, WORD wPort)
{
	
	return Connect(TranslateAddr(szServerIP), wPort);
}
//解释服务器地址
DWORD TCPSocket::TranslateAddr(const char* pszServerAddr)
{
	////转化地址
	DWORD dwServerIP = inet_addr(pszServerAddr);
	if (dwServerIP == INADDR_NONE)
	{
		LPHOSTENT lpHost = gethostbyname(pszServerAddr);
		if (lpHost == NULL) return INADDR_NONE;
		dwServerIP = ((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	return dwServerIP;
}