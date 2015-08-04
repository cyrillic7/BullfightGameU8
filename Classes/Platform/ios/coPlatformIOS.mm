#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//////////////////////////////////////////////////////////////////////////
#include "../coPlatform.h"
#include "../../Libs/JsonAide.h"
//#include "../../LogonScene/LogonScene.h"
//#include "../../Tools/DataModel.h"
#include "FMLayerWebView.h"
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
		CCLOG("--jniCcAction-----------\n%s\n----------------- <<%s>>", aide.getError(), __FUNCTION__);
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
            
            CFRelease(uuidStr);
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
        case 200:
        case 201:
        {
            std::string url=aide.getString("url");
            //LogonScene *pLS;
            FMLayerWebView* web = FMLayerWebView::create();
            web->iCurAction=act;
            //CCSize winSize=CCDirector::sharedDirector()->getWinSize();
            //web->setContentSize(CCSize(winSize.width/2,winSize.height/2));
            //web->setPosition(CCPoint(100,50));
            web->setUrlWithOpen(url);
            
            CCDirector::sharedDirector()->getRunningScene()->addChild(web);
            
        }
            break;
        case 500:
        {
            std::string sPackageName=aide.getString("packageName");
            std::string sActivity=aide.getString("activity");
            
            //NSString *sTemp=[NSString stringWithFormat:@"%s",sActivity.c_str()];
            NSString *sTemp=[NSString stringWithFormat:@"DDPGame://"];
            BOOL isAction=[[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:sTemp]];
            if (isAction) {
                [[UIApplication sharedApplication] openURL:[NSURL URLWithString:sTemp]];
            }
            CCLOG("isAction:%d <<%s>>",isAction,__PRETTY_FUNCTION__);
        }
            break;
            
        default:
            break;
    }
	CCLOG("ios.............................  %d",act);

	return "";
}
//////////////////////////////////////////////////////////////////////////
#endif // #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
