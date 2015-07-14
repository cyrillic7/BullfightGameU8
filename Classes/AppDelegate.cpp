#include "AppDelegate.h"
#include "Tools/GameConfig.h"
#include "LogonScene/LogonScene.h"
#include "Tools/BaseAttributes.h"
#include "Tools/DataModel.h"
#include "Network/TCPSocket/TCPSocketControl.h"
#include "MTNotificationQueue/MTNotificationQueue.h"
#include "MainScene/MainSceneOxHundred.h"
#include "GameLobby/GameLobbyScene.h"
#include "MTNotificationQueue/LobbyMsgHandler.h"
USING_NS_CC;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "vld.h"
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
	TCPSocketControl *tcp=TCPSocketControl::sharedTCPSocketControl();
	//tcp->removeTCPSocket(SOCKET_LOGON_GAME);
	//tcp->stopSocket(SOCKET_LOBBY);
	//tcp->removeTCPSocket(SOCKET_LOBBY);
	CC_SAFE_DELETE(tcp);
	MTNotificationQueue *mtQueue=MTNotificationQueue::sharedNotificationQueue();
	CC_SAFE_DELETE(mtQueue);


	LobbyMsgHandler *pLobbyMsgHandler=LobbyMsgHandler::sharedLobbyMsgHandler();
	CC_SAFE_DELETE(pLobbyMsgHandler);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//CCEGLView::sharedOpenGLView()->setFrameSize(SCENE_SIZE.width, SCENE_SIZE.height);
	//CCEGLView::sharedOpenGLView()->setFrameSize(1024, 768);
	CCEGLView::sharedOpenGLView()->setFrameSize(960, 640);
	//CCEGLView::sharedOpenGLView()->setFrameSize(568, 320);
#endif
	//CCEGLView::sharedOpenGLView()->setDesignResolutionSize(SCENE_SIZE.width, SCENE_SIZE.height, kResolutionExactFit);//��Ļ����
	//CCEGLView::sharedOpenGLView()->setDesignResolutionSize(SCENE_SIZE.width, SCENE_SIZE.height, kResolutionFixedHeight);//��Ļ����
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(SCENE_SIZE.width, SCENE_SIZE.height, kResolutionFixedWidth);
	pDirector->setProjection(kCCDirectorProjection2D);//2D投影
    // turn on display FPS
    pDirector->setDisplayStats(true);
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
	//设置缩放因子
	//CCDirector::sharedDirector()->setContentScaleFactor(SCENE_SIZE.height / pDirector->getWinSize().height);
    // create a scene. it's an autorelease object
	CCScene *pScene = LogonScene::scene();
	//CCScene *pScene=MainSceneOxHundred::scene();
    //run
    pDirector->runWithScene(pScene);
	//////////////////////////////////////////////////////////////////////////
	LobbyMsgHandler::sharedLobbyMsgHandler();
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
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
