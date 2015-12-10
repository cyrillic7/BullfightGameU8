package com.xw.BullfightGame.wxapi;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.util.Property;
import android.widget.Toast;

import com.tencent.mm.sdk.constants.ConstantsAPI;
import com.tencent.mm.sdk.modelbase.BaseReq;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.modelmsg.SendAuth;
import com.tencent.mm.sdk.modelmsg.ShowMessageFromWX;
import com.tencent.mm.sdk.modelmsg.WXAppExtendObject;
import com.tencent.mm.sdk.modelmsg.WXMediaMessage;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.sdk.openapi.WXAPIFactory;
import com.tx.wx.wxapi.Constants;
import com.xw.BullfightGame.BullfightGame;
import com.xw.BullfightGame.R;

public class WXEntryActivity extends Activity implements IWXAPIEventHandler{
	@Override  
	protected void onCreate(Bundle savedInstanceState) {  
	        // TODO Auto-generated method stub  
	        super.onCreate(savedInstanceState);  
	        //setContentView(R.layout.flash_activity);  
	        IWXAPI api= WXAPIFactory.createWXAPI(BullfightGame.game, Constants.APP_ID);
			api.handleIntent(getIntent(), this);
	    }  
	@Override
	public void onReq(BaseReq req) {
		// TODO Auto-generated method stub
		switch (req.getType()) {
		case ConstantsAPI.COMMAND_GETMESSAGE_FROM_WX:
			goToGetMsg();		
			break;
		case ConstantsAPI.COMMAND_SHOWMESSAGE_FROM_WX:
			goToShowMsg((ShowMessageFromWX.Req) req);
			break;
		default:
			break;
		}
	}

	@Override
	public void onResp(BaseResp resp) {
	        switch (resp.errCode) {  
	        case BaseResp.ErrCode.ERR_OK:  
	        {
	        	SendAuth.Resp re = (SendAuth.Resp) resp;
	        	String token = re.code;//应该是类似code
	        	BullfightGame.JniWXLogin(token);
	        }
	            break;  
	        case BaseResp.ErrCode.ERR_USER_CANCEL:
	        	Toast.makeText(this, "取消登录!", Toast.LENGTH_LONG).show();
				break;
	        case BaseResp.ErrCode.ERR_AUTH_DENIED:
	        case BaseResp.ErrCode.ERR_SENT_FAILED:
	        	Toast.makeText(this, "登录失败!", Toast.LENGTH_LONG).show();
				break;
	        default:  
	            break;  
	        } 
			finish();
		/*// TODO Auto-generated method stub
		int result = 0;
		
		switch (resp.errCode) {
		case BaseResp.ErrCode.ERR_OK:
			result = R.string.errcode_success;
			break;
		case BaseResp.ErrCode.ERR_USER_CANCEL:
			result = R.string.errcode_cancel;
			break;
		case BaseResp.ErrCode.ERR_AUTH_DENIED:
			result = R.string.errcode_deny;
			break;
		default:
			result = R.string.errcode_unknown;
			break;
		}
		
		Toast.makeText(this, result, Toast.LENGTH_LONG).show();
		finish();*/
	}
	
	private void goToGetMsg() {
		Intent intent = new Intent(this, BullfightGame.class);
		intent.putExtras(getIntent());
		startActivity(intent);
		finish();
	}
	
	private void goToShowMsg(ShowMessageFromWX.Req showReq) {
		Toast.makeText(this, "goToShowMsg========", Toast.LENGTH_LONG).show();
		WXMediaMessage wxMsg = showReq.message;		
		WXAppExtendObject obj = (WXAppExtendObject) wxMsg.mediaObject;
		
		StringBuffer msg = new StringBuffer(); // 组织一个待显示的消息内容
		msg.append("description: ");
		msg.append(wxMsg.description);
		msg.append("\n");
		msg.append("extInfo: ");
		msg.append(obj.extInfo);
		msg.append("\n");
		msg.append("filePath: ");
		msg.append(obj.filePath);
		
		Intent intent = new Intent(this, BullfightGame.class);
		intent.putExtra(Constants.ShowMsgActivity.STitle, wxMsg.title);
		intent.putExtra(Constants.ShowMsgActivity.SMessage, msg.toString());
		intent.putExtra(Constants.ShowMsgActivity.BAThumbData, wxMsg.thumbData);
		startActivity(intent);
		finish();
	}
}