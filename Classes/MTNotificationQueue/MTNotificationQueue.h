/*
 * MTNotificationQueue.h
 *
 *  Created on: 2015年3月31日
 *      Author: xw007
 */

#ifndef MTNOTIFICATIONQUEUE_H_
#define MTNOTIFICATIONQUEUE_H_
#include "cocos2d.h"
#include "../Network/SEvent.h"
USING_NS_CC;
using namespace std;
class MTNotificationQueue:public CCObject {
	static MTNotificationQueue * mInstance;
	typedef struct
	{
		string name;
		CCObject* object;
	} NotificationArgs;
	vector<NotificationArgs> notifications;
	MTNotificationQueue(void);
public:
	static MTNotificationQueue* sharedNotificationQueue();
	void postNotifications(float dt);
	~MTNotificationQueue(void);
	void postNotification(const char* name, CCObject* object);
    
};

#endif /* MTNOTIFICATIONQUEUE_H_ */
