//
//  ClassicLobbyScene.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#include "ClassicLobbyScene.h"
#include "../Tools/Tools.h"
#include "../Tools/GameConfig.h"
#include "../PopDialogBox/PopDialogBoxUserInfo.h"
#include "GameLobbyScene.h"
#include "../MainScene/MainSceneOxTwo.h"
#include "../MainScene/MainSceneOxOneByOne.h"
#include "../MainScene/MainSceneOxSixSwap.h"
#include "../Network/CMD_Server/CMD_GameServer.h"
//#include "../Network/ListernerThread/GameIngListerner.h"
#include "../Network/CMD_Server/Packet.h"
#include "../Tools/DataModel.h"
#include "../PopDialogBox/PopDialogBoxLoading.h"
#include "../PopDialogBox/PopDialogBoxTipInfo.h"
#include "../Network/MD5/MD5.h"
#include "../Network/SEvent.h"
//#include "../Network/CMD_Server/cmd_ox.h"
ClassicLobbyScene::ClassicLobbyScene()
	:isDeleteList(false)
	, isEnterGame(false)
	, gameItem(ITEM_1)
{
	DataModel::sharedDataModel()->isSit = false;
	scheduleUpdate();
}
ClassicLobbyScene::~ClassicLobbyScene(){
	CCLOG("~ <<%s>>", __FUNCTION__);
	//

}
CCScene* ClassicLobbyScene::scene(bool isHundred)
{
	CCScene *scene = CCScene::create();
	ClassicLobbyScene *layer = ClassicLobbyScene::create();
	if (isHundred)
	{
		layer->setGameItem(ITEM_3);
	}
	else
	{
		layer->setGameItem(ITEM_1);
	}
	layer->backGameItem = layer->getGameItem();
	scene->addChild(layer);
	
	return scene;
}
void ClassicLobbyScene::onEnter(){
	BaseLobbyScene::onEnter();
	/*//添加背景
	CCSize deviceSize=DataModel::sharedDataModel()->deviceSize;
	CCSprite *spriteBg=CCSprite::create("res/main_bg.jpg");
	this->addChild(spriteBg);
	spriteBg->setPosition(ccp(deviceSize.width/2,deviceSize.height/2));
	float scale=deviceSize.height/spriteBg->getContentSize().height;
	spriteBg->setScale(scale);*/

	m_pWidget = UILayer::create();
	this->addChild(m_pWidget);


	UILayout *pWidget = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIClassicLobby.ExportJson)));
	m_pWidget->addWidget(pWidget);


	pBUserInfo->loadTextureNormal("back.png", UI_TEX_TYPE_PLIST);
	pBUserInfo->loadTexturePressed("back_press.png", UI_TEX_TYPE_PLIST);
	pBUserInfo->setScale9Enabled(false);
	pBUserInfo->ignoreContentAdaptWithSize(true);
	pBUserInfo->setSize(CCSize(51, 40));
	pBUserInfo->getChildByName("Image_23")->setVisible(false);


	UIButton* pBRecharge = static_cast<UIButton*>(m_pWidget->getWidgetByName("ButtonRecharge"));
	pBRecharge->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuFirstRecharge));

	//userName->setText(Tools::GBKToUTF8(DataModel::sharedDataModel()->userInfo->szNickName));
	std::string nickName = Tools::GBKToUTF8(DataModel::sharedDataModel()->userInfo->szNickName);
	userName->setText(Tools::subUTF8(nickName, 0, 4));
	/*UIButton* button = static_cast<UIButton*>(m_pWidget->getWidgetByName("buttonUser"));
	button->addTouchEventListener(this, SEL_TouchEvent(&ClassicLobbyScene::menuResetUser));*/

	/*for (int i = 0; i < MAX_LEVEL_COUNT; i++)
	{
		pBLevel[i] = static_cast<UIButton*>(m_pWidget->getWidgetByName(CCString::createWithFormat("ButtonScene%d", i + 1)->getCString()));
		pBLevel[i]->addTouchEventListener(this, SEL_TouchEvent(&ClassicLobbyScene::menuStar));
	}*/
	pLVViewRoom = static_cast<UIListView*>(m_pWidget->getWidgetByName("ListViewRoom"));
	//设置cell样式
	pLVViewRoom->setItemModel(pLVViewRoom->getItem(0));
	pLVViewRoom->removeAllItems();


	//更新房间列表
	updateRoomList();
	//游戏选择卡列表视图
	pLVItems = static_cast<UIListView*>(m_pWidget->getWidgetByName("ListViewItems"));
	//初始化游戏选项触摸事件
	initItemTouchEvent();
	if (getGameItem()!=ITEM_3)
	{
		selectGameItem(0);
	}
	else
	{
		pLVItems->setEnabled(false);
		pLVItems->getParent()->setVisible(false);
	}
	
	//添加监听事件
	//CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(ClassicLobbyScene::onPlay),S_L_PLAY,NULL);
	//CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(ClassicLobbyScene::onConfigFinish),S_L_CONFIG_FINISH,NULL);
	//CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(ClassicLobbyScene::onOpen),S_L_OPEN,NULL);
}
void ClassicLobbyScene::onExit(){
	//移除监听事件 
	//CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, S_L_PLAY); 
	//CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, S_L_CONFIG_FINISH); 
	//CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, S_L_OPEN); 


	this->removeAllChildrenWithCleanup(true);
	GUIReader::purge();
	CCArmatureDataManager::purge();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();

	BaseLobbyScene::onExit();
}
void ClassicLobbyScene::initTCPLogon(int index){
	PopDialogBox *pdb = PopDialogBoxLoading::create();
	this->addChild(pdb);
	pdb->setTag(TAG_LOADING);
	//pdb->setSocketName(SOCKET_LOGON_ROOM);

	//TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_LOGON_ROOM);

	std::string ip = "";
	short port = 0;
	//TCPSocketControl *tcp=TCPSocketControl::sharedTCPSocketControl();
	switch (getGameItem())
	{
	/*case ITEM_0:
	{
		ip = DataModel::sharedDataModel()->tagGameServerListOxTwo[index]->szServerAddr;
		port = DataModel::sharedDataModel()->tagGameServerListOxTwo[index]->wServerPort;
	}
	break;*/
	case ITEM_1:
	{
		ip = DataModel::sharedDataModel()->tagGameServerListOxOneByOne[index]->szServerAddr;
		port = DataModel::sharedDataModel()->tagGameServerListOxOneByOne[index]->wServerPort;
	}
	break;
	case ITEM_2:
	{
		ip = DataModel::sharedDataModel()->tagGameServerListSixSwap[index]->szServerAddr;
		port = DataModel::sharedDataModel()->tagGameServerListSixSwap[index]->wServerPort;
	}
	break;
	case ITEM_3:
	{
		ip = DataModel::sharedDataModel()->tagGameServerListOxHundred[index]->szServerAddr;
		port = DataModel::sharedDataModel()->tagGameServerListOxHundred[index]->wServerPort;
	}
	break;
	default:
		break;
	}
	//tcp->listerner=new GameListerner();
	//tcp->listerner = new LogonGameListerner();
	//tcp->startSendThread();
	GameIngMsgHandler::sharedGameIngMsgHandler()->connectServer(ip, port);
	onEventConnect(1, NULL, 0);
	//getSocket()->createSocket(ip, port, new GameIngListerner());
}
//更新房间列表
void  ClassicLobbyScene::updateRoomList(){
	switch (getGameItem())
	{
	/*case ITEM_0://二人牛牛
	{
		updateRoom(DataModel::sharedDataModel()->tagGameServerListOxTwo);
	}
	break;*/
	case ITEM_1://通比牛牛
	{
		updateRoom(DataModel::sharedDataModel()->tagGameServerListOxOneByOne);
	}
	break;
	case ITEM_2://六人换牌
	{
		updateRoom(DataModel::sharedDataModel()->tagGameServerListSixSwap);
	}
	break;
	case ITEM_3://百人牛牛
	{
		updateRoom(DataModel::sharedDataModel()->tagGameServerListOxHundred);
	}
		break;
	default:
		break;
	}
}
//更新房间列表
void  ClassicLobbyScene::updateRoom(std::vector<tagGameServer *> vec){
	pLVViewRoom->scrollToLeft(0.3, true);
	UIListView *pLVTemp = pLVViewRoom;
	pLVTemp->removeAllItems();

	int tempSize = vec.size();
	if (tempSize == 0)
	{
		return;
	}

	for (int i = 0; i < tempSize; i++)
	{
		int inserterPos = pLVTemp->getItems()->count();
		pLVTemp->insertDefaultItem(inserterPos);
		//按键
		UIButton *pBScene = static_cast<UIButton*>(pLVTemp->getItem(inserterPos)->getChildByName("ButtonScene"));
		pBScene->setTag(i+1);
		pBScene->addTouchEventListener(this, SEL_TouchEvent(&ClassicLobbyScene::menuStar));
		//icon
		UIImageView *pIVIcon = static_cast<UIImageView*>(pBScene->getChildByName("ImageItemIcon"));
		pIVIcon->loadTexture(CCString::createWithFormat("u_c_scene_%d.png",(i%4)+1)->getCString(), UI_TEX_TYPE_PLIST);
		//房间名
		UILabel *pLRoomName0 = static_cast<UILabel*>(pBScene->getChildByName("LabelRoomName0"));
		UILabel *pLRoomName1 = static_cast<UILabel*>(pBScene->getChildByName("LabelRoomName1"));
		pLRoomName0->setText(GBKToUTF8(vec[i]->szGameLevel));
		pLRoomName1->setText(GBKToUTF8(vec[i]->szGameLevel));
		//限制条件
	
		UILabel *description = static_cast<UILabel*>(pBScene->getChildByName("ImageLabelBg")->getChildByName("LabelDescription"));
		description->setText(Tools::GBKToUTF8(vec[i]->szDescription));

		/*//拍卖物品名称
		UILabel *pGoodsName = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("LabelGoodsContent"));
		
		pIVCell->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxAuction::onMenuSelectMyAuctionCell));
		pIVCell->setTag(i);*/
	}

	/*for (int i = 0; i < MAX_LEVEL_COUNT; i++)
	{
		if (i < vec.size())
		{
			pBLevel[i]->setEnabled(true);
			//房间描述
			UILabel *description = static_cast<UILabel*>(pBLevel[i]->getChildByName("ImageLabelBg")->getChildByName("LabelDescription"));
			description->setText(Tools::GBKToUTF8(vec[i]->szDescription));
		}
		else
		{
			pBLevel[i]->setEnabled(false);
		}
	}*/
}
//初始化游戏选项触摸事件
void ClassicLobbyScene::initItemTouchEvent(){
	for (int i = 0; i < pLVItems->getItems()->count(); i++)
	{
		UIPanel *pItem = static_cast<UIPanel*>(pLVItems->getItem(i));
		UICheckBox *pCheckBox = static_cast<UICheckBox*>(pItem->getChildByName("CheckBoxItme"));
		pCheckBox->addEventListenerCheckBox(this, SEL_SelectedStateEvent(&ClassicLobbyScene::onCheckBoxSelectedStateEvent));
	}
}
//选择游戏项
void ClassicLobbyScene::selectGameItem(int iItemIndex){
	for (int i = 0; i < pLVItems->getItems()->count(); i++)
	{
		UIPanel *pItem = static_cast<UIPanel*>(pLVItems->getItem(i));
		UICheckBox *pCheckBox = static_cast<UICheckBox*>(pItem->getChildByName("CheckBoxItme"));
		UIImageView *pIGameName = static_cast<UIImageView*>(pItem->getChildByName("ImageGameName"));
		if (i == iItemIndex)
		{
			pCheckBox->setSelectedState(true);
			pCheckBox->setTouchEnabled(false);
			pIGameName->setColor(ccc3(255, 255, 255));
			setGameItem((GameItem)(iItemIndex + 1));
			backGameItem = getGameItem();
			updateRoomList();
		}
		else
		{
			pCheckBox->setSelectedState(false);
			pCheckBox->setTouchEnabled(true);
			pIGameName->setColor(ccc3(112, 54, 8));
		}
	}

}
void ClassicLobbyScene::menuResetUser(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		popDialogBox();
		break;
	default:
		break;
	}
}
void ClassicLobbyScene::menuStar(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		UIButton *button = (UIButton*)pSender;
		initTCPLogon(button->getTag() - 1);
	}
	break;
	default:
		break;
	}
}
void ClassicLobbyScene::popDialogBox(){
	/*if (TCPSocketControl::sharedTCPSocketControl()->listerner)
	{
		TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_LOGON_ROOM);
	}*/
	GameIngMsgHandler::sharedGameIngMsgHandler()->gameSocket.Destroy(true);

	Tools::setTransitionAnimation(0, 0, GameLobbyScene::scene(false));
}
void ClassicLobbyScene::enterMainSceneByMode(int mode){
	switch (mode)
	{
	/*case ITEM_0:
		Tools::setTransitionAnimation(0, 0, MainSceneOxTwo::scene());
		break;*/
	case ITEM_1:
		Tools::setTransitionAnimation(0, 0, MainSceneOxOneByOne::scene());
		break;
	case ITEM_2:
		Tools::setTransitionAnimation(0, 0, MainSceneOxSixSwap::scene());
		break;
	case ITEM_3:
	{
		Tools::setTransitionAnimation(0, 0, MainSceneOxHundred::scene());
	}
		break;
	default:
		break;
	}
}
void ClassicLobbyScene::update(float delta){
	if (isReadMessage)
	{
		//MessageQueue::update(delta);
		MessageQueueGameIng::update(delta);
	}
	/*if (DataModel::sharedDataModel()->isSit)
	{
	DataModel::sharedDataModel()->isSit=false;
	enterMainSceneByMode(1);
	}*/
}
void ClassicLobbyScene::onPlay(CCObject *obj){
	PopDialogBox *pdb = (PopDialogBoxLoading*)this->getChildByTag(TAG_LOADING);
	pdb->removeFromParentAndCleanup(true);

	isEnterGame = true;
}
void ClassicLobbyScene::onConfigFinish(CCObject *obj){
#if (DEBUG_TEST==0)
	CMD_GR_UserSitDown sit;
	sit.wTableID=38;
	//sit.wChairID=abs(rand()%2);
	sit.wChairID=1;
	bool isSend=TCPSocketControl::sharedTCPSocketControl()->SendData(MDM_GR_USER,SUB_GR_USER_SITDOWN,&sit, sizeof(sit));
	CCLOG("Classic 坐下:%d",isSend);
#endif
	//	enterMainSceneByMode(1);
}

void ClassicLobbyScene::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_MB_UNCONNECT:
	{
		setGameItem(backGameItem);
		PopDialogBoxLoading *pLoading = (PopDialogBoxLoading*)this->getChildByTag(TAG_LOADING);
		if (pLoading)
		{
			pLoading->removeFromParentAndCleanup(true);
		}
		GameIngMsgHandler::sharedGameIngMsgHandler()->gameSocket.Destroy(false);
		PopDialogBoxTipInfo *tipInfo = PopDialogBoxTipInfo::create();
		this->addChild(tipInfo);
		tipInfo->setTipInfoContent("与服务器断开连接");
	}
		break;
	case MDM_MB_SOCKET://socket连接成功
		onEventConnect(wSubCmdID, pDataBuffer, wDataSize);
		break;
	case MDM_GR_LOGON:
		onEventLogon(wSubCmdID, pDataBuffer, wDataSize);
		break;
		/*case MDM_GR_USER:
			onSubUserState(wSubCmdID,pDataBuffer,wDataSize);
			break;*/
	case MDM_GR_CONFIG://配置
		return configEvent(wSubCmdID, pDataBuffer, wDataSize);
		break;
	default:
		CCLOG("main:%d  sub:%d<<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
		break;
	}
}
//连接成功
void ClassicLobbyScene::onEventConnect(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_SOCKET_OPEN:
	{
		CMD_GR_LogonMobile logonMobile;
		memset(&logonMobile, 0, sizeof(CMD_GR_LogonMobile));

		switch (getGameItem())
		{
		/*case ITEM_0:
		{
			logonMobile.wGameID = 210;
			logonMobile.dwProcessVersion = VERSION_CLIENT;
		}
		break;*/
		case ITEM_1:
		{
			logonMobile.wGameID = 130;
			logonMobile.dwProcessVersion = 17235969;
		}
		break;
		case ITEM_2:
		{
			logonMobile.wGameID = 430;
			logonMobile.dwProcessVersion = 17235969;
		}
		break;
		case ITEM_3:
		{
			logonMobile.wGameID = 30;
			logonMobile.dwProcessVersion = 17039361;
			//logonMobile.wGameID = 430;
			//logonMobile.dwProcessVersion = 17235969;
		}
		break;
		default:
			break;
		}


		//设备类型
		logonMobile.cbDeviceType = DEVICE_TYPE_ANDROID;
		logonMobile.wBehaviorFlags = BEHAVIOR_LOGON_IMMEDIATELY;
		logonMobile.wPageTableCount = 10;

		logonMobile.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;

		MD5 m;
		//MD5::char8 str[] = "z12345678";
		//m.ComputMd5(str,sizeof(str)-1);
		m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(), DataModel::sharedDataModel()->sLogonPassword.length());
		std::string md5PassWord = m.GetMd5();
		strcpy(logonMobile.szPassword, md5PassWord.c_str());

		strcpy(logonMobile.szMachineID, "123");

		bool isSend = GameIngMsgHandler::sharedGameIngMsgHandler()->gameSocket.SendData(MDM_GR_LOGON, SUB_GR_LOGON_MOBILE, &logonMobile, sizeof(logonMobile));
					 
	}
	break;
	default:
		break;
	}
}
//配置
void ClassicLobbyScene::configEvent(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GR_CONFIG_COLUMN:
	{
		/*int size=sizeof(tagColumnItem);
		//效验参数
		assert(wDataSize >= sizeof(CMD_GR_ConfigColumn));
		if (wDataSize < sizeof(CMD_GR_ConfigColumn)) return false;
		*/
		CMD_GR_ConfigColumn *lf = (CMD_GR_ConfigColumn*)pDataBuffer;
		CCLOG("列表配置");
	}
	break;
	case SUB_GR_CONFIG_SERVER:
	{
		int size = sizeof(CMD_GR_ConfigServer);
		//效验参数
		assert(wDataSize >= sizeof(CMD_GR_ConfigServer));
		if (wDataSize < sizeof(CMD_GR_ConfigServer)) return;

		CMD_GR_ConfigServer *lf = (CMD_GR_ConfigServer*)pDataBuffer;
		CCLOG("房间配置");
	}
	break;
	case SUB_GR_CONFIG_PROPERTY:
	{
		CMD_GR_ConfigProperty *lf = (CMD_GR_ConfigProperty*)pDataBuffer;
		CCLOG("道具配置");
	}
	break;
	case SUB_GR_CONFIG_FINISH:
	{
		CCLOG("配置完成");
		unscheduleUpdate();

		if (getGameItem()==ITEM_3)
		{
			//构造数据
			CMD_GF_GameOption GameOption;
			GameOption.dwFrameVersion = VERSION_FRAME;
			GameOption.cbAllowLookon = 0;
			GameOption.dwClientVersion = VERSION_CLIENT;
			//发送
			//bool isSend = getSocket()->SendData(MDM_GF_FRAME, SUB_GF_GAME_OPTION, &GameOption, sizeof(GameOption));
			bool isSend = GameIngMsgHandler::sharedGameIngMsgHandler()->gameSocket.SendData(MDM_GF_FRAME, SUB_GF_GAME_OPTION, &GameOption, sizeof(GameOption));
			if (isSend)
			{

				Tools::setTransitionAnimation(0, 0, MainSceneOxHundred::scene());
				//enterMainSceneByMode(1);
			}
		}
		else
		{
			enterMainSceneByMode(getGameItem());
		}
		/*unscheduleUpdate();
		*/

		
		//构造数据
		/*CMD_GF_GameOption GameOption;
		GameOption.dwFrameVersion=VERSION_FRAME;
		GameOption.cbAllowLookon=0;
		GameOption.dwClientVersion=VERSION_CLIENT;
		//发送
		bool isSend = TCPSocketControl::sharedTCPSocketControl()->SendData(MDM_GF_FRAME, SUB_GF_GAME_OPTION, &GameOption, sizeof(GameOption));
		if (isSend)
		{
		MTNotificationQueue::sharedNotificationQueue()->postNotification(S_L_CONFIG_FINISH,NULL);
		}*/
	}
	break;
	default:
		break;
	}
}
//登录
void ClassicLobbyScene::onEventLogon(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GR_UPDATE_NOTIFY:
	{
		//效验参数
		assert(wDataSize >= sizeof(CMD_GR_UpdateNotify));
		if (wDataSize < sizeof(CMD_GR_UpdateNotify)) return;

		CMD_GR_UpdateNotify *lf = (CMD_GR_UpdateNotify*)pDataBuffer;
		//CCLOG("升级提示 %d");
		setGameItem(backGameItem);
		CCLOG("升级提示:%ld  %ld  %ld<<%s>>", lf->dwCurrentClientVersion, lf->dwCurrentFrameVersion, lf->dwCurrentPlazaVersion, __FUNCTION__);
	}
	break;
	case SUB_GR_LOGON_SUCCESS:
	{
		//效验参数
		assert(wDataSize >= sizeof(CMD_GR_LogonSuccess));
		if (wDataSize < sizeof(CMD_GR_LogonSuccess)) return;

		CMD_GR_LogonSuccess *lf = (CMD_GR_LogonSuccess*)pDataBuffer;
		CCLOG("登录成功");
	}
	break;
	case SUB_GR_LOGON_FINISH:{
		CCLOG("登录完成");
	}
							 break;
	case SUB_GR_LOGON_FAILURE:
	{
		CMD_GR_LogonFailure *lf = (CMD_GR_LogonFailure*)pDataBuffer;
		CCLOG("登录失败:%s", Tools::GBKToUTF8(lf->szDescribeString).c_str());

		this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
		//TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_LOGON_GAME);
		//TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_LOGON_ROOM);
		GameIngMsgHandler::sharedGameIngMsgHandler()->gameSocket.Destroy(true);

		PopDialogBoxTipInfo *tipInfo = PopDialogBoxTipInfo::create();
		this->addChild(tipInfo);
		tipInfo->setTipInfoContent(GBKToUTF8(lf->szDescribeString).c_str());
		setGameItem(backGameItem);
	}
	break;
	default:
		break;
	}
}

//用户状态
void ClassicLobbyScene::onSubUserState(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GR_USER_STATUS:
	{
		//效验参数
		assert(wDataSize >= sizeof(CMD_GR_UserStatus));
		if (wDataSize < sizeof(CMD_GR_UserStatus)) return;

		CMD_GR_UserStatus *info = (CMD_GR_UserStatus*)pDataBuffer;
		switch (info->UserStatus.cbUserStatus)
		{
		case US_SIT://坐下
		{
			CCLOG("state==sit-----------%ld<<%s>>", info->dwUserID, __FUNCTION__);
			if (info->dwUserID == DataModel::sharedDataModel()->userInfo->dwUserID){
				unscheduleUpdate();
				//DataModel::sharedDataModel()->isSit=true;
				CCLOG("坐下:table: %d desk:%d", info->UserStatus.wTableID, info->UserStatus.wChairID);
				DataModel::sharedDataModel()->userInfo->wTableID = info->UserStatus.wTableID;
				DataModel::sharedDataModel()->userInfo->wChairID = info->UserStatus.wChairID;

				//enterMainSceneByMode(1);
				//构造数据
				CMD_GF_GameOption GameOption;
				GameOption.dwFrameVersion = VERSION_FRAME;
				GameOption.cbAllowLookon = 0;
				GameOption.dwClientVersion = VERSION_CLIENT;
				//发送
				bool isSend = GameIngMsgHandler::sharedGameIngMsgHandler()->gameSocket.SendData(MDM_GF_FRAME, SUB_GF_GAME_OPTION, &GameOption, sizeof(GameOption));
				if (isSend)
				{
					enterMainSceneByMode(getGameItem());
				}
			}
		}
		break;
		case US_FREE://站立
		{
			CCLOG("state==free-----------%ld", info->dwUserID);
			if (info->dwUserID == DataModel::sharedDataModel()->userInfo->dwUserID)
			{
				//MTNotificationQueue::sharedNotificationQueue()->postNotification(S_L_US_FREE,NULL);
			}
			else
			{

			}
		}
		break;
		case US_READY://同意
		{
			CCLOG("state==ready-----------%ld<<%s>>", info->dwUserID, __FUNCTION__);
		}
		break;
		case US_PLAYING:
		{
			if (info->dwUserID == DataModel::sharedDataModel()->userInfo->dwUserID)
			{
				//MTNotificationQueue::sharedNotificationQueue()->postNotification(S_L_CONFIG_FINISH,NULL);
			}
			CCLOG("state==playing-----------%ld", info->dwUserID);
		}
		break;
		default:
			CCLOG("state==Other userID:%ld 状态：%d<<%s>>", info->dwUserID, info->UserStatus.cbUserStatus, __FUNCTION__);
			break;
		}
	}
	break;
	default:
		break;
	}

}
//复选框回调（选择游戏）
void ClassicLobbyScene::onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type){
	switch (type)
	{
	case CHECKBOX_STATE_EVENT_SELECTED:
	{
		UICheckBox *checkBox = (UICheckBox*)pSender;
		selectGameItem(checkBox->getTag() - 1);
	}
	break;
	case CHECKBOX_STATE_EVENT_UNSELECTED:
	{

	}
	break;
	default:
		break;
	}
}
//快速游戏
void ClassicLobbyScene::quickGame(){
	backGameItem = getGameItem();
	setGameItem(ITEM_1);
	initTCPLogon(0);
}