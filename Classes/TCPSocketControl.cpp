/*
 * TCPSocketControl.cpp
 *
 *  Created on: 2015年3月30日
 *      Author: xw007
 */

#include "TCPSocketControl.h"


TCPSocketControl* _sharedTCPSocketControl;
pthread_t threadNewwork;


TCPSocketControl::TCPSocketControl()
:isRun(true)
,tcp(NULL)
{
	// TODO Auto-generated constructor stub
}

TCPSocketControl::~TCPSocketControl() {
	// TODO Auto-generated destructor stub
	isRun=false;
	if (tcp)
	{
		delete tcp;
		tcp=NULL;
	}
}
TCPSocketControl* TCPSocketControl::sharedTCPSocketControl()
{
	if (_sharedTCPSocketControl == 0) {
		_sharedTCPSocketControl = new TCPSocketControl();
		_sharedTCPSocketControl->initTCPSocket();
		return _sharedTCPSocketControl;
	}
	return _sharedTCPSocketControl;
}
void TCPSocketControl::initTCPSocket(){
	
}
int TCPSocketControl::startSendThread(){
	int errCode = 0;
	do{
		pthread_attr_t tAttr;
		errCode = pthread_attr_init(&tAttr);

		CC_BREAK_IF(errCode != 0);

		errCode = pthread_attr_setdetachstate(&tAttr, PTHREAD_CREATE_DETACHED);

		if (errCode != 0) {
			pthread_attr_destroy(&tAttr);
			break;
		}

		errCode = pthread_create(&threadNewwork, &tAttr, networkThread, this);
		pthread_detach(threadNewwork);
	} while (0);
	return errCode;
}
void* TCPSocketControl::networkThread(void* object){
	TCPSocketControl *app=(TCPSocketControl*)object;
	app->initNetwork();
	return 0;
}
void TCPSocketControl::initNetwork(){
	/*DefaultListerner *df=new DefaultListerner();
	*///TCPSocket tcp;
	if (tcp)
	{
		delete tcp;
		tcp=NULL;
	}else
	{
		
	}
	tcp=new TCPSocket();
	tcp->Init();
	tcp->Create(AF_INET, SOCK_STREAM, 0);
	tcp->SetListerner(listerner);
	isRun=tcp->Connect(ip, port);
	//tcp=NULL;
	//delete tcp;
	CCLog("Connect:%d", isRun);
	/*while(isRun){

	}
	tcp.Close();
	CCLog("close----------------------------");
	isRun=true;*/
}
void TCPSocketControl::deleteControl(){
	
}
void TCPSocketControl::stopSocket(){
	//tcp->getListerner()->End();
	tcp->Clean();
	tcp->Close();

	//delete _sharedTCPSocketControl;
	//_sharedTCPSocketControl=NULL;

	//thread_
	//delete tcp;
	//tcp=NULL;
	//tcp->deleteListerner();
	/*delete tcp->getListerner();
	SocketListerner *tcps=tcp->getListerner();
	tcps=NULL;*/
}
bool TCPSocketControl::SendData(WORD wMainCmdID, WORD wSubCmdID, void * const pData, WORD wDataSize){
	return tcp->SendData(wMainCmdID,wSubCmdID,pData,wDataSize);
}
bool TCPSocketControl::SendData(WORD wMainCmdID, WORD wSubCmdID){
	return tcp->SendData(wMainCmdID,wSubCmdID);
}