/* 
 * File:   SocketListerner.cpp
 * Author: beykery
 * 
 * Created on 2013年12月30日, 下午4:00
 */

#include "SocketListerner.h"
#include "cocos2d.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#pragma comment(lib, "wsock32")
#pragma comment(lib,"ws2_32.lib")
#include <winsock2.h>
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


SocketListerner::SocketListerner():context(NULL)
{
}

SocketListerner::~SocketListerner()
{
}

void SocketListerner::SetContext(TCPSocket* context)
{
    this->context = context;
}

void SocketListerner::Run()
{
	while (true)
	{
		bool isRead=context->OnSocketNotifyRead(0,0);
		if (!isRead)
		{
			break;
		}
		
	}
	this->OnClose(context, true);
}

