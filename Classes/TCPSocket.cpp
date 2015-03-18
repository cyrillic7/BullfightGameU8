#include <stdio.h>
#include "TCPSocket.h"

#ifdef WIN32
#pragma comment(lib, "wsock32")
#endif

TCPSocket::TCPSocket(SOCKET sock) {
    m_sock = sock;
}

TCPSocket::~TCPSocket() {
}

int TCPSocket::Init() {
#ifdef WIN32
    /*
     http://msdn.microsoft.com/zh-cn/vstudio/ms741563(en-us,VS.85).aspx
     
     typedef struct WSAData {
     WORD wVersion;								//winsock version
     WORD wHighVersion;							//The highest version of the Windows Sockets specification that the Ws2_32.dll can support
     char szDescription[WSADESCRIPTION_LEN+1];
     char szSystemStatus[WSASYSSTATUS_LEN+1];
     unsigned short iMaxSockets;
     unsigned short iMaxUdpDg;
     char FAR * lpVendorInfo;
     }WSADATA, *LPWSADATA;
     */
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
        printf("0000000000");
        return false;
    }
    printf("11111111111");
    return true;
}

bool TCPSocket::Connect(const char* ip, unsigned short port) {
    struct sockaddr_in svraddr;
    svraddr.sin_family = AF_INET;
    svraddr.sin_addr.s_addr = inet_addr(ip);
    svraddr.sin_port = htons(port);
    int ret = connect(m_sock, (struct sockaddr*) &svraddr, sizeof(svraddr));
    if (ret == SOCKET_ERROR) {
        printf("no-------");
        return false;
    }
    printf("ok------");
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
