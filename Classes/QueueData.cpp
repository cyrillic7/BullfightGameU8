/*
 * QueueData.cpp
 *
 *  Created on: 2015年4月3日
 *      Author: xw007
 */

#include "QueueData.h"

QueueData::QueueData()
//:pDataBuffer(NULL)
:wSubCmdID(0)
,wDataSize(0)
{
	//pDataBuffer=new void*();
}

QueueData::~QueueData() {
	//CC_SAFE_DELETE(pDataBuffer);
}