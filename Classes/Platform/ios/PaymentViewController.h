
#import <UIKit/UIKit.h>
#import <StoreKit/StoreKit.h>
@interface PaymentViewController : UIViewController<SKPaymentTransactionObserver,SKProductsRequestDelegate>

@property (strong, nonatomic) IBOutlet UITextField *productID;

@property (strong, nonatomic) IBOutlet UIButton *purchase;

- (IBAction)purchaseFunc;

@end