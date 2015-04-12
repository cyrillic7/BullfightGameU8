/*
 * QueueData.h
 *
 *  Created on: 2015年4月3日
 *      Author: xw007
 */

#ifndef QUEUEDATA_H_
#define QUEUEDATA_H_
#include "cocos2d.h"
#include "Packet.h"
USING_NS_CC;
#define MAX_TCP_LEN 1024*5
class QueueData:public CCObject {
public:
	TCP_Command command;
	WORD wSubCmdID;
	//void * pDataBuffer;
	WORD wDataSize;

    struct SendData
    {
        char sSendData[MAX_TCP_LEN];
        DWORD dwDataLen;
    }sendData;
public:
	QueueData();
	virtual ~QueueData();
};

#endif /* QUEUEDATA_H_ */
