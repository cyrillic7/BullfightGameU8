#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//////////////////////////////////////////////////////////////////////////
#include "../coPlatform.h"
#include "../../Libs/JsonAide.h"
USING_NS_CC;

//---------------------------------------------------------------------------
/**
 * 执行动作
 @param jsonString 动作参数 {act:0, ...}
 *  act:0 (登陆), type:(登陆类型 0:匿名登陆 1:qq登陆)
 */
std::string platformAction(const std::string& jsonString)
{
	JsonAide aide(jsonString);

	//判断读取成功与否
	if (aide.hasError())
	{
		CCLog("--jniCcAction-----------\n%s\n----------------- <<%s>>", aide.getError(), __FUNCTION__);
		return false;
	}

	int act = aide.getInt("act", -1);

	CCLog("ios.............................  %d",act);

	return "";
}
//////////////////////////////////////////////////////////////////////////
#endif // #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
