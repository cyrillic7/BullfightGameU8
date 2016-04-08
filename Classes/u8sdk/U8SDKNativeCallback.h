#ifndef __U8SDK_NATIVE_CALLBACK_H__
#define __U8SDK_NATIVE_CALLBACK_H__

#include "cocos2d.h"
#include "U8SDKCallback.h"

class U8SDKNativeCallback :public CCNode
{

private:
	U8SDKCallback* gameCallback;

public:
	
	U8SDKNativeCallback(U8SDKCallback* callback);
	virtual ~U8SDKNativeCallback();

	virtual void OnU8InitSuc(cocos2d::CCNode *sender, void * data);

	virtual void OnU8LoginSuc(cocos2d::CCNode *sender, void * data);

	virtual void OnU8SwitchLogin(cocos2d::CCNode *sender, void * data);

	virtual void OnU8Logout(cocos2d::CCNode *sender, void * data);

	virtual void OnU8PaySuc(cocos2d::CCNode *sender, void * data);

};

#endif