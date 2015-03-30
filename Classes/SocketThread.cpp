#include "SocketThread.h"
#include "cocos2d.h"
//#include "ResPonseThread.h"
USING_NS_CC;
int SocketThread::start(){    	
	int errCode = 0;
	do{
		pthread_attr_t tAttr;
		errCode = pthread_attr_init(&tAttr);
		CC_BREAK_IF(errCode!=0);
		//但是上面这个函数其他内容则主要为你创建的线程设定为分离式
		errCode = pthread_attr_setdetachstate(&tAttr, PTHREAD_CREATE_DETACHED);
		if (errCode!=0) {
			pthread_attr_destroy(&tAttr);
			break;
		}		
		errCode = pthread_create(&pid,&tAttr,start_thread,this);
	}while (0);
	return errCode;
} 


void* SocketThread::start_thread(void *arg)   {  
	SocketThread* thred=(SocketThread*)arg;
	TCPSocket cdSocket;
	cdSocket.Init();	
	bool isok=cdSocket.Create(AF_INET,SOCK_STREAM,0);	
	bool iscon=cdSocket.Connect("125.88.145.41", 8100);
	if(iscon){
		thred->state=0;
		//ResPonseThread::GetInstance()->start();// 启动响应参数
		CCLOG("conection");
	}else{
		thred->state=1;
	}	
	thred->csocket=cdSocket;
	return NULL;                                                                                    
}
TCPSocket SocketThread::getSocket(){
	return this->csocket;
}

//SocketThread* SocketThread::m_pInstance=new SocketThread; 
SocketThread* SocketThread::m_pInstance;
SocketThread* SocketThread::GetInstance(){	
	return m_pInstance;
}

void SocketThread::stop(){
#if WIN32
	pthread_cancel(pid);
	pthread_detach(pid); 
#endif
}

SocketThread::SocketThread(void)
{

}
SocketThread::~SocketThread(void)
{
	//if(m_pInstance!=NULL){
		delete m_pInstance;
	//}
}
