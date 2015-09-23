package com.xw.BullfightGame;



import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.util.HashMap;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.AlertDialog.Builder;
import android.app.Service;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.DialogInterface.OnClickListener;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.os.Environment;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

public class UpdateManager extends Service
{
	/** 下载中 */
	private static final int DOWNLOAD = 1;
	/** 下载结束 */
	private static final int DOWNLOAD_FINISH = 2;
	/**下载失败*/
	private static final int DOWNLOADFAIL=3;
	
	
	/**下载保存路径 */
	private String mSavePath;
	/** 记录进度条数量 */
	private int progress;
	
	/** 保存解析的XML信息 */
	HashMap<String, String> mHashMap;
	/** 更新进度条 */
	private ProgressBar mProgress;
	/**进度条进度数字*/
	private TextView updateProgressBar;
	private Dialog mDownloadDialog;
//	private Dialog mDownloadFailDialog;//下载失败对话框
	/** 是否取消更新 */
	private boolean cancelUpdate = false;
//	private Context mContext;
	
	private BullfightGame bma;
	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return null;
	}
	@Override
	public void onCreate() {
		super.onCreate();
		bma=BullfightGame.game;
	}

	//@Override
	public void onStart(Intent intent, int startId) {
		super.onStart(intent, startId);
		showDownloadDialog();
		//checkUpdate();
		// System.out.println("onStartName:"+intent.getClass().getName());
//		startTimer();
	}

	
	public void onDestroy() {
		super.onDestroy();
//		stopTimer();
	}
//	public UpdateManager(Context context)
//	{
//		this.mContext = context;
//	}
	/**
	 * 检测软件更新
	 */
	/*public void checkUpdate()
	{
		if (isUpdate())
		{
			// 显示提示对话框
			showNoticeDialog();
		} else
		{
			Toast.makeText(bma,"已是最新版本", Toast.LENGTH_LONG).show();
		}
	}*/
	/**
	 * 检查软件是否有更新版本
	 * 
	 * @return
	 */
	/*private boolean isUpdate()
	{
		// 获取当前软件版本
		float versionCode = getVersionCode(bma);
		// 把version.xml放到网络上，然后获取文件信息
		InputStream inStream = ParseXmlService.class.getClassLoader().getResourceAsStream("com/thirdnet/zh/update/version.xml");
		// 解析XML文件。 由于XML文件比较小，因此使用DOM方式进行解析
		ParseXmlService service = new ParseXmlService();
		try
		{
			mHashMap = service.parseXml(inStream);
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		if (null != mHashMap)
		{
			float serviceCode = Float.valueOf(mHashMap.get("version"));
			System.out.println("serviceCode:"+serviceCode);
			System.out.println("versionCode:"+versionCode);
			// 版本判断
			if (serviceCode > versionCode)
			{
				return true;
			}
		}
		return false;
	}*/

	/**
	 * 获取本地软件版本号
	 * 
	 * @param context
	 * @return
	 */
	/*private float getVersionCode(Context context)
	{
		float versionCode = 0;
		try
		{
			// 获取软件版本号，对应AndroidManifest.xml下android:versionCode
			versionCode = Float.parseFloat(context.getPackageManager().getPackageInfo("com.thirdnet.zh", 0).versionName);
		} catch (NameNotFoundException e)
		{
			e.printStackTrace();
		}
		return versionCode;
	}*/

	/**
	 * 显示软件更新对话框
	 */
	/*private void showNoticeDialog()
	{
		// 构造对话框
		AlertDialog.Builder builder = new Builder(bma);
		builder.setTitle("软件更新");
		builder.setMessage("检测到新版本，立即更新吗");
		// 更新
		builder.setPositiveButton("更新", new OnClickListener()
		{
			@Override
			public void onClick(DialogInterface dialog, int which)
			{
				cancelUpdate=false;
				dialog.dismiss();
				// 显示下载对话框
				showDownloadDialog();
			}
		});
		// 稍后更新
		builder.setNegativeButton("稍后更新", new OnClickListener()
		{
			@Override
			public void onClick(DialogInterface dialog, int which)
			{
				restart();
				dialog.dismiss();
			}
		});
		Dialog noticeDialog = builder.create();
		noticeDialog.show();
	}*/
	/**
	 * 显示软件下载对话框
	 */
	private void showDownloadDialog()
	{
		mHashMap.put("url",bma.updateUrl );
		Log.v("update","name:"+bma.updateUrl.lastIndexOf("/"));
		String name=bma.updateUrl.substring( bma.updateUrl.lastIndexOf("/"));
		mHashMap.put("name1",name);
		Log.v("update", "name:"+name);
		Log.v("update",bma.updateUrl);
		// 构造软件下载对话框
		AlertDialog.Builder builder = new Builder(bma);
		builder.setTitle("正在更新...");
		// 给下载对话框增加进度条
		final LayoutInflater inflater = LayoutInflater.from(bma);
		View v = inflater.inflate(R.layout.softupdate_view, null);
		mProgress = (ProgressBar) v.findViewById(R.id.update_progress);
		updateProgressBar=(TextView) v.findViewById(R.id.updateProgressBar);
		builder.setView(v);
		// 取消更新
		builder.setNegativeButton("取消", new OnClickListener()
		{
			@Override
			public void onClick(DialogInterface dialog, int which)
			{
				restart();
				dialog.dismiss();
				// 设置取消状态
				cancelUpdate = true;
			}
		});
		mDownloadDialog = builder.create();
		mDownloadDialog.show();
		// 下载文件
		downloadApk();
	}
	/**
	 * 下载apk文件
	 */
	private void downloadApk()
	{
		// 启动新线程下载软件
		new downloadApkThread().start();
	}
	/**
	 * 下载文件线程
	 */
	private class downloadApkThread extends Thread
	{
		@Override
		public void run()
		{
			try
			{
				// 判断SD卡是否存在，并且是否具有读写权限
				if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED))
				{
					// 获得存储卡的路径
					String sdpath = Environment.getExternalStorageDirectory() + "/";
					mSavePath = sdpath + "download";
					URL url = new URL(mHashMap.get("url"));
					// 创建连接
					URLConnection conn = (URLConnection) url.openConnection();
					conn.connect();
					// 获取文件大小
					int length = conn.getContentLength();
					// 创建输入流
					InputStream is = conn.getInputStream();
					if(is==null){
						mDownloadDialog.dismiss();
						Toast.makeText(bma,"与服务器断开连接!", Toast.LENGTH_LONG).show();
						return;
					}

					File file = new File(mSavePath);
					// 判断文件目录是否存在
					if (!file.exists())
					{
						file.mkdir();
					}
					File apkFile = new File(mSavePath, mHashMap.get("name"));
					FileOutputStream fos = new FileOutputStream(apkFile);
					int count = 0;
					// 缓存
					byte buf[] = new byte[1024];
					// 写入到文件中
					do
					{
						int numread = is.read(buf);
						count += numread;
						// 计算进度条位置
						progress = (int) (((float) count / length) * 100);
						// 更新进度
						mHandler.sendEmptyMessage(DOWNLOAD);
						if (numread <= 0)
						{
							// 下载完成
							mHandler.sendEmptyMessage(DOWNLOAD_FINISH);
							break;
						}
						// 写入文件
						fos.write(buf, 0, numread);
					} while (!cancelUpdate);// 点击取消就停止下载.
					fos.close();
					is.close();
				}
			} catch (Exception e)
			{
				mHandler.sendEmptyMessage(DOWNLOADFAIL);
				System.out.println("下载异常:"+e.getMessage());
				e.printStackTrace();
			}
			// 取消下载对话框显示
			mDownloadDialog.dismiss();
			
		}

	}
	private void showFail(){
		new AlertDialog.Builder(bma)
		.setTitle("失败！")
		.setMessage("更新失败，请重新下载")
		.setPositiveButton("确定",
				new DialogInterface.OnClickListener() {

					@Override
					public void onClick(DialogInterface dialog,
							int which) {
//						restart();
						dialog.dismiss();
					}
				}).show();
	}
	private Handler mHandler = new Handler()
	{
		public void handleMessage(Message msg)
		{
			switch (msg.what)
			{
			// 正在下载
			case DOWNLOAD:
				// 设置进度条位置
				mProgress.setProgress(progress);
				updateProgressBar.setText(progress+"%");
				break;
			case DOWNLOAD_FINISH:
				restart();
				// 安装文件
				installApk();
				break;
			case DOWNLOADFAIL:
				restart();
				//下载失败
				showFail();
				break;
			default:
				break;
			}
		};
	};
	/**
	 * 安装APK文件
	 */
	private void installApk()
	{
		File apkfile = new File(mSavePath, mHashMap.get("name"));
		if (!apkfile.exists())
		{
			return;
		}
		// 通过Intent安装APK文件
		Intent i = new Intent(Intent.ACTION_VIEW);
		i.setDataAndType(Uri.parse("file://" + apkfile.toString()), "application/vnd.android.package-archive");
		bma.startActivity(i);
	}
	private void restart(){
		stopService(new Intent(bma,UpdateManager.class));
	}
}
