#ifndef _JniHelper_H_
#define _JniHelper_H_
//////////////////////////////////////////////////////////////////////////

#include "cocos2d.h"
USING_NS_CC;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include <Jni.h>
#include "platform/android/jni/JniHelper.h"

bool getJniMethod(jobject& jobj, JniMethodInfo &minfo, const char* sFunction, const char* sSignature);


#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

//////////////////////////////////////////////////////////////////////////
#endif // _JniHelper_H_