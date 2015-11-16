
#import <UIKit/UIKit.h>
#import <StoreKit/StoreKit.h>
@interface PaymentViewController : NSObject<SKPaymentTransactionObserver,SKProductsRequestDelegate>
{
    UIView *mView;
    UIActivityIndicatorView *activityView;
}
- (void)purchaseFunc;
@end