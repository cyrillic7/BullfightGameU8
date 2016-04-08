#include "AppDelegate.h"
#include "Tools/GameConfig.h"
#include "Logo/LogoScene.h"
#include "Tools/BaseAttributes.h"
#include "Tools/DataModel.h"
#include "MTNotificationQueue/MTNotificationQueue.h"
#include "MainScene/MainSceneOxHundred.h"
#include "GameLobby/GameLobbyScene.h"
#include "MTNotificationQueue/LobbyMsgHandler.h"
#include "MTNotificationQueue/GameIngMsgHandler.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//#include "vld.h"
#endif
AppDelegate::AppDelegate() {

}
AppDelegate::~AppDelegate() 
{
	//TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_LOBBY);
	//TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_LOBBY);

	DataModel *m = DataModel::sharedDataModel();
	CC_SAFE_RELEASE_NULL(m);
	BaseAttributes *base=BaseAttributes::sharedAttributes();
	CC_SAFE_DELETE(base);
	
	MTNotificationQueue *mtQueue=MTNotificationQueue::sharedNotificationQueue();
	CC_SAFE_DELETE(mtQueue);


	LobbyMsgHandler *pLobbyMsgHandler=LobbyMsgHandler::sharedLobbyMsgHandler();
	CC_SAFE_DELETE(pLobbyMsgHandler);

	GameIngMsgHandler *pGameIngMsgHandler = GameIngMsgHandler::sharedGameIngMsgHandler();
	CC_SAFE_DELETE(pGameIngMsgHandler);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	CCEGLView::sharedOpenGLView()->setFrameSize(SCENE_SIZE.width, SCENE_SIZE.height);
	//CCEGLView::sharedOpenGLView()->setFrameSize(800, 480);
	//CCEGLView::sharedOpenGLView()->setFrameSize(960, 640);
	//CCEGLView::sharedOpenGLView()->setFrameSize(1136, 640);
#endif
	//CCEGLView::sharedOpenGLView()->setDesignResolutionSize(SCENE_SIZE.width, SCENE_SIZE.height, kResolutionExactFit);//��Ļ����
	//CCEGLView::sharedOpenGLView()->setDesignResolutionSize(SCENE_SIZE.width, SCENE_SIZE.height, kResolutionFixedHeight);//��Ļ����
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(SCENE_SIZE.width, SCENE_SIZE.height, kResolutionFixedWidth);
	pDirector->setProjection(kCCDirectorProjection2D);//2D投影
    // turn on display FPS
    pDirector->setDisplayStats(false);
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
	//设置缩放因子
	//CCDirector::sharedDirector()->setContentScaleFactor(SCENE_SIZE.height / pDirector->getWinSize().height);
    // create a scene. it's an autorelease object
	CCScene *pScene = LogoScene::scene();
	//CCScene *pScene=MainSceneOxHundred::scene();
    //run
    pDirector->runWithScene(pScene);
	//////////////////////////////////////////////////////////////////////////
	LobbyMsgHandler::sharedLobbyMsgHandler();
	GameIngMsgHandler::sharedGameIngMsgHandler();

	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(  
		schedule_selector(MTNotificationQueue::postNotifications),  
		MTNotificationQueue::sharedNotificationQueue(),  
		1.0 / 60.0,  
		false);
	
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

	if (DataModel::isMusic) {
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		SimpleAudioEngine::sharedEngine()->resumeAllEffects();
	}
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
/////////////////////////////////////////////////////////////////////////////
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>

//call c
#ifdef __cplusplus
extern "C"
{
#endif


	//java 调用()
	JNIEXPORT void JNICALL Java_com_xw_BullfightGame_BullfightGame_JniOnActivity(JNIEnv* env, jobject job, jint type)
	{
		switch (type)
		{
		case 0://关闭web视图
		{
			BaseLobbyScene::lobbyScene->closeWebView();
		}
			break;
		case 1://网络连接
		{
			CGameSocket::isConnect=true;
		}
			break;//网络断开
		case 2:
		{
			CGameSocket::isConnect=false;
		}
			break;
		default:
		{

		}
		break;
		}
	}
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////
#endif // #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
