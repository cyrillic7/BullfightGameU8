#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//////////////////////////////////////////////////////////////////////////
#include "../coPlatform.h"
#include "../../Libs/JsonAide.h"
USING_NS_CC;
/**
 * 执行动作
 @param jsonString 动作参数 {act:0, ...}
 *  act:0 (登陆), type:(登陆类型 0:匿名登陆 1:qq登陆)
 */
std::string platformAction(const std::string& jsonString)
{
	try
	{
		JsonAide aide(jsonString);

		//判断读取成功与否
		if (aide.hasError())
		{
			CCLOG("--jniCcAction-----------\n%s\n----------------- <<%s>>", aide.getError(), __FUNCTION__);
			return false;
		}

		int act = aide.getInt("act", -1);
		CCLOG("win32................................................... %d<<%s>>", act,__FUNCTION__);

		switch (act)
		{
		case 100:
			return "A100003BBEEB9Q2";
			break;
		case 600:
			return "1.1.0";
			break;
		case 800:
			return "MFB20150926180340277543138";
			break;
		default:
			break;
		}
		/*
		switch (act)
		{
		case 10:	// 本地通知
			{
			}
			break;
		case 20:	// 支付
			{
				G_NOTIFY("PAYMENT_PAY_FAILED", MTData::create(0, 0, 0, "无法充值 "));
			}
			break;
		case 100:	// 签名验证
			{
				G_NOTIFY("LOADING_COMPLETE", 0);
			}
			break;
		case 200:	// 登陆动作
			{
				int type		= aide.getInt("type", 0);
				std::string uid = aide.getString("uid", "");
				G_NOTIFY("AUTH_SUCCESS", MTData::create(type, 0, 0, type == 0 ? "" : uid));
			}
			break;
		case 500:	 // 退出游戏
			{
				G_NOTIFY("GAME_EXIT", 0);
			}
			break;
		}*/
	}
	catch (...)
	{

	}

	return "";
}
//////////////////////////////////////////////////////////////////////////
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)