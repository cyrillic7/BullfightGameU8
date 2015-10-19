package com.tx.qq.qqapi;

import org.json.JSONObject;

import android.util.Log;

import com.tencent.tauth.IUiListener;
import com.tencent.tauth.UiError;

public class BaseUiListener implements IUiListener {

	@Override
	public void onCancel() {
		// TODO Auto-generated method stub
		//showResult("onCancel", "");
		Log.v("", "");
	}

	@Override
	public void onComplete(Object  response) {
		Log.v("", "");
		// TODO Auto-generated method stub
		//mBaseMessageText.setText("onComplete:");
		 //mMessageText.setText(response.toString());
		 //doComplete(response);
	}

	@Override
	public void onError(UiError e) {
		Log.v("", "");
		// TODO Auto-generated method stub
		//showResult("onError:", "code:" + e.errorCode + ", msg:"
				// + e.errorMessage + ", detail:" + e.errorDetail);
	}
}
