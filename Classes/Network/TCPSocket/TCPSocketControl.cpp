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
,listerner(NULL)
,tcpSocket(NULL)
{
	// TODO Auto-generated constructor stub

}


TCPSocketControl::~TCPSocketControl() {
	// TODO Auto-generated destructor stub
	isRun=false;
	if (tcpSocket)
	{
		delete tcpSocket;
		tcpSocket=NULL;
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
	if (tcpSocket)
	{
		delete tcpSocket;
		tcpSocket=NULL;
	}
	tcpSocket=new TCPSocket();
	tcpSocket->Init();
	tcpSocket->Create(AF_INET, SOCK_STREAM, 0);
	tcpSocket->SetListerner(listerner);
	isRun=tcpSocket->Connect(ip, port);
	//tcp=NULL;
	//delete tcp;
	CCLog("Connect:%d", isRun);
}
/*void TCPSocketControl::stopSocket(){
	tcpSocket->Clean();
	tcpSocket->Close();
}*/
bool TCPSocketControl::SendData(WORD wMainCmdID, WORD wSubCmdID, void * const pData, WORD wDataSize){
	return tcpSocket->SendData(wMainCmdID,wSubCmdID,pData,wDataSize);
}
bool TCPSocketControl::SendData(WORD wMainCmdID, WORD wSubCmdID){
	return tcpSocket->SendData(wMainCmdID,wSubCmdID);
}
TCPSocket *TCPSocketControl::getTCPSocket(std::string key){
	std::map<std::string,TCPSocket*>::iterator iter;
	iter=mTcpSocket.find(key);
	if (iter==mTcpSocket.end())
	{
		TCPSocket *tcp=new TCPSocket();
		mTcpSocket.insert(std::map<std::string,TCPSocket*>::value_type(key,tcp));
		return tcp;
	}
	return iter->second;
}
void TCPSocketControl::stopSocket(std::string key){
	if (getTCPSocket(key)->eSocketState != TCPSocket::SOCKET_STATE_FREE)
	{
		getTCPSocket(key)->Clean();
		getTCPSocket(key)->Close();
	}
}
void TCPSocketControl::removeTCPSocket(std::string key){
	delete getTCPSocket(key);
	mTcpSocket.erase(key);
}