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
    switch (act) {
        case 100:
        {
           // NSString *retrieveuuid = [SSKeychain passwordForService:@"com.mohe.userinfo"account:@"uuid"];
            
            //if ( retrieveuuid == nil || [retrieveuuid isEqualToString:@""]){
                CFUUIDRef uuid = CFUUIDCreate(NULL);
                assert(uuid != NULL);
                CFStringRef uuidStr = CFUUIDCreateString(NULL, uuid);
                
                NSString *retrieveuuid = [NSString stringWithFormat:@"%@", uuidStr];
                
                //[SSKeychain setPassword:retrieveuuid forService:@"com.mohe.userinfo"account:@"uuid"];D9661007-6BC0-4F49-AA79-1C996E601AFE
            //27C54597-2384-4995-8C0C-D1688F81A965
           // }
            // const charchar *cStr = [[[NSProcessInfo processInfo] globallyUniqueString] UTF8String];
            NSLog(@"---:%@",retrieveuuid);
           // const char *cStr =[[[NSProcessInfo processInfo] globallyUniqueString] UTF8String];
            //return [str str];
            return [retrieveuuid UTF8String];
        }
            break;
            
        default:
            break;
    }
	CCLog("ios.............................  %d",act);

	return "";
}
//////////////////////////////////////////////////////////////////////////
#endif // #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
