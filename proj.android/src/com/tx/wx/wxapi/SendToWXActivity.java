package com.tx.wx.wxapi;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.tencent.mm.sdk.modelmsg.SendMessageToWX;
import com.tencent.mm.sdk.modelmsg.WXMediaMessage;
import com.tencent.mm.sdk.modelmsg.WXWebpageObject;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.WXAPIFactory;
import com.xw.BullfightGame.BullfightGame;
import com.xw.BullfightGame.R;
public class SendToWXActivity {

	public void sendShareTest(String text, int flag){
		/*// 初始化一个WXTextObject对象
		WXTextObject textObj = new WXTextObject();
		textObj.text = text;

		// 用WXTextObject对象初始化一个WXMediaMessage对象
		WXMediaMessage msg = new WXMediaMessage();
		msg.mediaObject = textObj;
		// 发送文本类型的消息时，title字段不起作用
		// msg.title = "Will be ignored";
		msg.description = text;

		// 构造一个Req
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("text"); // transaction字段用于唯一标识一个请求
		req.message = msg;
		//req.scene = isTimelineCb.isChecked() ? SendMessageToWX.Req.WXSceneTimeline : SendMessageToWX.Req.WXSceneSession;
		req.scene=SendMessageToWX.Req.WXSceneSession;
		// 调用api接口发送数据到微信
		IWXAPI api= WXAPIFactory.createWXAPI(BullfightGame.game, Constants.APP_ID);
		api.sendReq(req);
		//BullfightGame.game.finish();*/
		 WXWebpageObject webpage = new WXWebpageObject();  
		 webpage.webpageUrl = "http://m.qicainiu.com/Home/Download";  
		 WXMediaMessage msg = new WXMediaMessage(webpage);  
		 msg.title = (String) BullfightGame.game.getResources().getText(R.string.app_name);
		 msg.description = text;  
		 //这里替换一张自己工程里的图片资源  
		 Bitmap thumb = BitmapFactory.decodeResource(BullfightGame.game.getResources(), R.drawable.icon);  
		 msg.setThumbImage(thumb);        
		 SendMessageToWX.Req req = new SendMessageToWX.Req();  
		 req.transaction = String.valueOf(System.currentTimeMillis());  
		 req.message = msg;  
		
		 req.scene = flag==0?SendMessageToWX.Req.WXSceneSession:SendMessageToWX.Req.WXSceneTimeline;  
		 IWXAPI api= WXAPIFactory.createWXAPI(BullfightGame.game, Constants.APP_ID);
		api.sendReq(req);
	}
	/*private String buildTransaction(final String type) {
		return (type == null) ? String.valueOf(System.currentTimeMillis()) : type + System.currentTimeMillis();
	}*/
}
