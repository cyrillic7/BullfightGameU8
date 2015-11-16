
#import "PaymentViewController.h"
#import "EAGLView.h"

@interface PaymentViewController ()

@end

@implementation PaymentViewController

- (id)init
{
    self = [super init];
    if (self) {
        CGRect cRect=[[UIScreen mainScreen] applicationFrame];
        NSLog(@"%f  %f %f  %f",cRect.origin.x,cRect.origin.y,cRect.size.width,cRect.size.height);
         mView=[[UIView alloc] initWithFrame:CGRectMake(cRect.origin.x, cRect.origin.y, cRect.size.width/2 , cRect.size.height/2)];
        [mView setBackgroundColor:[UIColor blackColor]];
        [mView setAlpha:0.5];
        [mView setUserInteractionEnabled:YES];
        [[EAGLView sharedEGLView] addSubview:mView];
        [self showLoading];
        //
        
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    }
    return self;
}
-(void)showLoading{
    activityView = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhite];
    activityView.frame = CGRectMake(mView.frame.size.width/2, mView.frame.size.height/2, 37.0f, 37.0f);
    [mView addSubview:activityView];
    [activityView startAnimating];
}
- (void)purchaseFunc{
    if ([SKPaymentQueue canMakePayments]) {
        // 执行下面提到的第5步：
        NSString *propID=[NSString stringWithFormat:@"com.xw.BullfightGame_6"];
        [self getProductInfo:propID];
    } else {
        NSLog(@"失败，用户禁止应用内付费购买.");
    }
    /*NSString *product = self.productID.text;
    if([SKPaymentQueue canMakePayments]){
        [self requestProductData:product];
    }else{
        NSLog(@"不允许程序内付费");
    }*/
}
// 下面的ProductId应该是事先在itunesConnect中添加好的，已存在的付费项目。否则查询会失败。
- (void)getProductInfo:(NSString *)type {
    NSArray *product = [[NSArray alloc] initWithObjects:type,nil];
    NSSet *set = [NSSet setWithArray:product];
    
   // NSSet * set = [NSSet setWithArray:@[@"ProductId"]];
    SKProductsRequest * request = [[SKProductsRequest alloc] initWithProductIdentifiers:set];
    request.delegate = self;
    [request start];
}

// 以上查询的回调函数
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {
    NSArray *myProduct = response.products;
    if (myProduct.count == 0) {
        NSLog(@"无法获取产品信息，购买失败。");
        return;
    }
    SKPayment * payment = [SKPayment paymentWithProduct:myProduct[0]];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions {
    for (SKPaymentTransaction *transaction in transactions)
    {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased://交易完成
                NSLog(@"transactionIdentifier = %@", transaction.transactionIdentifier);
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed://交易失败
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored://已经购买过该商品
                [self restoreTransaction:transaction];
                break;
            case SKPaymentTransactionStatePurchasing:      //商品添加进列表
                NSLog(@"商品添加进列表");
                break;
            default:
                break;
        }
    }
    
}

- (void)completeTransaction:(SKPaymentTransaction *)transaction {
    // Your application should implement these two methods.
    NSString * productIdentifier = transaction.payment.productIdentifier;
    NSString * receipt = [transaction.transactionReceipt base64Encoding];
    if ([productIdentifier length] > 0) {
        // 向自己的服务器验证购买凭证
    }
    [self showTip:@"购买成功" :@"购买成功"];
    // Remove the transaction from the payment queue.
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
    
}

- (void)failedTransaction:(SKPaymentTransaction *)transaction {
    if(transaction.error.code != SKErrorPaymentCancelled) {
        [self showTip:@"购买失败" :@"购买失败，请稍后再次尝试！"];
        //NSLog(@"购买失败");
    } else {
        [self showTip:@"取消购买" :@"用户取消交易"];
        //NSLog(@"用户取消交易");
    }
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void)restoreTransaction:(SKPaymentTransaction *)transaction {
    // 对于已购商品，处理恢复购买的逻辑
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}
-(void)showTip:(NSString *)title :(NSString *)sContent{
    [activityView stopAnimating];
    [mView removeFromSuperview];
    [self release];
    
    UIAlertView *alter = [[UIAlertView alloc] initWithTitle:title message:sContent delegate:nil cancelButtonTitle:@"好的" otherButtonTitles:nil];
    [alter show];
    [alter release];
}

- (void)dealloc{
    [mView release];
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
    [super dealloc];
}

@end