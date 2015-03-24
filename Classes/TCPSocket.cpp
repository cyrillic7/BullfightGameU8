#include <stdio.h>
#include "TCPSocket.h"
#include "StructLogon.h"
#include "Tools.h"
#ifdef WIN32
#include <objbase.h>
#pragma comment(lib, "wsock32")
#else
//#include <uuid/uuid.h>
typedef struct _GUID
{
	unsigned long Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char Data4[8];
} GUID, UUID;

#define WSAEWOULDBLOCK                   10035L
#endif

#define ASSERT(_Expression)     ((void)0)
/*//发送字节映射表
BYTE TCPSocket::g_SendByteMap[256]=				
{
	0x70,0x2F,0x40,0x5F,0x44,0x8E,0x6E,0x45,0x7E,0xAB,0x2C,0x1F,0xB4,0xAC,0x9D,0x91,
	0x0D,0x36,0x9B,0x0B,0xD4,0xC4,0x39,0x74,0xBF,0x23,0x16,0x14,0x06,0xEB,0x04,0x3E,
	0x12,0x5C,0x8B,0xBC,0x61,0x63,0xF6,0xA5,0xE1,0x65,0xD8,0xF5,0x5A,0x07,0xF0,0x13,
	0xF2,0x20,0x6B,0x4A,0x24,0x59,0x89,0x64,0xD7,0x42,0x6A,0x5E,0x3D,0x0A,0x77,0xE0,
	0x80,0x27,0xB8,0xC5,0x8C,0x0E,0xFA,0x8A,0xD5,0x29,0x56,0x57,0x6C,0x53,0x67,0x41,
	0xE8,0x00,0x1A,0xCE,0x86,0x83,0xB0,0x22,0x28,0x4D,0x3F,0x26,0x46,0x4F,0x6F,0x2B,
	0x72,0x3A,0xF1,0x8D,0x97,0x95,0x49,0x84,0xE5,0xE3,0x79,0x8F,0x51,0x10,0xA8,0x82,
	0xC6,0xDD,0xFF,0xFC,0xE4,0xCF,0xB3,0x09,0x5D,0xEA,0x9C,0x34,0xF9,0x17,0x9F,0xDA,
	0x87,0xF8,0x15,0x05,0x3C,0xD3,0xA4,0x85,0x2E,0xFB,0xEE,0x47,0x3B,0xEF,0x37,0x7F,
	0x93,0xAF,0x69,0x0C,0x71,0x31,0xDE,0x21,0x75,0xA0,0xAA,0xBA,0x7C,0x38,0x02,0xB7,
	0x81,0x01,0xFD,0xE7,0x1D,0xCC,0xCD,0xBD,0x1B,0x7A,0x2A,0xAD,0x66,0xBE,0x55,0x33,
	0x03,0xDB,0x88,0xB2,0x1E,0x4E,0xB9,0xE6,0xC2,0xF7,0xCB,0x7D,0xC9,0x62,0xC3,0xA6,
	0xDC,0xA7,0x50,0xB5,0x4B,0x94,0xC0,0x92,0x4C,0x11,0x5B,0x78,0xD9,0xB1,0xED,0x19,
	0xE9,0xA1,0x1C,0xB6,0x32,0x99,0xA3,0x76,0x9E,0x7B,0x6D,0x9A,0x30,0xD6,0xA9,0x25,
	0xC7,0xAE,0x96,0x35,0xD0,0xBB,0xD2,0xC8,0xA2,0x08,0xF3,0xD1,0x73,0xF4,0x48,0x2D,
	0x90,0xCA,0xE2,0x58,0xC1,0x18,0x52,0xFE,0xDF,0x68,0x98,0x54,0xEC,0x60,0x43,0x0F
};

//接收字节映射表
BYTE TCPSocket::g_RecvByteMap[256] =
{
	0x51,0xA1,0x9E,0xB0,0x1E,0x83,0x1C,0x2D,0xE9,0x77,0x3D,0x13,0x93,0x10,0x45,0xFF,
	0x6D,0xC9,0x20,0x2F,0x1B,0x82,0x1A,0x7D,0xF5,0xCF,0x52,0xA8,0xD2,0xA4,0xB4,0x0B,
	0x31,0x97,0x57,0x19,0x34,0xDF,0x5B,0x41,0x58,0x49,0xAA,0x5F,0x0A,0xEF,0x88,0x01,
	0xDC,0x95,0xD4,0xAF,0x7B,0xE3,0x11,0x8E,0x9D,0x16,0x61,0x8C,0x84,0x3C,0x1F,0x5A,
	0x02,0x4F,0x39,0xFE,0x04,0x07,0x5C,0x8B,0xEE,0x66,0x33,0xC4,0xC8,0x59,0xB5,0x5D,
	0xC2,0x6C,0xF6,0x4D,0xFB,0xAE,0x4A,0x4B,0xF3,0x35,0x2C,0xCA,0x21,0x78,0x3B,0x03,
	0xFD,0x24,0xBD,0x25,0x37,0x29,0xAC,0x4E,0xF9,0x92,0x3A,0x32,0x4C,0xDA,0x06,0x5E,
	0x00,0x94,0x60,0xEC,0x17,0x98,0xD7,0x3E,0xCB,0x6A,0xA9,0xD9,0x9C,0xBB,0x08,0x8F,
	0x40,0xA0,0x6F,0x55,0x67,0x87,0x54,0x80,0xB2,0x36,0x47,0x22,0x44,0x63,0x05,0x6B,
	0xF0,0x0F,0xC7,0x90,0xC5,0x65,0xE2,0x64,0xFA,0xD5,0xDB,0x12,0x7A,0x0E,0xD8,0x7E,
	0x99,0xD1,0xE8,0xD6,0x86,0x27,0xBF,0xC1,0x6E,0xDE,0x9A,0x09,0x0D,0xAB,0xE1,0x91,
	0x56,0xCD,0xB3,0x76,0x0C,0xC3,0xD3,0x9F,0x42,0xB6,0x9B,0xE5,0x23,0xA7,0xAD,0x18,
	0xC6,0xF4,0xB8,0xBE,0x15,0x43,0x70,0xE0,0xE7,0xBC,0xF1,0xBA,0xA5,0xA6,0x53,0x75,
	0xE4,0xEB,0xE6,0x85,0x14,0x48,0xDD,0x38,0x2A,0xCC,0x7F,0xB1,0xC0,0x71,0x96,0xF8,
	0x3F,0x28,0xF2,0x69,0x74,0x68,0xB7,0xA3,0x50,0xD0,0x79,0x1D,0xFC,0xCE,0x8A,0x8D,
	0x2E,0x62,0x30,0xEA,0xED,0x2B,0x26,0xB9,0x81,0x7C,0x46,0x89,0x73,0xA2,0xF7,0x72
};
//数据加密密钥
const static DWORD g_dwPacketKey=0xA55AA55A;*/
const static DWORD g_dwPacketKey = 0xC39AC594A;
BYTE TCPSocket::g_SendByteMap[256] =
{
	0x61, 0x63, 0xF6, 0xA5, 0xF5, 0x5A, 0x07, 0xF0, 0x13, 0x12, 0x5C, 0x8B, 0xBC, 0xE1, 0x65, 0xD8,
	0x44, 0x8E, 0x6E, 0x45, 0x1F, 0xB4, 0xAC, 0x9D, 0x91, 0x70, 0x2F, 0x40, 0x5F, 0x7E, 0xAB, 0x2C,
	0xC1, 0x18, 0x52, 0xFE, 0x54, 0xEC, 0x60, 0x43, 0x0F, 0x90, 0xCA, 0xE2, 0x58, 0xDF, 0x68, 0x98,
	0x86, 0x83, 0xB0, 0x22, 0x26, 0x46, 0x4F, 0x6F, 0x2B, 0xE8, 0x00, 0x1A, 0xCE, 0x28, 0x4D, 0x3F,
	0x97, 0x95, 0x49, 0x84, 0x8F, 0x51, 0x10, 0xA8, 0x82, 0x72, 0x3A, 0xF1, 0x8D, 0xE5, 0xE3, 0x79,
	0xE4, 0xCF, 0xB3, 0x09, 0x34, 0xF9, 0x17, 0x9F, 0xDA, 0xC6, 0xDD, 0xFF, 0xFC, 0x5D, 0xEA, 0x9C,
	0x3C, 0xD3, 0xA4, 0x85, 0x47, 0x3B, 0xEF, 0x37, 0x7F, 0x87, 0xF8, 0x15, 0x05, 0x2E, 0xFB, 0xEE,
	0x24, 0x59, 0x89, 0x64, 0x5E, 0x3D, 0x0A, 0x77, 0xE0, 0xF2, 0x20, 0x6B, 0x4A, 0xD7, 0x42, 0x6A,
	0x8C, 0x0E, 0xFA, 0x8A, 0x57, 0x6C, 0x53, 0x67, 0x41, 0x80, 0x27, 0xB8, 0xC5, 0xD5, 0x29, 0x56,
	0x71, 0x31, 0xDE, 0x21, 0xBA, 0x7C, 0x38, 0x02, 0xB7, 0x93, 0xAF, 0x69, 0x0C, 0x75, 0xA0, 0xAA,
	0x1D, 0xCC, 0xCD, 0xBD, 0xAD, 0x66, 0xBE, 0x55, 0x33, 0x81, 0x01, 0xFD, 0xE7, 0x1B, 0x7A, 0x2A,
	0xD4, 0xC4, 0x39, 0x74, 0x14, 0x06, 0xEB, 0x04, 0x3E, 0x0D, 0x36, 0x9B, 0x0B, 0xBF, 0x23, 0x16,
	0x32, 0x99, 0xA3, 0x76, 0x9A, 0x30, 0xD6, 0xA9, 0x25, 0xE9, 0xA1, 0x1C, 0xB6, 0x9E, 0x7B, 0x6D,
	0x1E, 0x4E, 0xB9, 0xE6, 0x7D, 0xC9, 0x62, 0xC3, 0xA6, 0x03, 0xDB, 0x88, 0xB2, 0xC2, 0xF7, 0xCB,
	0x4B, 0x94, 0xC0, 0x92, 0x78, 0xD9, 0xB1, 0xED, 0x19, 0xDC, 0xA7, 0x50, 0xB5, 0x4C, 0x11, 0x5B,
	0xD0, 0xBB, 0xD2, 0xC8, 0xD1, 0x73, 0xF4, 0x48, 0x2D, 0xC7, 0xAE, 0x96, 0x35, 0xA2, 0x08, 0xF3
};

//接收映射
BYTE TCPSocket::g_RecvByteMap[256] =
{
	0x3A, 0xAA, 0x97, 0xD9, 0xB7, 0x6C, 0xB5, 0x06, 0xFE, 0x53, 0x76, 0xBC, 0x9C, 0xB9, 0x81, 0x28,
	0x46, 0xEE, 0x09, 0x08, 0xB4, 0x6B, 0xBF, 0x56, 0x21, 0xE8, 0x3B, 0xAD, 0xCB, 0xA0, 0xD0, 0x14,
	0x7A, 0x93, 0x33, 0xBE, 0x70, 0xC8, 0x34, 0x8A, 0x3D, 0x8E, 0xAF, 0x38, 0x1F, 0xF8, 0x6D, 0x1A,
	0xC5, 0x91, 0xC0, 0xA8, 0x54, 0xFC, 0xBA, 0x67, 0x96, 0xB2, 0x4A, 0x65, 0x60, 0x75, 0xB8, 0x3F,
	0x1B, 0x88, 0x7E, 0x27, 0x10, 0x13, 0x35, 0x64, 0xF7, 0x42, 0x7C, 0xE0, 0xED, 0x3E, 0xD1, 0x36,
	0xEB, 0x45, 0x22, 0x86, 0x24, 0xA7, 0x8F, 0x84, 0x2C, 0x71, 0x05, 0xEF, 0x0A, 0x5D, 0x74, 0x1C,
	0x26, 0x00, 0xD6, 0x01, 0x73, 0x0E, 0xA5, 0x87, 0x2E, 0x9B, 0x7F, 0x7B, 0x85, 0xCF, 0x12, 0x37,
	0x19, 0x90, 0x49, 0xF5, 0xB3, 0x9D, 0xC3, 0x77, 0xE4, 0x4F, 0xAE, 0xCE, 0x95, 0xD4, 0x1D, 0x68,
	0x89, 0xA9, 0x48, 0x31, 0x43, 0x63, 0x30, 0x69, 0xDB, 0x72, 0x83, 0x0B, 0x80, 0x4C, 0x11, 0x44,
	0x29, 0x18, 0xE3, 0x99, 0xE1, 0x41, 0xFB, 0x40, 0x2F, 0xC1, 0xC4, 0xBB, 0x5F, 0x17, 0xCD, 0x57,
	0x9E, 0xCA, 0xFD, 0xC2, 0x62, 0x03, 0xD8, 0xEA, 0x47, 0xC7, 0x9F, 0x1E, 0x16, 0xA4, 0xFA, 0x9A,
	0x32, 0xE6, 0xDC, 0x52, 0x15, 0xEC, 0xCC, 0x98, 0x8B, 0xD2, 0x94, 0xF1, 0x0C, 0xA3, 0xA6, 0xBD,
	0xE2, 0x20, 0xDD, 0xD7, 0xB1, 0x8C, 0x59, 0xF9, 0xF3, 0xD5, 0x2A, 0xDF, 0xA1, 0xA2, 0x3C, 0x51,
	0xF0, 0xF4, 0xF2, 0x61, 0xB0, 0x8D, 0xC6, 0x7D, 0x0F, 0xE5, 0x58, 0xDA, 0xE9, 0x5A, 0x92, 0x2D,
	0x78, 0x0D, 0x2B, 0x4E, 0x50, 0x4D, 0xD3, 0xAC, 0x39, 0xC9, 0x5E, 0xB6, 0x25, 0xE7, 0x6F, 0x66,
	0x07, 0x4B, 0x79, 0xFF, 0xF6, 0x04, 0x02, 0xDE, 0x6A, 0x55, 0x82, 0x6E, 0x5C, 0xAB, 0x23, 0x5B
};
//TCPSocket::TCPSocket(){
//	m_dwSendPacketCount = 0;
//}
TCPSocket::TCPSocket(SOCKET sock) {
    m_sock = sock;

	m_wRecvSize = 0;
	m_dwSendPacketCount = 0;
	m_dwRecvPacketCount = 0;
	m_dwSendTickCount = 0;
						
	//m_dwSendXorKey = 0x12345678;
	//m_dwRecvXorKey = 0x12345678;

	m_wRecvSize = 0;
	m_cbSendRound = 0;
	m_cbRecvRound = 0;
	m_dwSendXorKey = 0;
	m_dwRecvXorKey = 0;
	m_dwSendTickCount = 0;
	m_dwRecvTickCount = 0;
	m_dwSendPacketCount = 0;
	m_dwRecvPacketCount = 0;
	m_nRecHit = 0;
	//m_cbSocketStatus=SHUT_REASON_NORMAL;
	
	//m_ProxyInfo.wProxyPort=0;
	//m_ProxyInfo.cbProxyType=enProxyServerType::ProxyType_None;
	/*uuid_t uu;
	    int i;
	    uuid_generate(uu);
	
		    for (i = 0; i < 16; i++)
		    { 
		        printf("%02X-", uu[i]);
		    }
	   printf("\n");*/
	
	   

}

TCPSocket::~TCPSocket() {
}

int TCPSocket::Init() {
#ifdef WIN32
    WSADATA wsaData;
    //#define MAKEWORD(a,b) ((WORD) (((BYTE) (a)) | ((WORD) ((BYTE) (b))) << 8))
    WORD version = MAKEWORD(2, 0);
    int ret = WSAStartup(version, &wsaData); //win sock start up
    if (ret) {
        //		cerr << "Initilize winsock error !" << endl;
        return -1;
    }
#endif
	
    return 0;
}
//this is just for windows
int TCPSocket::Clean() {
#ifdef WIN32
    return (WSACleanup());
#endif
    return 0;
}

TCPSocket& TCPSocket::operator =(SOCKET s) {
    m_sock = s;
    return (*this);
}

TCPSocket::operator SOCKET() {
    return m_sock;
}
//create a socket object win/lin is the same
// af:
bool TCPSocket::Create(int af, int type, int protocol) {
    m_sock = socket(af, type, protocol);
    if (m_sock == INVALID_SOCKET) {
        return false;
    }
    return true;
}

bool TCPSocket::Connect(const char* ip, unsigned short port) {
    struct sockaddr_in svraddr;
    svraddr.sin_family = AF_INET;
    svraddr.sin_addr.s_addr = inet_addr(ip);
    svraddr.sin_port = htons(port);
    int ret = connect(m_sock, (struct sockaddr*) &svraddr, sizeof(svraddr));
    if (ret == SOCKET_ERROR) {
        return false;
    }
    return true;
}

bool TCPSocket::Bind(unsigned short port) {
    struct sockaddr_in svraddr;
    svraddr.sin_family = AF_INET;
    svraddr.sin_addr.s_addr = INADDR_ANY;
    svraddr.sin_port = htons(port);
    
    int opt = 1;
    if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char*) &opt, sizeof(opt))
        < 0)
        return false;
    
    int ret = bind(m_sock, (struct sockaddr*) &svraddr, sizeof(svraddr));
    if (ret == SOCKET_ERROR) {
        return false;
    }
    return true;
}
//for server
bool TCPSocket::Listen(int backlog) {
    int ret = listen(m_sock, backlog);
    if (ret == SOCKET_ERROR) {
        return false;
    }
    return true;
}

bool TCPSocket::Accept(TCPSocket& s, char* fromip) {
    struct sockaddr_in cliaddr;
    socklen_t addrlen = sizeof(cliaddr);
    SOCKET sock = accept(m_sock, (struct sockaddr*) &cliaddr, &addrlen);
    if (sock == SOCKET_ERROR) {
        return false;
    }
    
    s = sock;
    if (fromip != NULL)
        sprintf(fromip, "%s", inet_ntoa(cliaddr.sin_addr));
    
    return true;
}

int TCPSocket::Select(){
    FD_ZERO(&fdR);
    FD_SET(m_sock, &fdR);
    struct timeval mytimeout;
    mytimeout.tv_sec=3;
    mytimeout.tv_usec=0;
    int result= select(m_sock,&fdR,NULL,NULL,NULL);
    // µ⁄“ª∏ˆ≤Œ ˝ « 0 ∫Õ sockfd ÷–µƒ◊Ó¥Û÷µº”“ª
    // µ⁄∂˛∏ˆ≤Œ ˝ « ∂¡ºØ, “≤æÕ « sockset
    // µ⁄»˝, Àƒ∏ˆ≤Œ ˝ «–¥ºØ∫Õ“Ï≥£ºØ, ‘⁄±æ≥Ã–Ú÷–∂ºŒ™ø’
    // µ⁄ŒÂ∏ˆ≤Œ ˝ «≥¨ ± ±º‰, º¥‘⁄÷∏∂® ±º‰ƒ⁄»‘√ª”–ø…∂¡, ‘Ú≥ˆ¥Ì
    
    //case -1:                            error handled by u;
    if(result==-1){
        return -1;
    }
    /*else if(result==0){
     return -4;
     }*/
    else {
        if(FD_ISSET(m_sock,&fdR)){
            return -2;
        }else {
            return -3;
        }
    }
}



int TCPSocket::Send(const char* buf, int len, int flags) {
    int bytes;
    int count = 0;
    
    while (count < len) {
        const char* a= buf + count;
        bytes = send(m_sock, buf + count, len - count, flags);
        if (bytes == -1 || bytes == 0)
            return -1;
        count += bytes;
    }
    
    return count;
}

int TCPSocket::Recv(char* buf, int len, int flags) {
    return (recv(m_sock, buf, len, flags));
}

int TCPSocket::Close() {
#ifdef WIN32
    return (closesocket(m_sock));
#else
    return (close(m_sock));
#endif
}

int TCPSocket::GetError() {
#ifdef WIN32
    return (WSAGetLastError());
#else
    return -1;
#endif
}

bool TCPSocket::DnsParse(const char* domain, char* ip) {
    struct hostent* p;
    if ((p = gethostbyname(domain)) == NULL)
        return false;
    
    sprintf(ip, "%u.%u.%u.%u", (unsigned char) p->h_addr_list[0][0],
            (unsigned char) p->h_addr_list[0][1],
            (unsigned char) p->h_addr_list[0][2],
            (unsigned char) p->h_addr_list[0][3]);
    
    return true;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//加密数据
WORD TCPSocket::EncryptBuffer(BYTE pcbDataBuffer[], WORD wDataSize, WORD wBufferSize)
{
	//效验参数
	ASSERT(wDataSize >= sizeof(TCP_Head));
	ASSERT(wBufferSize >= (wDataSize + 2 * sizeof(DWORD)));
	ASSERT(wDataSize <= (sizeof(TCP_Head)+SOCKET_TCP_PACKET));

	//调整长度
	WORD wEncryptSize = wDataSize - sizeof(TCP_Command), wSnapCount = 0;
	if ((wEncryptSize%sizeof(DWORD)) != 0)
	{
		wSnapCount = sizeof(DWORD)-wEncryptSize%sizeof(DWORD);
		memset(pcbDataBuffer + sizeof(TCP_Info)+wEncryptSize, 0, wSnapCount);
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
		Guid.Data1=121324L;
		Guid.Data2 = 22;
		Guid.Data3 = 34;
		for (int i= 0; i < 8; i++) {
			Guid.Data4[i]='s';
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
		memmove(pcbDataBuffer + sizeof(TCP_Head)+sizeof(DWORD), pcbDataBuffer + sizeof(TCP_Head), wDataSize);
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
WORD TCPSocket::CrevasseBuffer(BYTE pcbDataBuffer[], WORD wDataSize)
{
	//效验参数
	ASSERT(m_dwSendPacketCount > 0);
	ASSERT(wDataSize >= sizeof(TCP_Head));
	ASSERT(((TCP_Head *)pcbDataBuffer)->TCPInfo.wPacketSize == wDataSize);

	//调整长度
	WORD wSnapCount = 0;
	if ((wDataSize%sizeof(DWORD)) != 0)
	{
		wSnapCount = sizeof(DWORD)-wDataSize%sizeof(DWORD);
		memset(pcbDataBuffer + wDataSize, 0, wSnapCount);
	}

	//解密数据
	DWORD dwXorKey = m_dwRecvXorKey;
	DWORD * pdwXor = (DWORD *)(pcbDataBuffer + sizeof(TCP_Info));
	WORD  * pwSeed = (WORD *)(pcbDataBuffer + sizeof(TCP_Info));
	WORD wEncrypCount = (wDataSize + wSnapCount - sizeof(TCP_Info)) / 4;
	for (WORD i = 0; i<wEncrypCount; i++)
	{
		if ((i == (wEncrypCount - 1)) && (wSnapCount>0))
		{
			BYTE * pcbKey = ((BYTE *)&m_dwRecvXorKey) + sizeof(DWORD)-wSnapCount;
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
	if (cbCheckCode!=0)
	{
		printf("数据包效验码错误");
	}

	return wDataSize;
}
//随机映射
WORD TCPSocket::SeedRandMap(WORD wSeed)
{
	DWORD dwHold = wSeed;
	return (WORD)((dwHold = dwHold * 241103L + 2533101L) >> 16);
}

//映射发送数据
BYTE TCPSocket::MapSendByte(BYTE const cbData)
{
	BYTE cbMap = g_SendByteMap[(BYTE)(cbData + m_cbSendRound)];
	m_cbSendRound += 3;
	return cbMap;
}

//映射接收数据
BYTE TCPSocket::MapRecvByte(BYTE const cbData)
{
	BYTE cbMap = g_RecvByteMap[cbData] - m_cbRecvRound;
	m_cbRecvRound += 3;
	return cbMap;
}

//发送函数
DWORD __cdecl TCPSocket::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	//效验状态
	if (m_sock == INVALID_SOCKET) return false;
	//if (m_cbSocketStatus != SOCKET_STATUS_CONNECT) return false;

	//构造数据
	BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
	TCP_Head * pHead = (TCP_Head *)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;

	//加密数据
	WORD wSendSize = EncryptBuffer(cbDataBuffer, sizeof(TCP_Head), sizeof(cbDataBuffer));

	//发送数据
	return SendDataBuffer(cbDataBuffer, wSendSize);
}

//发送函数
DWORD __cdecl TCPSocket::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//效验状态
	if (m_sock == INVALID_SOCKET) return false;
	//if (m_cbSocketStatus != SOCKET_STATUS_CONNECT) return false;

	//效验大小
	ASSERT(wDataSize <= SOCKET_TCP_PACKET);
	if (wDataSize > SOCKET_TCP_PACKET) return false;

	//构造数据
	BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
	TCP_Head * pHead = (TCP_Head *)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;
	if (wDataSize > 0)
	{
		ASSERT(pData != NULL);
		CopyMemory(pHead + 1, pData, wDataSize);
	}

	//加密数据
	WORD wSendSize = EncryptBuffer(cbDataBuffer, sizeof(TCP_Head)+wDataSize, sizeof(cbDataBuffer));
	//发送数据
	return SendDataBuffer(cbDataBuffer, wSendSize);
}
//发送数据
DWORD TCPSocket::SendDataBuffer(void * pBuffer, WORD wSendSize)
{
	//效验参数
	ASSERT(wSendSize != 0);
	ASSERT(pBuffer != NULL);

	//发送数据
	WORD wSended = 0;
	while (wSended < wSendSize)
	{
		int iErrorCode = send(m_sock, (char *)pBuffer + wSended, wSendSize - wSended, 0);
		//int nError = GetLastError();
		if (iErrorCode == SOCKET_ERROR)
		{
			if (GetError() == WSAEWOULDBLOCK)
			{
				//m_dwSendTickCount = GetTickCount() / 1000L;
				m_dwSendTickCount = Tools::getMicroSeconds() / 1000L;
				return true;
			}
			return false;
		}
		wSended += iErrorCode;
	}
	//m_dwSendTickCount = GetTickCount() / 1000L;
	m_dwSendTickCount = Tools::getMicroSeconds()/1000L;
	return true;
}
//网络读取
long TCPSocket::OnSocketNotifyRead(unsigned int wParam, long lParam)
{
	try
	{
		//读取数据
		int iRetCode = recv(m_sock, (char *)m_cbRecvBuf + m_wRecvSize, sizeof(m_cbRecvBuf)-m_wRecvSize, 0);
		if (iRetCode == SOCKET_ERROR) return 0;
		ASSERT(m_dwSendPacketCount > 0);
		m_wRecvSize += iRetCode;
		//m_dwRecvTickCount = GetTickCount() / 1000L;
		m_dwRecvTickCount = Tools::getMicroSeconds() / 1000L;

		//变量定义
		WORD wPacketSize = 0;
		BYTE cbDataBuffer[SOCKET_TCP_PACKET + sizeof(TCP_Head)];
		TCP_Head * pHead = (TCP_Head *)m_cbRecvBuf;

		while (m_wRecvSize >= sizeof(TCP_Head))
		{
			//效验参数
			wPacketSize = pHead->TCPInfo.wPacketSize;
			//			ASSERT(pHead->CmdInfo.cbVersion==SOCKET_VER);   lhm delete
			ASSERT(wPacketSize <= (SOCKET_TCP_PACKET + sizeof(TCP_Head)));
			//			if (pHead->CmdInfo.cbVersion!=SOCKET_VER) throw TEXT("数据包版本错误");  lhm delete
			//if (wPacketSize > (SOCKET_TCP_PACKET + sizeof(TCP_Head))) throw TEXT("数据包太大");
			if (m_wRecvSize < wPacketSize) return 1;

			//拷贝数据
			m_dwRecvPacketCount++;
			CopyMemory(cbDataBuffer, m_cbRecvBuf, wPacketSize);
			m_wRecvSize -= wPacketSize;
			//MoveMemory(m_cbRecvBuf, m_cbRecvBuf + wPacketSize, m_wRecvSize);
			memmove(m_cbRecvBuf, m_cbRecvBuf + wPacketSize, m_wRecvSize);
			//解密数据
			WORD wRealySize = CrevasseBuffer(cbDataBuffer, wPacketSize);
			ASSERT(wRealySize >= sizeof(TCP_Head));

			//解释数据
			WORD wDataSize = wRealySize - sizeof(TCP_Head);
			void * pDataBuffer = cbDataBuffer + sizeof(TCP_Head);
			TCP_Command Command = ((TCP_Head *)cbDataBuffer)->CommandInfo;
			//内核命令
			if (Command.wMainCmdID == MDM_KN_COMMAND)
			{
				switch (Command.wSubCmdID)
				{
					case SUB_KN_DETECT_SOCKET:	//网络检测
					{
						m_nRecHit++;
						//发送数据
						SendData(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, pDataBuffer, wDataSize);
						break;
					}
				}
				continue;
			}
			//处理数据
			bool bSuccess = OnEventTCPSocketRead(m_wSocketID, Command, pDataBuffer, wDataSize);
			//if (bSuccess == false) throw TEXT("网络数据包处理失败");
			if (!bSuccess)
			{
				return bSuccess;
			}
		};
	}
	catch (char* pszError)
	{
		//int nError = GetLastError();
		Close();
		//CloseSocket(SHUT_REASON_EXCEPTION);
	}

	return 1;
}
