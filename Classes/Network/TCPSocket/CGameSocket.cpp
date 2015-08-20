#include "CGameSocket.h"
#include "../CMD_Server/Packet.h"
#include "../../Tools/Tools.h"
#ifdef WIN32
#include <objbase.h>
#pragma comment(lib, "wsock32")
#else
typedef struct _GUID
{
	unsigned long Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char Data4[8];
} GUID, UUID;
#endif
bool CGameSocket::isConnect = true;
CGameSocket::CGameSocket()
	:pIGameSocket(NULL)
{
	resetData();
}
void CGameSocket::resetData(){
	// 初始化
	memset(m_bufOutput, 0, sizeof(m_bufOutput));
	memset(m_bufInput, 0, sizeof(m_bufInput));

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	fTime = 0;
	m_cbSendRound = 0;
	m_cbRecvRound = 0;
	m_dwSendXorKey = 0;
	m_dwRecvXorKey = 0;

	m_dwSendTickCount = 0;
	m_dwRecvTickCount = 0;
	m_dwSendPacketCount = 0;
	m_dwRecvPacketCount = 0;

	m_sockClient = INVALID_SOCKET;
	m_nInbufLen = 0;
	m_nInbufStart = 0;
	m_nOutbufLen = 0;

	setSocketState(SOCKET_STATE_FREE);
}
void CGameSocket::closeSocket()
{
#ifdef WIN32
	closesocket(m_sockClient);
	WSACleanup();
#else
	shutdown(m_sockClient,SHUT_RDWR);//避免延时卡住
	close(m_sockClient);
#endif
	//End();
}
void CGameSocket::Run(){
	
	sockaddr_in	addr_in;
	memset((void *)&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(port);
	addr_in.sin_addr.s_addr = ip;

	if (connect(m_sockClient, (sockaddr *)&addr_in, sizeof(addr_in)) == SOCKET_ERROR) {
		if (hasError()) {
			closeSocket();
			setSocketState(SOCKET_STATE_CONNECT_FAILURE);
			return ;
		}
		else	// WSAWOLDBLOCK
		{
			timeval timeout;
			timeout.tv_sec = iBlockSec;
			timeout.tv_usec = 0;
			fd_set writeset, exceptset;
			FD_ZERO(&writeset);
			FD_ZERO(&exceptset);
			FD_SET(m_sockClient, &writeset);
			FD_SET(m_sockClient, &exceptset);

			int ret = select(FD_SETSIZE, NULL, &writeset, &exceptset, &timeout);
			if (ret == 0 || ret < 0) {
				closeSocket();
				setSocketState(SOCKET_STATE_CONNECT_FAILURE);
				return ;
			}
			else	// ret > 0
			{
				ret = FD_ISSET(m_sockClient, &exceptset);
				if (ret)		// or (!FD_ISSET(m_sockClient, &writeset)
				{
					closeSocket();
					setSocketState(SOCKET_STATE_CONNECT_FAILURE);
					return ;
				}
			}
		}
	}



	struct linger so_linger;
	so_linger.l_onoff = 1;
	so_linger.l_linger = 500;
	setsockopt(m_sockClient, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));
	//设置socket状态为连接成功
	setSocketState(SOCKET_STATE_CONNECT_SUCCESS);

	//End();
	//CCLOG("createSocket----------------- <<%s>>", __FUNCTION__);
}
bool CGameSocket::Create(const char* pszServerIP, int nServerPort, int nBlockSec, bool bKeepAlive /*= FALSE*/)
{
	// 检查参数
	if (pszServerIP == 0 || strlen(pszServerIP) > 15) {
		return false;
	}
	if (eSocketState == SOCKET_STATE_CONNECT_ING)
	{
		return true;
	}
	
	resetData();
	setSocketState(SOCKET_STATE_CONNECT_ING);


	
#ifdef WIN32
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData);//win sock start up
	if (ret != 0) {
		return false;
	}
#endif

	// 创建主套接字
	m_sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sockClient == INVALID_SOCKET) {
		closeSocket();
		setSocketState(SOCKET_STATE_CONNECT_FAILURE);
		return false;
	}

	// 设置SOCKET为KEEPALIVE
	if (bKeepAlive)
	{
		int		optval = 1;
		if (setsockopt(m_sockClient, SOL_SOCKET, SO_KEEPALIVE, (char *)&optval, sizeof(optval)))
		{
			closeSocket();
			setSocketState(SOCKET_STATE_CONNECT_FAILURE);
			return false;
		}
	}

#ifdef WIN32
	DWORD nMode = 1;
	int nRes = ioctlsocket(m_sockClient, FIONBIO, &nMode);
	if (nRes == SOCKET_ERROR) {
		closeSocket();
		setSocketState(SOCKET_STATE_CONNECT_FAILURE);
		return false;
	}
#else
	// 设置为非阻塞方式
	fcntl(m_sockClient, F_SETFL, O_NONBLOCK);
#endif

	unsigned long serveraddr = inet_addr(pszServerIP);
	if (serveraddr == INADDR_NONE)	// 检查IP地址格式错误
	{
		closeSocket();
		setSocketState(SOCKET_STATE_CONNECT_FAILURE);
		return false;
	}
	ip = serveraddr;
	port = nServerPort;
	iBlockSec = nBlockSec;

	m_nInbufLen = 0;
	m_nInbufStart = 0;
	m_nOutbufLen = 0;
	fTime = 0;
	Start();
	/*
	sockaddr_in	addr_in;
	memset((void *)&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(nServerPort);
	addr_in.sin_addr.s_addr = serveraddr;

	if (connect(m_sockClient, (sockaddr *)&addr_in, sizeof(addr_in)) == SOCKET_ERROR) {
		if (hasError()) {
			closeSocket();
			return false;
		}
		else	// WSAWOLDBLOCK
		{
			timeval timeout;
			timeout.tv_sec = nBlockSec;
			timeout.tv_usec = 0;
			fd_set writeset, exceptset;
			FD_ZERO(&writeset);
			FD_ZERO(&exceptset);
			FD_SET(m_sockClient, &writeset);
			FD_SET(m_sockClient, &exceptset);

			int ret = select(FD_SETSIZE, NULL, &writeset, &exceptset, &timeout);
			if (ret == 0 || ret < 0) {
				closeSocket();
				return false;
			}
			else	// ret > 0
			{
				ret = FD_ISSET(m_sockClient, &exceptset);
				if (ret)		// or (!FD_ISSET(m_sockClient, &writeset)
				{
					closeSocket();
					return false;
				}
			}
		}
	}

	m_nInbufLen = 0;
	m_nInbufStart = 0;
	m_nOutbufLen = 0;

	struct linger so_linger;
	so_linger.l_onoff = 1;
	so_linger.l_linger = 500;
	setsockopt(m_sockClient, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));

	//设置socket状态为连接成功
	setSocketState(SOCKET_CONNECT_SUCCESS);*/
	return true;
}

bool CGameSocket::SendMsg(void* pBuf, int nSize)
{
	if (pBuf == 0 || nSize <= 0) {
		return false;
	}

	if (m_sockClient == INVALID_SOCKET) {
		return false;
	}
	// 检查通讯消息包长度
	int packsize = 0;
	packsize = nSize;

	// 检测BUF溢出
	if (m_nOutbufLen + nSize > OUTBUFSIZE) {
		// 立即发送OUTBUF中的数据，以清空OUTBUF。
		Flush();
		if (m_nOutbufLen + nSize > OUTBUFSIZE) {
			// 出错了
			Destroy(false);
			return false;
		}
	}
	// 数据添加到BUF尾
	memcpy(m_bufOutput + m_nOutbufLen, pBuf, nSize);
	m_nOutbufLen += nSize;
	return true;
}

bool CGameSocket::ReceiveMsg(void* pBuf, int& nSize)
{
	//检查参数
	if (pBuf == NULL || nSize <= 0) {
		return false;
	}

	if (m_sockClient == INVALID_SOCKET) {
		return false;
	}

	// 检查是否有一个消息(小于2则无法获取到消息长度)
	if (m_nInbufLen < sizeof(TCP_Head)) {
		//  如果没有请求成功  或者   如果没有数据则直接返回
		if (!recvFromSock() || m_nInbufLen < sizeof(TCP_Head)) {		// 这个m_nInbufLen更新了
			return false;
		}
	}


	// 计算要拷贝的消息的大小（一个消息，大小为整个消息的第一个16字节），因为环形缓冲区，所以要分开计算
	//int packsize = (unsigned char)m_bufInput[m_nInbufStart] +
	//	(unsigned char)m_bufInput[(m_nInbufStart + 1) % INBUFSIZE] * 256; // 注意字节序，高位+低位
	// 复制出一个信息头
	if (m_nInbufStart + sizeof(TCP_Head) > INBUFSIZE) {
		// 如果一个消息有回卷（被拆成两份在环形缓冲区的头尾）
		// 先拷贝环形缓冲区末尾的数据
		int copylen = INBUFSIZE - m_nInbufStart;
		memcpy(pBuf, m_bufInput + m_nInbufStart, copylen);

		// 再拷贝环形缓冲区头部的剩余部分
		memcpy((unsigned char *)pBuf + copylen, m_bufInput, sizeof(TCP_Head) - copylen);
	}
	else {
		// 消息没有回卷，可以一次拷贝出去
		memcpy(pBuf, m_bufInput + m_nInbufStart, sizeof(TCP_Head));
	}
	TCP_Head * pHead = (TCP_Head *)pBuf;
	int packsize = pHead->TCPInfo.wPacketSize;
	// 检测消息包尺寸错误 暂定最大16k
	if (packsize <= 0 || packsize > _MAX_MSGSIZE) {
		m_nInbufLen = 0;		// 直接清空INBUF
		m_nInbufStart = 0;
		return false;
	}

	// 检查消息是否完整(如果将要拷贝的消息大于此时缓冲区数据长度，需要再次请求接收剩余数据)
	if (packsize > m_nInbufLen) {
		// 如果没有请求成功   或者    依然无法获取到完整的数据包  则返回，直到取得完整包
		if (!recvFromSock() || packsize > m_nInbufLen) {	// 这个m_nInbufLen已更新
			return false;
		}
	}

	// 复制出一个消息
	if (m_nInbufStart + packsize > INBUFSIZE) {
		// 如果一个消息有回卷（被拆成两份在环形缓冲区的头尾）
		// 先拷贝环形缓冲区末尾的数据
		int copylen = INBUFSIZE - m_nInbufStart;
		memcpy(pBuf, m_bufInput + m_nInbufStart, copylen);

		// 再拷贝环形缓冲区头部的剩余部分
		memcpy((unsigned char *)pBuf + copylen, m_bufInput, packsize - copylen);
		nSize = packsize;
	}
	else {
		// 消息没有回卷，可以一次拷贝出去
		memcpy(pBuf, m_bufInput + m_nInbufStart, packsize);
		nSize = packsize;
	}

	//拷贝数据
	m_dwRecvPacketCount++;
	//解密数据
	unsigned short wRealySize = CrevasseBuffer((unsigned char*)pBuf, packsize);
	nSize = wRealySize;
	//解释数据
	unsigned short wDataSize = wRealySize - sizeof(TCP_Head);
	void * pDataBuffer = (unsigned char*)pBuf + sizeof(TCP_Head);
	TCP_Command Command = ((TCP_Head *)pBuf)->CommandInfo;
	//内核命令
	if (Command.wMainCmdID == MDM_KN_COMMAND)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_KN_DETECT_SOCKET:	//网络检测
		{
			//发送数据
			SendData(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, pDataBuffer, wDataSize);
			break;
		}
		}
	}

	// 重新计算环形缓冲区头部位置
	m_nInbufStart = (m_nInbufStart + packsize) % INBUFSIZE;
	m_nInbufLen -= packsize;
	return	true;
}

bool CGameSocket::hasError()
{
#ifdef WIN32
	int err = WSAGetLastError();
	if (err != WSAEWOULDBLOCK) {
#else
	int err = errno;
	if (err != EINPROGRESS && err != EAGAIN) {
#endif
		return true;
	}

	return false;
	}

// 从网络中读取尽可能多的数据，实际向服务器请求数据的地方
bool CGameSocket::recvFromSock(void)
{
	if (m_nInbufLen >= INBUFSIZE || m_sockClient == INVALID_SOCKET) {
		return false;
	}

	// 接收第一段数据
	int	savelen, savepos;			// 数据要保存的长度和位置
	if (m_nInbufStart + m_nInbufLen < INBUFSIZE)	{	// INBUF中的剩余空间有回绕
		savelen = INBUFSIZE - (m_nInbufStart + m_nInbufLen);		// 后部空间长度，最大接收数据的长度
	}
	else {
		savelen = INBUFSIZE - m_nInbufLen;
	}

	// 缓冲区数据的末尾
	savepos = (m_nInbufStart + m_nInbufLen) % INBUFSIZE;
	CHECKF(savepos + savelen <= INBUFSIZE);
	int inlen = recv(m_sockClient, m_bufInput + savepos, savelen, 0);
	if (inlen > 0) {
		// 有接收到数据
		m_nInbufLen += inlen;

		if (m_nInbufLen > INBUFSIZE) {
			return false;
		}

		// 接收第二段数据(一次接收没有完成，接收第二段数据)
		if (inlen == savelen && m_nInbufLen < INBUFSIZE) {
			int savelen = INBUFSIZE - m_nInbufLen;
			int savepos = (m_nInbufStart + m_nInbufLen) % INBUFSIZE;
			CHECKF(savepos + savelen <= INBUFSIZE);
			inlen = recv(m_sockClient, m_bufInput + savepos, savelen, 0);
			if (inlen > 0) {
				m_nInbufLen += inlen;
				if (m_nInbufLen > INBUFSIZE) {
					return false;
				}
			}
			else if (inlen == 0) {
				Destroy(false);
				return false;
			}
			else {
				// 连接已断开或者错误（包括阻塞）
				if (hasError()) {
					Destroy(false);
					return false;
				}
			}
		}
	}
	else if (inlen == 0) {
		Destroy(false);
		return false;
	}
	else {
		// 连接已断开或者错误（包括阻塞）
		if (hasError()) {
			Destroy(false);
			return false;
		}
	}

	return true;
}

bool CGameSocket::Flush(void)		//? 如果 OUTBUF > SENDBUF 则需要多次SEND（）
{
	if (m_sockClient == INVALID_SOCKET) {
		return false;
	}

	if (m_nOutbufLen <= 0) {
		return true;
	}

	// 发送一段数据
	int	outsize;
	outsize = send(m_sockClient, m_bufOutput, m_nOutbufLen, 0);
	if (outsize > 0) {
		// 删除已发送的部分
		if (m_nOutbufLen - outsize > 0) {
			memcpy(m_bufOutput, m_bufOutput + outsize, m_nOutbufLen - outsize);
		}

		m_nOutbufLen -= outsize;

		if (m_nOutbufLen < 0) {
			return false;
		}
	}
	else {
		if (hasError()) {
			Destroy(false);
			return false;
		}
	}

	return true;
}

bool CGameSocket::Check(void)
{
	// 检查状态
	if (m_sockClient == INVALID_SOCKET) {
		return false;
	}
	if (!isConnect)
	{
		Destroy(false);
		return false;
	}
	char buf[1];
	int	ret = recv(m_sockClient, buf, 1, MSG_PEEK);
	if (ret == 0) {
		Destroy(false);
		return false;
	}
	else if (ret < 0) {
		if (hasError()) {
			Destroy(false);
			return false;
		}
		else {	// 阻塞
			return true;
		}
	}
	else {	// 有数据
		return true;
	}
	return true;
}

void CGameSocket::Destroy(bool isActive)
{
	// 关闭
	struct linger so_linger;
	so_linger.l_onoff = 1;
	so_linger.l_linger = 500;
	int ret = setsockopt(m_sockClient, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));

	closeSocket();

	m_sockClient = INVALID_SOCKET;
	m_nInbufLen = 0;
	m_nInbufStart = 0;
	m_nOutbufLen = 0;

	memset(m_bufOutput, 0, sizeof(m_bufOutput));
	memset(m_bufInput, 0, sizeof(m_bufInput));
	if (isActive)
	{
		setSocketState(SOCKET_STATE_FREE);
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//发送函数
DWORD __cdecl CGameSocket::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	//效验状态
	if (m_sockClient == INVALID_SOCKET) return false;
	

	//构造数据
	BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
	TCP_Head * pHead = (TCP_Head *)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;

	//加密数据
	WORD wSendSize = EncryptBuffer(cbDataBuffer, sizeof(TCP_Head), sizeof(cbDataBuffer));

	//发送数据
	return SendMsg(cbDataBuffer, wSendSize);
}

//发送函数
DWORD __cdecl CGameSocket::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//效验状态
	if (m_sockClient == INVALID_SOCKET) return false;


	//效验大小
	//ASSERT(wDataSize <= SOCKET_TCP_PACKET);
	if (wDataSize > SOCKET_TCP_PACKET) return false;

	//构造数据
	BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
	TCP_Head * pHead = (TCP_Head *)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;
	if (wDataSize > 0)
	{
		assert(pData != NULL);
		CopyMemory(pHead + 1, pData, wDataSize);
	}

	//加密数据
	WORD wSendSize = EncryptBuffer(cbDataBuffer, sizeof(TCP_Head) + wDataSize, sizeof(cbDataBuffer));
	//发送数据
	return SendMsg(cbDataBuffer, wSendSize);
}
//加密数据
WORD CGameSocket::EncryptBuffer(BYTE pcbDataBuffer[], WORD wDataSize, WORD wBufferSize)
{
	//效验参数
	assert(wDataSize >= sizeof(TCP_Head));
	assert(wBufferSize >= (wDataSize + 2 * sizeof(DWORD)));
	assert(wDataSize <= (sizeof(TCP_Head) + SOCKET_TCP_PACKET));

	//调整长度
	WORD wEncryptSize = wDataSize - sizeof(TCP_Command), wSnapCount = 0;
	if ((wEncryptSize%sizeof(DWORD)) != 0)
	{
		wSnapCount = sizeof(DWORD) - wEncryptSize%sizeof(DWORD);
		memset(pcbDataBuffer + sizeof(TCP_Info) + wEncryptSize, 0, wSnapCount);
	}

	//效验码与字节映射
	BYTE cbCheckCode = 0;
	for (WORD i = sizeof(TCP_Info); i < wDataSize; i++)
	{
		cbCheckCode += pcbDataBuffer[i];
		pcbDataBuffer[i] = MapSendByte(pcbDataBuffer[i]);
	}

	//填写信息头
	TCP_Head * pHead = (TCP_Head *)pcbDataBuffer;
	pHead->TCPInfo.cbCheckCode = ~cbCheckCode + 1;
	pHead->TCPInfo.wPacketSize = wDataSize;
	//	pHead->CmdInfo.cbVersion=SOCKET_VER;  lhm delete

	//创建密钥
	DWORD dwXorKey = m_dwSendXorKey;
	if (m_dwSendPacketCount == 0)
	{
		//生成第一次随机种子
		GUID Guid;
		Guid.Data1 = abs(rand() % 10000);
		Guid.Data2 = abs(rand() % 100);
		Guid.Data3 = abs(rand() % 100);
		
		for (int i = 0; i < 8; i++) {
			Guid.Data4[i] = 's';
		}

		//CoCreateGuid(&Guid);
		//dwXorKey = GetTickCount()*GetTickCount();
		dwXorKey = Tools::getMicroSeconds();
		dwXorKey ^= Guid.Data1;
		dwXorKey ^= Guid.Data2;
		dwXorKey ^= Guid.Data3;
		dwXorKey ^= *((DWORD *)Guid.Data4);

		//随机映射种子
		dwXorKey = SeedRandMap((WORD)dwXorKey);
		dwXorKey |= ((DWORD)SeedRandMap((WORD)(dwXorKey >> 16))) << 16;
		dwXorKey ^= g_dwPacketKey;
		m_dwSendXorKey = dwXorKey;
		m_dwRecvXorKey = dwXorKey;
	}

	//加密数据
	WORD * pwSeed = (WORD *)(pcbDataBuffer + sizeof(TCP_Info));
	DWORD * pdwXor = (DWORD *)(pcbDataBuffer + sizeof(TCP_Info));
	WORD wEncrypCount = (wEncryptSize + wSnapCount) / sizeof(DWORD);
	for (int i = 0; i < wEncrypCount; i++)
	{
		*pdwXor++ ^= dwXorKey;
		dwXorKey = SeedRandMap(*pwSeed++);
		dwXorKey |= ((DWORD)SeedRandMap(*pwSeed++)) << 16;
		dwXorKey ^= g_dwPacketKey;
	}

	//插入密钥
	if (m_dwSendPacketCount == 0)
	{
		//MoveMemory(pcbDataBuffer + sizeof(TCP_Head)+sizeof(DWORD), pcbDataBuffer + sizeof(TCP_Head), wDataSize);
		memmove(pcbDataBuffer + sizeof(TCP_Head) + sizeof(DWORD), pcbDataBuffer + sizeof(TCP_Head), wDataSize);
		*((DWORD *)(pcbDataBuffer + sizeof(TCP_Head))) = m_dwSendXorKey;
		pHead->TCPInfo.wPacketSize += sizeof(DWORD);
		wDataSize += sizeof(DWORD);
	}

	//设置变量
	m_dwSendPacketCount++;
	m_dwSendXorKey = dwXorKey;
	return wDataSize;
}

//解密数据
WORD CGameSocket::CrevasseBuffer(BYTE pcbDataBuffer[], WORD wDataSize)
{
	//效验参数
	assert(m_dwSendPacketCount > 0);
	assert(wDataSize >= sizeof(TCP_Head));
	assert(((TCP_Head *)pcbDataBuffer)->TCPInfo.wPacketSize == wDataSize);

	//调整长度
	WORD wSnapCount = 0;
	if ((wDataSize%sizeof(DWORD)) != 0)
	{
		wSnapCount = sizeof(DWORD) - wDataSize%sizeof(DWORD);
		memset(pcbDataBuffer + wDataSize, 0, wSnapCount);
	}

	//解密数据
	DWORD dwXorKey = m_dwRecvXorKey;
	DWORD * pdwXor = (DWORD *)(pcbDataBuffer + sizeof(TCP_Info));
	WORD  * pwSeed = (WORD *)(pcbDataBuffer + sizeof(TCP_Info));
	WORD wEncrypCount = (wDataSize + wSnapCount - sizeof(TCP_Info)) / 4;
	for (WORD i = 0; i < wEncrypCount; i++)
	{
		if ((i == (wEncrypCount - 1)) && (wSnapCount > 0))
		{
			BYTE * pcbKey = ((BYTE *)&m_dwRecvXorKey) + sizeof(DWORD) - wSnapCount;
			CopyMemory(pcbDataBuffer + wDataSize, pcbKey, wSnapCount);
		}
		dwXorKey = SeedRandMap(*pwSeed++);
		dwXorKey |= ((DWORD)SeedRandMap(*pwSeed++)) << 16;
		dwXorKey ^= g_dwPacketKey;
		*pdwXor++ ^= m_dwRecvXorKey;
		m_dwRecvXorKey = dwXorKey;
	}

	//效验码与字节映射
	TCP_Head * pHead = (TCP_Head *)pcbDataBuffer;
	BYTE cbCheckCode = pHead->TCPInfo.cbCheckCode;
	for (int i = sizeof(TCP_Info); i < wDataSize; i++)
	{
		pcbDataBuffer[i] = MapRecvByte(pcbDataBuffer[i]);
		cbCheckCode += pcbDataBuffer[i];
	}
	//if (cbCheckCode != 0) throw TEXT("数据包效验码错误");//注：先不做处理
	if (cbCheckCode != 0)
	{
		CCLOG("数据包效验码错误");
	}

	return wDataSize;
}
//随机映射
WORD CGameSocket::SeedRandMap(WORD wSeed)
{
	DWORD dwHold = wSeed;
	return (WORD)((dwHold = dwHold * 241103L + 2533101L) >> 16);
}

//映射发送数据
BYTE CGameSocket::MapSendByte(BYTE const cbData)
{
	BYTE cbMap = g_SendByteMap[(BYTE)(cbData + m_cbSendRound)];
	m_cbSendRound += 3;
	return cbMap;
}

//映射接收数据
BYTE CGameSocket::MapRecvByte(BYTE const cbData)
{
	BYTE cbMap = g_RecvByteMap[cbData] - m_cbRecvRound;
	m_cbRecvRound += 3;
	return cbMap;
}

//更新socket收发数据
void CGameSocket::updateSocketData(float delta){
	
	if (getSocketState() == SOCKET_STATE_FREE) {
		return;
	}
	//超时判断
	fTime += delta;
	if (fTime >= OUT_TIME)
	{
		fTime = 0;
		setSocketState(SOCKET_STATE_ERROR);
	}
	if (getSocketState() == SOCKET_STATE_CONNECT_ING) {
		return;
	}
	//断开连接
	if (getSocketState() == SOCKET_STATE_ERROR) {
		pIGameSocket->onError("与服务器断开连接");
		setSocketState(CGameSocket::SOCKET_STATE_FREE);
		return;
	}
	//连接失败
	if (getSocketState() == SOCKET_STATE_CONNECT_FAILURE) {
		pIGameSocket->onError(" 连接服务器失败 ");
		setSocketState(CGameSocket::SOCKET_STATE_FREE);
		return;
	}
	if (!Check()) {
		//gameSocket = NULL;
		//onConnectionAbort();
		setSocketState(CGameSocket::SOCKET_STATE_ERROR);
		// 掉线了
		CCLOG("abort------------- <<%s>>", __FUNCTION__);
		return;
	}

	Flush();
	// 接收数据（取得缓冲区中的所有消息，直到缓冲区为空）
	while (true)
	{
		char buffer[_MAX_MSGSIZE] = { 0 };
		int nSize = sizeof(buffer);
		char* pbufMsg = buffer;
		if (getSocketState() != SOCKET_STATE_CONNECT_SUCCESS)
		{
			if (getSocketState() != SOCKET_STATE_FREE&&getSocketState() != SOCKET_STATE_CONNECT_ING)
			{
				setSocketState(SOCKET_STATE_ERROR);
			}
			break;
		}
		if (!ReceiveMsg(pbufMsg, nSize)) {
			break;
		}
		fTime = 0;
		// 接收数据（取得缓冲区中的所有消息，直到缓冲区为空）
		TCP_Head* pHead = (TCP_Head*)pbufMsg;
		WORD wDataSize = nSize - sizeof(TCP_Head);
		void * pDataBuffer = pbufMsg + sizeof(TCP_Head);
		pIGameSocket->onMessage(pHead->CommandInfo.wMainCmdID, pHead->CommandInfo.wSubCmdID, pDataBuffer, wDataSize);
		
	}
}
