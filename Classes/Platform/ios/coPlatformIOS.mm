#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//////////////////////////////////////////////////////////////////////////
#include "../coPlatform.h"
#include "../../Libs/JsonAide.h"
//#include "../../LogonScene/LogonScene.h"
//#include "../../Tools/DataModel.h"
#include "FMLayerWebView.h"
#include "weixin/WXApiResponseHandler.h"
#include "weixin/WXApiRequestHandler.h"
#include "PaymentViewController.h"
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
        case 300:
        {
            std::string propName = aide.getString("propName");
            std::string propInfo = aide.getString("propInfo");
            std::string propPice = aide.getString("propPice");
            //pay(propName,propInfo,propPice);
            PaymentViewController *pay=[[PaymentViewController alloc] init];
            [pay purchaseFunc];
            
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
        case 600:
        {
            NSString *version=(NSString*)[[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
            return [version UTF8String];
        }
            break;
        case 900://分享
        {
            NSString *kLinkURL=@"http://m.qicainiu.com/Home/Download";
            UIImage *thumbImage = [UIImage imageNamed:@"Icon-114.png"];
            NSLog(@"URL:%s  %f",[kLinkURL UTF8String],thumbImage.size.width);
           /* [WXApiResponseHandler respLinkURL:kLinkURL
                                        Title:@"达人牛牛"
                                  Description:@"在不牛牛"
                                   ThumbImage:thumbImage];*/
            [WXApiRequestHandler sendLinkURL:kLinkURL
                                     TagName:@"share"
                                       Title:@"达人牛牛"
                                 Description:@"再不牛牛我们就老了！您的微信好友邀请您玩“ 达人牛牛”，立即前往下载游戏支援好友！~ "
                                  ThumbImage:thumbImage
                                     InScene:WXSceneSession];

            //[self dismissModalViewControllerAnimated:YES];
        }
            break;
        case 901://分享
        {
            NSString *kLinkURL=@"http://m.qicainiu.com/Home/Download";
            UIImage *thumbImage = [UIImage imageNamed:@"Icon-114.png"];
            NSLog(@"URL:%s  %f",[kLinkURL UTF8String],thumbImage.size.width);
            /* [WXApiResponseHandler respLinkURL:kLinkURL
             Title:@"达人牛牛"
             Description:@"在不牛牛"
             ThumbImage:thumbImage];*/
            [WXApiRequestHandler sendLinkURL:kLinkURL
                                     TagName:@"share"
                                       Title:@"达人牛牛"
                                 Description:@"再不牛牛我们就老了！您的微信好友邀请您玩“ 达人牛牛”，立即前往下载游戏支援好友！~ "
                                  ThumbImage:thumbImage
                                     InScene:WXSceneTimeline];
            
            //[self dismissModalViewControllerAnimated:YES];
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
