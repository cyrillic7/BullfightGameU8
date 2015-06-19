#include "JniHelper.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//////////////////////////////////////////////////////////////////////////
using namespace std;

bool getJniMethod(jobject& jobj, JniMethodInfo &minfo, const char* sFunction, const char* sSignature)
{
#define JNI_PACKAGE "com/xw/BullfightGame/BullfightGame"
	//getStaticMethodInfo，判断Java静态函数是否存在，并且把信息保存到minfo里
	//参数1：JniMethodInfo
	//参数2：Java类包名+类名
	//参数3：Java函数名称
	//参数4：函数参数类型和返回值类型
	if (!JniHelper::getStaticMethodInfo(minfo,JNI_PACKAGE,"shared","()L" JNI_PACKAGE ";")) 
		return false;

	//这里的调用getInstance，返回类的对象。
	jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	return JniHelper::getMethodInfo(minfo,JNI_PACKAGE,sFunction,sSignature);
}

//////////////////////////////////////////////////////////////////////////
#endif // #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
