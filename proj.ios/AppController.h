#import <UIKit/UIKit.h>
#import "Reachability.h"

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    RootViewController    *viewController;
}
@property(nonatomic,strong)Reachability *coon;
@end

