
#import "PaymentViewController.h"
#import "EAGLView.h"

@interface PaymentViewController ()

@end

@implementation PaymentViewController
 
- (id)init
{
    self = [super init];
    if (self) {
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
        
        CGRect cRect=[[UIScreen mainScreen] applicationFrame];
        NSLog(@"%f  %f %f  %f",cRect.origin.x,cRect.origin.y,cRect.size.width,cRect.size.height);
         mView=[[UIView alloc] initWithFrame:CGRectMake(cRect.origin.x, cRect.origin.y, cRect.size.width, cRect.size.height)];
        [mView setBackgroundColor:[UIColor blackColor]];
        [mView setAlpha:0.5];
       
        [[EAGLView sharedEGLView] addSubview:mView];
        [self showLoading];
        //
         [mView setUserInteractionEnabled:NO];
        
        
    }
    return self;
}
-(void)showLoading{
    activityView = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhite];
    int sizeW=36;
    activityView.frame = CGRectMake(mView.frame.size.width/2-sizeW/2, mView.frame.size.height/2-sizeW/2, sizeW, sizeW);
    [mView addSubview:activityView];
    [activityView startAnimating];
}
- (void)purchaseFunc{
    if ([SKPaymentQueue canMakePayments]) {
        // 执行下面提到的第5步：
        NSString *propID=[NSString stringWithFormat:@"com.xw.BullfightGame_6"];
        [self getProductInfo:propID];
    } else {
        [self removePayView];
        //[self showTip:@"购买" :@"不允许App内购买项目"];
        //NSLog(@"失败，用户禁止应用内付费购买.");
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
       // NSLog(@"无法获取产品信息，购买失败。");
        [self showTip:@"购买失败" :@"无法获取产品信息，购买失败。"];
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
    [self removePayView];
    //[self showTip:@"购买成功" :@"购买成功"];
    // Remove the transaction from the payment queue.
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
    
}

- (void)failedTransaction:(SKPaymentTransaction *)transaction {
    if(transaction.error.code != SKErrorPaymentCancelled) {
        [self showTip:@"购买失败" :@"购买失败，请稍后再次尝试！"];
        //NSLog(@"购买失败");
    } else {
        [self removePayView];
        //[self showTip:@"取消购买" :@"用户取消交易"];
        NSLog(@"用户取消交易");
    }
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void)restoreTransaction:(SKPaymentTransaction *)transaction {
    // 对于已购商品，处理恢复购买的逻辑
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}
-(void)removePayView{
    [activityView stopAnimating];
    [mView removeFromSuperview];
    [self release];
}
-(void)showTip:(NSString *)title :(NSString *)sContent{
    [self removePayView];
    
    UIAlertView *alter = [[UIAlertView alloc] initWithTitle:title message:sContent delegate:nil cancelButtonTitle:@"好的" otherButtonTitles:nil];
    [alter show];
    [alter release];
}

- (void)dealloc{
    [mView release];
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
    [super dealloc];
}

+(NSString *)GetDateTimeLongString
{
    //实例化一个NSDateFormatter对象
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyyMMddHHmmssSSS"];
    //用[NSDate date]可以获取系统当前时间
    NSString *currentDateStr = [dateFormatter stringFromDate:[NSDate date]];
    //输出格式为：2010-10-27 10:22:13
    NSLog(@"%@",currentDateStr);
    //alloc后对不使用的对象别忘了release
    [dateFormatter release];
    /*SimpleDateFormat format = new SimpleDateFormat("yyyyMMddHHmmssSSS",
     Locale.getDefault());
     Date date = new Date();
     String key = format.format(date);
     System.out.println("Time:"+key);
     return key;*/
    return currentDateStr;
}
+(NSString*)CreateRandom:(int)length:(int)useNum:(int)useLow:(int)useUpp:(int)useSpe:(NSString*)custom
{
    //byte[] data = new byte[4];
    /*
     Random random=new Random();*/
    //NSString *str = @"";
    NSMutableString *str = [[NSMutableString alloc] init];
    NSMutableString *str2 = [[NSMutableString alloc] init];
    [str2 appendFormat:@"%@",custom];
    if (useNum == 1)
    {
        [str2 appendString:@"0123456789"];
    }
    if (useLow == 1)
    {
        [str2 appendString:@"abcdefghijklmnopqrstuvwxyz"];
    }
    if (useUpp == 1)
    {
        [str2 appendString:@"ABCDEFGHIJKLMNOPQRSTUVWXYZ"];
    }
    if (useSpe == 1)
    {
        [str2 appendString:@"!\"#$%&'()*+,-.<=>?@[\\]^_`{|}~"];
    }
    for (int i = 0; i < length; i++)
    {
        int start=arc4random()%([str2 length] - 1);
        NSString *strTemp=[str2 substringWithRange:NSMakeRange(start,1)];
        [str appendFormat:@"%@",strTemp];
        //int start=random.nextInt([str2 length] - 1);
        //String strTemp= str2.substring(start, start+1);
        //str = str +strTemp;
    }
    [str2 autorelease];
    //System.out.println("random:"+str);
    return [str autorelease];
}
+(NSString*)GetOrderIDByPrefix:(NSString *)prefix{
    //构造订单号 (形如:20101201102322159111111)
    int orderIDLength = 32;
    int randomLength = 6;
    NSMutableString *tradeNoBuffer = [[NSMutableString alloc] init];
    [tradeNoBuffer appendString:prefix];
    [tradeNoBuffer appendFormat:@"%@",[self GetDateTimeLongString]];
    
    if (([tradeNoBuffer length] + randomLength) > orderIDLength)
        randomLength = orderIDLength - [tradeNoBuffer length];
    [tradeNoBuffer appendFormat:@"%@",[self CreateRandom:randomLength :1 :0 :0 :0 :@""]];
    NSLog(@"buffer:%@",tradeNoBuffer);
    return [tradeNoBuffer autorelease];
}
@end