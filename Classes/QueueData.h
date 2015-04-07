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
class QueueData:public CCObject {
public:
	TCP_Command Command;
	void * pDataBuffer;
public:
	QueueData();
	virtual ~QueueData();
};

#endif /* QUEUEDATA_H_ */
