//
//  GameHUD.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#include "GameControlBase.h"
#include "../../Tools/GameConfig.h"
#include "../../Tools/DataModel.h"
#include "../../GameLobby/GameLobbyScene.h"
#include "../../Tools/Tools.h"
#include "../../Network/TCPSocket/TCPSocketControl.h"
#include "../../Network/CMD_Server/Packet.h"
#include "../../Network/CMD_Server/CMD_GameServer.h"
#include "../../Network/CMD_Server/cmd_ox.h"
//#include "QueueData.h"
#include "../../Network/SEvent.h"
#include "../../MainScene/MainSceneBase.h"
#include "../../Network/CMD_Server/PacketAide.h"
GameControlBase::GameControlBase()
	:pEndLayer(NULL)
	, pLTimerPromptContent(NULL)
	, isExitGame(false)
	{
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(CCS_PATH_SCENE(AnimationActionPrompt.ExportJson));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(CCS_PATH_SCENE(AnimationGameIng.ExportJson));
	schedule(SEL_SCHEDULE(&GameControlBase::updateTimer), 1);
	scheduleUpdate();
}
GameControlBase::~GameControlBase(){
	unschedule(SEL_SCHEDULE(&GameControlBase::updateTimer));
	unscheduleUpdate();
	TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_LOGON_ROOM);
}
void GameControlBase::onEnter(){
	CCLayer::onEnter();
	UILayer *pWidget = UILayer::create();
	this->addChild(pWidget);
	//初始化操作者动画
	initActionPrompt();

	UILayout *pLayout = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIGameHUD.ExportJson)));
	pWidget->addWidget(pLayout);

	UIButton* button = static_cast<UIButton*>(pWidget->getWidgetByName("buttonPause"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControlBase::menuPause));

	//设置牛牛容器
	pOptOx = static_cast<UIPanel*>(pWidget->getWidgetByName("optOxPanel"));
	pOptOx->setEnabled(false);

	button = static_cast<UIButton*>(pWidget->getWidgetByName("buttonOx"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControlBase::menuOpenCard));

	button = static_cast<UIButton*>(pWidget->getWidgetByName("buttonPrompt"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControlBase::menuPrompt));

	//绑定准备按键
	button = static_cast<UIButton*>(pWidget->getWidgetByName("buttonReady"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControlBase::menuReady));
	//准备容器
	pPanelReady = static_cast<UIPanel*>(pWidget->getWidgetByName("PanelReady"));
	//换牌容器
	pPanelSwapCard = static_cast<UIPanel*>(pWidget->getWidgetByName("PanelSwapCard"));
	button = static_cast<UIButton*>(pWidget->getWidgetByName("ButtonSwapCard"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControlBase::menuSwapCard));
	button = static_cast<UIButton*>(pWidget->getWidgetByName("ButtonDontSwapCard"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControlBase::menuDontSwapCard));

	pIVChangeCard = static_cast<UIImageView*>(pPanelSwapCard->getChildByName("ImageSwapBg"));

	pPanelSwapCard->setEnabled(false);
	//抢庄容器
	pFightForBanker = static_cast<UIPanel*>(pWidget->getWidgetByName("fightForBankerPanel"));
	pFightForBanker->setEnabled(false);
	//不抢庄
	button = static_cast<UIButton*>(pFightForBanker->getChildByName("notFightButton"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControlBase::menuNotFight));
	//抢庄
	button = static_cast<UIButton*>(pFightForBanker->getChildByName("fightButton"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControlBase::menuFight));
	//投注容器
	pBetting = static_cast<UIPanel*>(pWidget->getWidgetByName("bettingPanel"));
	pBetting->setEnabled(false);
	//加注按键
	for (int i = 0; i < 4; i++)
	{
		pbBetting[i] = static_cast<UIButton*>(pBetting->getChildByName(CCString::createWithFormat("bettingButton%d", i + 1)->getCString()));
		pbBetting[i]->addTouchEventListener(this, SEL_TouchEvent(&GameControlBase::menuBetting));
	}
	//设置用户名
	const char *name = Tools::GBKToUTF8(DataModel::sharedDataModel()->userInfo->szNickName);
	//getMainScene()->playerLayer->setUserName(3,name);
	//初始化计时器
	initTimer(pWidget);
	resetTimer(MAX_TIMER, NULL);
	//添加监听事件
	//CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(BaseGameControl::OnEventGameMessage),S_L_GAME_ING,NULL);
	//	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameControlBase::OnUserFree), S_L_US_FREE, NULL);
	//	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameControlBase::OnUserEnter), S_L_US_ENTER, NULL);
	//主动调用一次
	//onUserEnter();

	this->setTouchEnabled(true);
	this->setTouchPriority(1);
	this->setTouchMode(kCCTouchesOneByOne);
}
void GameControlBase::onExit(){
	//移除监听事件 
	//CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, S_L_GAME_ING); 
	//CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, S_L_US_FREE);
	//CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, S_L_US_ENTER);
	CCLayer::onExit();
}
//初始化操作者提示动画
void GameControlBase::initActionPrompt(){
	pArmatureActionPrompt = CCArmature::create("AnimationActionPrompt");
	this->addChild(pArmatureActionPrompt, 2);
	hideActionPrompt();
}
void GameControlBase::initTimer(UILayer *pWidget){
	pITimer = static_cast<UIImageView*>(pWidget->getWidgetByName("ImageTimer"));
	pITimer->setVisible(false);

	pLTimerNum = static_cast<UILabelAtlas*>(pWidget->getWidgetByName("AtlasLabelTimer"));
	iTimerCount = -1;
}
//触摸
bool GameControlBase::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	CCPoint touchPoint = this->convertTouchToNodeSpace(pTouch);
	CCLog("meChiarID:%d <<%s>>", DataModel::sharedDataModel()->userInfo->wChairID, __FUNCTION__);
	unsigned short beginPos = getViewChairID(DataModel::sharedDataModel()->userInfo->wChairID);
	getMainScene()->cardLayer->touchCard(beginPos,touchPoint);
	return true;
}
//获取视图位置
int GameControlBase::getViewChairID(int severChairID){
	int viewChair = 3 - DataModel::sharedDataModel()->userInfo->wChairID;
	if (viewChair < 0)
	{
		viewChair += MAX_CHAIR_COUNT;
	}
	viewChair += severChairID;
	viewChair %= MAX_CHAIR_COUNT;
	return viewChair;
}
void GameControlBase::resetTimer(float time, const char * promptContent){
	iTimerCount = time;
	pLTimerNum->setStringValue(CCString::createWithFormat("%d", iTimerCount)->getCString());
	pITimer->setVisible(true);
	if (pLTimerPromptContent)
	{
		pLTimerPromptContent->setText(promptContent);
		pITimer->setSize(CCSize(pLTimerPromptContent->getContentSize().width + 50, pITimer->getContentSize().height));
	}
}
//显示计时器
void GameControlBase::showTimer(){
	pITimer->setVisible(true);
}
//隐藏计时器
void GameControlBase::hideTimer(bool bIsReset){
	pITimer->setVisible(false);
	if (bIsReset)
	{
		iTimerCount = -1;
	}
}
void GameControlBase::updateTimer(float dt){
	iTimerCount--;
	if (iTimerCount < 0)
	{
		return;
	}
	if (iTimerCount == 0)
	{
		delayedAction();
	}

	pLTimerNum->setStringValue(CCString::createWithFormat("%d", iTimerCount)->getCString());
}
void GameControlBase::delayedAction(){
	switch (getMainScene()->getGameState())
	{
	case MainSceneOxTwo::STATE_READY:
	{

	}
	break;
	case MainSceneOxTwo::STATE_CALL_BANKER:
	{
		menuNotFight(NULL, TOUCH_EVENT_ENDED);
	}
	break;
	case MainSceneOxTwo::STATE_BETTING:
	{
		UIButton *button = UIButton::create();
		button->setTag(1);
		menuBetting(button, TOUCH_EVENT_ENDED);
	}
	break;
	case MainSceneOxTwo::STATE_OPT_OX:
	{
		menuOpenCard(NULL, TOUCH_EVENT_ENDED);
	}
	break;
	case MainSceneOxTwo::STATE_END:
	{

	}
	break;
	case MainSceneBase::STATE_SWAP_CARD:
	{
		menuDontSwapCard(NULL, TOUCH_EVENT_ENDED);
	}
		break;
	default:
		break;
	}
}
void GameControlBase::menuPause(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		//TCPSocketControl::sharedTCPSocketControl()->stopSocket();
		
		standUpWithExit();
		//Tools::setTransitionAnimation(0, 0, GameLobbyScene::scene(false));
	}
	break;
	default:
		break;
	}
}
//开牌
void GameControlBase::menuOpenCard(CCObject* pSender, TouchEventType type){
	/*switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
	hideTimer(true);
	getMainScene()->cardLayer->sortingOx(getMeChairID(),3);

	showActionPrompt(3);
	pOptOx->setEnabled(false);
	CMD_C_OxCard OxCard;
	OxCard.bOX=GetOxCard(DataModel::sharedDataModel()->card[getMeChairID()],5);
	//发送信息
	bool isSend=TCPSocketControl::sharedTCPSocketControl()->SendData(MDM_GF_GAME,SUB_C_OPEN_CARD,&OxCard,sizeof(OxCard));
	getMainScene()->setGameStateWithUpdate(MainSceneOxTwo::STATE_WAIT);
	}
	break;
	default:
	break;
	}*/
}
//提示
void GameControlBase::menuPrompt(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{

		bool isOx = getMainScene()->cardLayer->promptOx(getMeChairID());
		if (!isOx)
		{
			menuOpenCard(NULL, TOUCH_EVENT_ENDED);
		}
	}
	break;
	default:
		break;
	}
}
//更换桌子
void GameControlBase::menuChangeChair(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		//CCStringMake 
		//SUB_GR_USER_CHAIR_REQ
	}
	break;
	default:
		break;
	}
}
//准备按键
void GameControlBase::menuReady(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		hideTimer(true);
		//隐藏准备
		pPanelReady->setEnabled(false);
		//发送准备指使
		bool isSend = getSocket()->SendData(MDM_GF_FRAME, SUB_GF_USER_READY);
		//设置主状态为准备状态
		getMainScene()->setGameState(MainSceneOxTwo::STATE_READY);
		/*//获取按键子控件并隐藏
		CCArray *arrayImage = mButton->getChildren();
		for (int i = 0; i < arrayImage->count(); i++)
		{
		CCNode *image = (CCNode *)arrayImage->objectAtIndex(i);
		image->setVisible(false);
		}*/


		getMainScene()->onEventReadyFnish();
	}
	break;
	default:
		break;
	}
}

//不抢庄
void GameControlBase::menuNotFight(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		hideTimer(true);
		pFightForBanker->setEnabled(false);
		//getMainScene()->setGameStateWithUpdate(MainScene::STATE_BETTING);
		getMainScene()->setGameStateWithUpdate(MainSceneOxTwo::STATE_WAIT);
		//设置变量
		CMD_C_CallBanker CallBanker;
		CallBanker.bBanker = (BYTE)0;
		//发送信息
		getSocket()->SendData(MDM_GF_GAME, SUB_C_CALL_BANKER, &CallBanker, sizeof(CallBanker));
	}
	break;
	default:
		break;
	}
}
//抢庄
void GameControlBase::menuFight(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		hideTimer(true);
		pFightForBanker->setEnabled(false);
		//getMainScene()->setGameStateWithUpdate(MainScene::STATE_OPT_OX);
		getMainScene()->setGameStateWithUpdate(MainSceneOxTwo::STATE_WAIT);

		//设置变量
		CMD_C_CallBanker CallBanker;
		CallBanker.bBanker = (BYTE)1;

		//发送信息
		getSocket()->SendData(MDM_GF_GAME, SUB_C_CALL_BANKER, &CallBanker, sizeof(CallBanker));
		//SendSocketData(SUB_C_CALL_BANKER,&CallBanker,sizeof(CallBanker));
	}
	break;
	default:
		break;
	}
}
//投注
void GameControlBase::menuBetting(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		hideTimer(true);
		pBetting->setEnabled(false);
		UIButton *button = (UIButton*)pSender;
		int bTemp = button->getTag();
		long long lCurrentScore = 0;
		if (bTemp == 4)lCurrentScore = MAX(DataModel::sharedDataModel()->m_lTurnMaxScore / 8, 1L);
		else if (bTemp == 3)lCurrentScore = MAX(DataModel::sharedDataModel()->m_lTurnMaxScore / 4, 1L);
		else if (bTemp == 2)lCurrentScore = MAX(DataModel::sharedDataModel()->m_lTurnMaxScore / 2, 1L);
		else if (bTemp == 1)lCurrentScore = MAX(DataModel::sharedDataModel()->m_lTurnMaxScore, 1L);

		//getMainScene()->setGameStateWithUpdate(MainScene::STATE_OPT_OX);
		getMainScene()->setGameStateWithUpdate(MainSceneOxTwo::STATE_WAIT);

		//发送消息
		CMD_C_AddScore AddScore;
		AddScore.lScore = lCurrentScore;
		//发送信息
		getSocket()->SendData(MDM_GF_GAME, SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));
	}
	break;
	default:
		break;
	}
}
//换牌
void GameControlBase::menuSwapCard(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
        return;
		showActionPrompt(ACTION_PROMPT_OPT_CARD, ccp(0, -DataModel::sharedDataModel()->deviceSize.height / 2 + 50));
		getMainScene()->setGameStateWithUpdate(MainSceneBase::STATE_SWAP_CARD_ING);

		
		WORD wMeChairID = getViewChairID(DataModel::sharedDataModel()->userInfo->wChairID);
		getMainScene()->cardLayer->touchCard(wMeChairID, CCPointZero);
		/*if (getMainScene()->cardLayer->getAllUpCardCount(wMeChairID)>0)
		{
			pIVChangeCard->setEnabled(true);
		}*/
		
		

		//CCLog("swapCard------------- <<%s>>", __FUNCTION__);
		//hideActionPrompt();
	}
	break;
	default:
		break;
	}
}
//不换牌
void GameControlBase::menuDontSwapCard(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		onUserChangeCard(0,0);
		//hideActionPrompt();
	}
	break;
	default:
		break;
	}
}
//隐藏所有操作按键
void GameControlBase::hideAllActionPanel(){
	pOptOx->setEnabled(false);
	pPanelReady->setEnabled(false);
	pFightForBanker->setEnabled(false);
	pBetting->setEnabled(false);
	pPanelSwapCard->setEnabled(false);
}
void GameControlBase::updateState(){
	switch (getMainScene()->getGameState())
	{
	case MainSceneOxTwo::STATE_READY:
	{
		
		resetTimer(MAX_TIMER, NULL);
		if (IsLookonMode())
		{
			hideAllActionPanel();
			return;
		}
		//移除结算层
		if (pEndLayer)
		{
			pEndLayer->removeFromParentAndCleanup(true);
			pEndLayer = NULL;
		}
		pPanelReady->setEnabled(true);
		pOptOx->setEnabled(false);
	}
	break;
	case MainSceneOxTwo::STATE_CALL_BANKER:
	{
		resetTimer(MAX_TIMER, NULL);
		if (IsLookonMode() || !isPalyerState())
		{
			hideAllActionPanel();
			return;
		}
		pFightForBanker->setEnabled(true);
		pPanelReady->setEnabled(false);
	}
	break;
	case MainSceneBase::STATE_WAIT_SWAP_CARE://等待其它玩家换牌
	{
		resetTimer(MAX_TIMER, NULL);
		pPanelSwapCard->setEnabled(false);
		pFightForBanker->setEnabled(false);

	}
		break;
	case MainSceneOxTwo::STATE_OPT_OX:
	{
		resetTimer(MAX_TIMER, NULL);
		if (IsLookonMode() || !isPalyerState())
		{
			hideAllActionPanel();
			return;
		}
		pOptOx->setEnabled(true);
		pPanelSwapCard->setEnabled(false);
		pFightForBanker->setEnabled(false);
	}
	break;
	case MainSceneOxTwo::STATE_BETTING:
	{
		resetTimer(MAX_TIMER, NULL);
		if (IsLookonMode()||!isPalyerState())
		{
			hideAllActionPanel();
			return;
		}
		pBetting->setEnabled(true);
	}
	break;
	case MainSceneOxTwo::STATE_SEND_CARD:
	{
		//pBetting->setEnabled(false);
	}
	break;
	case MainSceneOxTwo::STATE_END:
	{
		resetTimer(MAX_TIMER, NULL);
// 		if (IsLookonMode())
// 		{
// 			hideAllActionPanel();
// 			return;
// 		}
		pOptOx->setEnabled(false);
		pPanelReady->setEnabled(false);
		pFightForBanker->setEnabled(false);
		pBetting->setEnabled(false);
		pPanelSwapCard->setEnabled(false);
	}
	break;
	case MainSceneBase::STATE_SWAP_CARD://换牌
	{
		resetTimer(MAX_TIMER, NULL);
		if (IsLookonMode() || !isPalyerState())
		{
			hideAllActionPanel();
			return;
		}
		pPanelSwapCard->setEnabled(true);

		UIImageView *pSwapBg = (UIImageView*)pPanelSwapCard->getChildByName("ImageSwapBg");
		pSwapBg->setEnabled(false);
	}
		break;
	default:
		break;
	}
}
//显示指定索引提示动画
void GameControlBase::showActionPrompt(int promptIndex,CCPoint offsetPos){
	CCPoint displayPos = ccp(DataModel::sharedDataModel()->deviceSize.width / 2, DataModel::sharedDataModel()->deviceSize.height / 2);
	CCPoint armaturePos = ccpAdd(displayPos,offsetPos);
	pArmatureActionPrompt->setPosition(armaturePos);

	pArmatureActionPrompt->setVisible(true);
	pArmatureActionPrompt->getAnimation()->play(CCString::createWithFormat("Animation%d", promptIndex)->getCString());
}
void GameControlBase::hideActionPrompt(){
	pArmatureActionPrompt->setVisible(false);
}
//获取庄家视图位置
int GameControlBase::getBankViewID(){
	if (getMeChairID() == wBankerUser)
	{
		return 3;
	}
	return 0;
}
//获取我的椅子位置
int GameControlBase::getMeChairID(){
	return DataModel::sharedDataModel()->userInfo->wChairID;
}


void GameControlBase::update(float delta){
	MessageQueue::update(delta);
	//OnEventGameMessage(NULL);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/*//网络消息
void BaseGameControl::OnEventGameMessage(CCObject *pObj){
if (DataModel::sharedDataModel()->readDataQueue.size()<=0)
{
return;
}
//std::list<ReadData>::iterator iter;
//iter=DataModel::sharedDataModel()->readDataList.begin();
ReadData iter=DataModel::sharedDataModel()->readDataQueue.front();
switch (iter.wMainCmdID)
{
case MDM_GF_GAME://游戏命令
//OnEventGameIng(iter->wSubCmdID,iter->sReadData,iter->wDataSize);
break;
case SUB_GR_USER_STATUS://用户状态
OnEventUserState(iter.wSubCmdID,iter.sReadData,iter.wDataSize);
break;
default:
break;
}
DataModel::sharedDataModel()->readDataQueue.pop();
}*/
void GameControlBase::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_GF_GAME:
		onEventGameIng(wSubCmdID, pDataBuffer, wDataSize);
		break;
	case MDM_GR_USER://用户信息
		onSubUserState(wSubCmdID, pDataBuffer, wDataSize);
		break;
	case MDM_GF_FRAME://框架命令
		frameEvent(wSubCmdID, pDataBuffer, wDataSize);
		break;
	default:
		CCLog("---------------- %d  %d <<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
		break;
	}
}

void GameControlBase::frameEvent(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GF_GAME_STATUS:
	{
		CCLog("%s<<%s>>", Tools::GBKToUTF8("游戏状态 "), __FUNCTION__);
		OnSocketSubGameStatus(pDataBuffer, wDataSize);
	}
	break;
	case SUB_GF_SYSTEM_MESSAGE:
	{
		CCLog("系统消息<<%s>>", __FUNCTION__);
	}
	break;
	case SUB_GF_GAME_SCENE:
	{
		CCLog("游戏场景<<%s>>", __FUNCTION__);
		OnEventSceneMessage(pDataBuffer, wDataSize);
	}
	break;
	default:
		CCLog("----------------sub:%d<<%s>>", wSubCmdID, __FUNCTION__);
		break;
	}
}
//游戏状态
bool GameControlBase::OnSocketSubGameStatus(void * pData, WORD wDataSize)
{
	//效验参数
	assert(wDataSize == sizeof(CMD_GF_GameStatus));
	if (wDataSize != sizeof(CMD_GF_GameStatus)) return false;

	//消息处理
	CMD_GF_GameStatus * pGameStatus = (CMD_GF_GameStatus *)pData;

	//设置变量
	m_cbGameStatus = pGameStatus->cbGameStatus;
	m_bAllowLookon = pGameStatus->cbAllowLookon ? true : false;
	//US_LOOKON
	
	return true;
}
//游戏场景
bool GameControlBase::OnEventSceneMessage(void * pData, WORD wDataSize){
	return true;
}
//是不是观察者
bool GameControlBase::IsLookonMode()
{
	//if (m_pIMySelfUserItem == NULL) return true;
	//return (m_pIMySelfUserItem->GetUserStatus() == US_LOOKON);
	return m_cbGameStatus == US_LOOKON;
}

//用户叫庄
bool GameControlBase::OnSubCallBanker(const void * pBuffer, WORD wDataSize){
	//效验数据 
	int size = sizeof(CMD_S_CallBanker);
	if (wDataSize != sizeof(CMD_S_CallBanker)) return false;
	CMD_S_CallBanker * pCallBanker = (CMD_S_CallBanker *)pBuffer;
	/*if(!pCallBanker->bFirstTimes && pCallBanker->wCallBanker==getMeChairID())
	{
	CCLog("推庄");
	}*/
	//始叫用户
	if (!IsLookonMode() && pCallBanker->wCallBanker == getMeChairID())
	{
		hideActionPrompt();
		getMainScene()->setGameStateWithUpdate(MainSceneOxTwo::STATE_CALL_BANKER);
	}
	else
	{
		showActionPrompt(ACTION_PROMPT_CALL_BANK,CCPointZero);
	}
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		getMainScene()->playerLayer->pPlayerData[i]->hideActionType();
	}
	/*//游戏当前处于叫庄状态
	//SetGameStatus(GS_TK_CALL);
	if(!pCallBanker->bFirstTimes && pCallBanker->wCallBanker==GetMeChairID())
	{
	//InsertSystemString("推庄");
	//PlayGameSound(AfxGetInstanceHandle(),_T("PUSH_BANK"));
	}

	//用户信息
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
	//视图位置
	m_wViewChairID[i]=SwitchViewChairID(i);

	//获取用户
	IClientUserItem * pUserData=GetTableUserItem(i);
	if (pUserData==NULL) continue;

	//游戏信息
	m_cbPlayStatus[i]=TRUE;

	//用户名字
	lstrcpyn(m_szNickName[i],pUserData->GetNickName(),CountArray(m_szNickName[i]));
	m_GameClientView.m_ptJetonMove[i].SetPoint(0,0);
	}

	//旁观者
	if (IsLookonMode())
	{
	//设置界面
	m_GameClientView.ViewDisplayType(false);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	//m_GameClientView.SetScoreInfo(0,0);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	m_GameClientView.SetUserTableScore(INVALID_CHAIR,0L);

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
	m_GameClientView.ShowScore(i,false);
	m_GameClientView.m_CardControl[i].SetOX(false);
	m_GameClientView.SetUserOxValue(i,0xff);
	m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
	if(i!=m_wViewChairID[GetMeChairID()])m_GameClientView.m_CardControl[i].SetDisplayFlag(false);
	m_GameClientView.m_CardControlOx[i].SetCardData(NULL,0);
	}

	//加注信息
	m_lTurnMaxScore=0L;
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_bUserOxCard,sizeof(m_bUserOxCard));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	}

	//始叫用户
	if(!IsLookonMode() && pCallBanker->wCallBanker==GetMeChairID())
	{
	//控件显示
	//ActiveGameFrame();
	m_GameClientView.m_btBanker.ShowWindow(SW_SHOW);
	m_GameClientView.m_btIdler.ShowWindow(SW_SHOW);
	}

	//等待标志
	WORD wViewID=m_wViewChairID[pCallBanker->wCallBanker];
	m_GameClientView.SetWaitCall((BYTE)wViewID);

	//实际定时器
	if(pCallBanker->wCallBanker==GetMeChairID())
	{
	SetGameClock(pCallBanker->wCallBanker,IDI_CALL_BANKER,TIME_USER_CALL_BANKER);
	}
	else SetGameClock(pCallBanker->wCallBanker,IDI_NULLITY,TIME_USER_CALL_BANKER);

	*/
	return true;
}
//游戏开始
bool GameControlBase::OnSubGameStart(const void * pBuffer, WORD wDataSize){
	//效验数据
	if (wDataSize != sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart = (CMD_S_GameStart *)pBuffer;
	wBankerUser = pGameStart->wBankerUser;
	//CCLog("开始游戏--庄家:%d   最大:::%lld",pGameStart->wBankerUser,pGameStart->lTurnMaxScore);
	DataModel::sharedDataModel()->m_lTurnMaxScore = pGameStart->lTurnMaxScore;
	//设置筹码
	for (int i = 0; i < 4; i++)
	{
		long long lCurrentScore = 0;
		if (i == 3)lCurrentScore = MAX(DataModel::sharedDataModel()->m_lTurnMaxScore / 8, 1L);
		else if (i == 2)lCurrentScore = MAX(DataModel::sharedDataModel()->m_lTurnMaxScore / 4, 1L);
		else if (i == 1)lCurrentScore = MAX(DataModel::sharedDataModel()->m_lTurnMaxScore / 2, 1L);
		else if (i == 0)lCurrentScore = MAX(DataModel::sharedDataModel()->m_lTurnMaxScore, 1L);
		//pbBetting[i]->setTitleText(CCString::createWithFormat("%lld", lCurrentScore)->getCString());
		UILabel *label = static_cast<UILabel*>(pbBetting[i]->getChildByName("LabelGold"));
		std::string sJetton = formatNumber(CCString::createWithFormat("%lld", lCurrentScore)->getCString());
		//label->setText(Tools::GBKToUTF8(sJetton.c_str()));
		label->setText(sJetton.c_str());
	}
	getMainScene()->playerLayer->setBankIcon(getChairIndex(getMeChairID(), wBankerUser));
	if (wBankerUser != getMeChairID())
	{
		hideActionPrompt();
		getMainScene()->setGameStateWithUpdate(MainSceneOxTwo::STATE_BETTING);
	}
	else
	{
		showActionPrompt(ACTION_PROMPT_ADD_SCORE, CCPointZero);
	}

	return true;
}

//用户下注
bool GameControlBase::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize != sizeof(CMD_S_AddScore)) return false;
	CMD_S_AddScore * pAddScore = (CMD_S_AddScore *)pBuffer;
	//CCLog("用户下注 %d  %lld",pAddScore->wAddScoreUser,pAddScore->lAddScoreCount);
	hideActionPrompt();
	/*
	//变量定义
	WORD wMeChairID=GetMeChairID();
	WORD wAddScoreUser=pAddScore->wAddScoreUser;
	WORD wViewChairID=m_wViewChairID[wAddScoreUser];

	//加注处理
	//if ((IsLookonMode()==true)||(pAddScore->wAddScoreUser!=wMeChairID))
	{
	//下注财富
	m_lTableScore[pAddScore->wAddScoreUser]=pAddScore->lAddScoreCount;

	//加注界面
	m_GameClientView.SetUserTableScore(wViewChairID,pAddScore->lAddScoreCount);

	//播放声音
	if (m_cbPlayStatus[wAddScoreUser]==TRUE)
	{
	PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
	}
	}

	//设置筹码
	//m_GameClientView.SetUserTableScore(wViewChairID,m_lTableScore[wAddScoreUser]);
	*/
	return true;
}

//发牌消息
bool GameControlBase::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize != sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * pSendCard = (CMD_S_SendCard *)pBuffer;
	CCLog("发牌");

	for (int i = 0; i < MAX_COUNT; i++)
	{
		DataModel::sharedDataModel()->card[getMeChairID()][i] = pSendCard->cbCardData[getMeChairID()][i];
	}
	getMainScene()->setServerStateWithUpdate(MainSceneOxTwo::STATE_SEND_CARD);
	/*
	//删除定时器
	KillGameClock(IDI_NULLITY);

	m_GameClientView.SetWaitInvest(false);

	CopyMemory(m_cbHandCardData,pSendCard->cbCardData,sizeof(m_cbHandCardData));

	WORD wMeChiarID=GetMeChairID();
	WORD wViewChairID=m_wViewChairID[wMeChiarID];
	if(IsAllowLookon() || !IsLookonMode())
	m_GameClientView.m_CardControl[wViewChairID].SetDisplayFlag(true);
	else
	m_GameClientView.m_CardControl[wViewChairID].SetDisplayFlag(false);

	//{修改发牌方式 2010-01-13
	for(WORD i=0;i<MAX_COUNT;i++)
	{
	for (WORD j=m_wBankerUser;j<m_wBankerUser+GAME_PLAYER;j++)
	{
	WORD w=j%GAME_PLAYER;
	if (m_cbPlayStatus[w]==TRUE)
	{
	WORD wViewChairID=m_wViewChairID[w];
	if(pSendCard->cbCardData[w][i]!=0&&w==GetMeChairID())
	{
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetCardData(cbCardData,CountArray(cbCardData));

	cbCardData[cbCardCount++]=m_cbHandCardData[w][i];
	m_GameClientView.m_CardControl[wViewChairID].SetCardData(cbCardData,cbCardCount);
	}else{
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetCardData(cbCardData,CountArray(cbCardData));

	cbCardData[cbCardCount++]=m_cbHandCardData[w%GAME_PLAYER][i];
	m_GameClientView.m_CardControl[wViewChairID].SetCardData(cbCardData,cbCardCount);
	}
	}
	}
	}
	OnSendCardFinish(0,0);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
	//修改结束}
	*/

	return true;
}

//用户摊牌
bool GameControlBase::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize != sizeof(CMD_S_Open_Card)) return false;
	CMD_S_Open_Card * pOpenCard = (CMD_S_Open_Card *)pBuffer;
	CCLog("openCardID:%d  bOpen:%d", pOpenCard->wPlayerID, pOpenCard->bOpen);

	//hideActionPrompt();
	//游戏信息
	/*WORD wID=pOpenCard->wPlayerID;
	//ASSERT(m_cbPlayStatus[wID]==TRUE);

	//摊牌标志
	if(wID!=GetMeChairID() || IsLookonMode())
	{
	WORD wViewChairID=m_wViewChairID[wID];
	m_GameClientView.ShowOpenCard(wViewChairID);
	//保存牛信息
	m_bUserOxCard[wID]=pOpenCard->bOpen;
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OPEN_CARD"));
	}
	*/
	return true;
}

//用户强退
bool GameControlBase::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize != sizeof(CMD_S_PlayerExit)) return false;
	CMD_S_PlayerExit * pPlayerExit = (CMD_S_PlayerExit *)pBuffer;
	CCLog("用户强退<<%s>>", __FUNCTION__);
	/*
	WORD wID=pPlayerExit->wPlayerID;

	//游戏信息
	ASSERT(m_cbPlayStatus[wID]==TRUE);
	m_cbPlayStatus[wID]=FALSE;

	//用户名字
	for(WORD i=0;i<32;i++)m_szNickName[wID][i]=0;
	m_szNickName[wID][0] = '#';

	BYTE bCount=0;
	for(WORD i=0;i<GAME_PLAYER;i++)if(m_cbPlayStatus[i]!=FALSE)bCount++;
	if(bCount==1)m_GameClientView.SetBankerUser(INVALID_CHAIR);
	*/
	return true;

}

//游戏结束
bool GameControlBase::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	if (wDataSize != sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd = (CMD_S_GameEnd *)pBuffer;

	hideActionPrompt();

	//显示积分
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		//CCLog("%d  %lld",i,pGameEnd->lGameScore[i]);
		//if(GetTableUserItem(i)!=NULL)m_GameClientView.ShowScore(m_wViewChairID[i],true);
		//m_GameClientView.SetUserTableScore(m_wViewChairID[i],pGameEnd->lGameScore[i]);
		//m_GameClientView.SetUserTotalScore(m_wViewChairID[i],pGameEnd->lGameScore[i]);
		long long lGameScore = pGameEnd->lGameScore[i];
		if (i == getMeChairID()){
			getMainScene()->playerLayer->showResultAnimation(3, lGameScore);
			if (lGameScore != 0){
				UIPanel *pPlayer0 = getMainScene()->playerLayer->pPlayerData[0]->pPlayerPanel;
				UIPanel *pPlayer3 = getMainScene()->playerLayer->pPlayerData[3]->pPlayerPanel;
				for (int i = 0; i < 60; i++)
				{
					if (lGameScore > 0)
					{
						CCPoint begingPos = ccpAdd(pPlayer0->getPosition(), ccp(pPlayer0->getContentSize().width / 2, pPlayer0->getContentSize().height / 2));
						CCPoint endPos = ccpAdd(pPlayer3->getPosition(), ccp(pPlayer3->getContentSize().width / 2, pPlayer3->getContentSize().height / 2));

						goldJump(i, begingPos, endPos);
					}
					else
					{
						CCPoint endPos = ccpAdd(pPlayer0->getPosition(), ccp(pPlayer0->getContentSize().width / 2, pPlayer0->getContentSize().height / 2));
						CCPoint begingPos = ccpAdd(pPlayer3->getPosition(), ccp(pPlayer3->getContentSize().width / 2, pPlayer3->getContentSize().height / 2));

						goldJump(i, begingPos, endPos);
					}
				}
			}

			/*if (lGameScore>0)
			{
			for (int i = 0; i < 10; i++)
			{
			CCSprite *gold=CCSprite::create("res/g_gold.png");
			gold->runAction(CCJumpTo::create(0.1,ccp(20,30),4,1));
			}
			}else
			{
			CCSprite *gold=CCSprite::create("res/g_gold.png");
			gold->runAction(CCJumpTo::create(0.1,ccp(20,30),4,1));
			}*/
		}
		else
		{
			getMainScene()->playerLayer->showResultAnimation(0, lGameScore);
		}


	}
	//显示牌型
	bool bOxSound = false;
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		//WORD wViewChairID=m_wViewChairID[i];
		if (i == getMeChairID() && !IsLookonMode())
			continue;
		bool isCardError = false;
		for (int j = 0; j < MAX_COUNT; j++)
		{
			DataModel::sharedDataModel()->card[i][j] = pGameEnd->cbCardData[i][j];
			if (pGameEnd->cbCardData[i][j] == 0)
			{
				isCardError = true;
				break;
			}
		}
		if (!isCardError)
		{
			getMainScene()->cardLayer->showCard(true,getChairIndex(DataModel::sharedDataModel()->userInfo->wChairID, i), i);
			getMainScene()->cardLayer->sortingOx(i, getChairIndex(DataModel::sharedDataModel()->userInfo->wChairID, i));
		}
	}


	pEndLayer = GameEndLayer::create();
	this->addChild(pEndLayer);
	pEndLayer->showEnd(pGameEnd->lGameScore[getMeChairID()] >= 0);
	getMainScene()->setGameStateWithUpdate(MainSceneOxTwo::STATE_END);
	/*
	CopyMemory(m_cbHandCardData,pGameEnd->cbCardData,sizeof(m_cbHandCardData));

	for (int i=0;i<GAME_PLAYER;i++)
	{
	if (i!=GetMeChairID())
	{
	WORD wViewchair=m_wViewChairID[i];
	m_GameClientView.m_CardControl[wViewchair].SetCardData(NULL,0);
	}
	}

	for(WORD i=0;i<MAX_COUNT;i++)
	{
	for (WORD j=m_wBankerUser;j<m_wBankerUser+GAME_PLAYER;j++)
	{
	WORD w=j%GAME_PLAYER;
	if (m_cbPlayStatus[w]==TRUE)
	{
	WORD wViewChairID=m_wViewChairID[w];
	if(pGameEnd->cbCardData[w][i]!=0&&w!=GetMeChairID())
	{
	BYTE cbCardData[MAX_COUNT];
	//BYTE cbChangeCardData[MAX_C_COUNT];
	BYTE cbCardCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetCardData(cbCardData,CountArray(cbCardData));

	cbCardData[cbCardCount++]=m_cbHandCardData[w][i];
	m_GameClientView.m_CardControl[wViewChairID].SetCardData(cbCardData,cbCardCount);
	}
	}
	}
	}

	//删除定时器
	KillGameClock(IDI_NULLITY);
	KillGameClock(IDI_CALL_BANKER);
	KillTimer(IDI_TIME_USER_ADD_SCORE);
	KillTimer(IDI_TIME_OPEN_CARD);

	//清理数据
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
	m_GameClientView.ShowOpenCard(i,FALSE);
	}
	m_GameClientView.SetWaitInvest(false);
	UpdateScoreControl(NULL,SW_HIDE);
	//m_GameClientView.m_btOx.ShowWindow(SW_HIDE);
	m_GameClientView.m_btHintOx.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);
	//m_GameClientView.m_btReSort.ShowWindow(SW_HIDE);
	m_GameClientView.m_btShortcut.ShowWindow(SW_HIDE);
	m_GameClientView.m_btBanker.ShowWindow(SW_HIDE);
	m_GameClientView.m_btIdler.ShowWindow(SW_HIDE);
	m_GameClientView.SetWaitCall(0xff);
	m_GameClientView.m_bOpenCard=false;

	//状态设置
	SetGameStatus(GAME_STATUS_FREE);

	//播放声音
	if (IsLookonMode()==false)
	{
	if (pGameEnd->lGameScore[GetMeChairID()]>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	}
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//显示积分
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
	if(GetTableUserItem(i)!=NULL)m_GameClientView.ShowScore(m_wViewChairID[i],true);
	m_GameClientView.SetUserTableScore(m_wViewChairID[i],pGameEnd->lGameScore[i]);
	m_GameClientView.SetUserTotalScore(m_wViewChairID[i],pGameEnd->lGameScore[i]);
	}
	m_GameClientView.m_ScoreView.ResetScore();
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
	//设置信息
	if (pGameEnd->lGameScore[i]!=0L || i==m_wBankerUser)
	{
	m_GameClientView.m_ScoreView.SetGameTax(pGameEnd->lGameTax[i],i);
	if (m_szNickName[i][0]=='#') m_GameClientView.m_ScoreView.SetGameScore(i,TEXT("已离开"),pGameEnd->lGameScore[i]);
	else m_GameClientView.m_ScoreView.SetGameScore(i,m_szNickName[i],pGameEnd->lGameScore[i]);
	}
	}
	//m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//显示牌型
	bool bOxSound=false;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
	WORD wViewChairID=m_wViewChairID[i];
	if(i==GetMeChairID() && !IsLookonMode())
	continue;

	m_GameClientView.m_CardControl[wViewChairID].SetDisplayFlag(true);

	//牛牛牌型
	if(m_cbHandCardData[i][0]>0)
	{
	if(m_bUserOxCard[i]==TRUE)
	{
	//扑克数据
	BYTE bCardData[MAX_COUNT];
	CopyMemory(bCardData,m_cbHandCardData[i],sizeof(bCardData));

	//获取牛牛数据
	bool bOx=m_GameLogic.GetOxCard(bCardData,MAX_COUNT);
	ASSERT(bOx);

	//加载数据
	m_GameClientView.m_CardControl[wViewChairID].SetCardData(bCardData,3);
	m_GameClientView.m_CardControlOx[wViewChairID].SetCardData(&bCardData[3],2);

	//显示点数
	BYTE bValue=m_GameLogic.GetCardLogicValue(bCardData[3])+m_GameLogic.GetCardLogicValue(bCardData[4]);
	if(bValue>10)bValue-=10;
	ASSERT(bValue>0);
	if(bValue>=10)bOxSound=true;
	m_GameClientView.SetUserOxValue(wViewChairID,bValue);
	}
	else
	{
	//无牛
	m_GameClientView.SetUserOxValue(wViewChairID,0);
	}
	}
	}
	m_GameClientView.ViewDisplayType(true);

	//牛牛声音
	if(bOxSound)
	{
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_OXOX"));
	}

	//开始按钮
	if (IsLookonMode()==false)
	{
	m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
	SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_USER_START_GAME);
	}

	//成绩显示在即时聊天对话框
	TCHAR szBuffer[512]=TEXT("");
	myprintf(szBuffer,CountArray(szBuffer),TEXT("本次对局结果:"));
	m_pIStringMessage->InsertNormalString(szBuffer);
	myprintf(szBuffer,CountArray(szBuffer),TEXT("用户昵称\t成绩"));
	m_pIStringMessage->InsertNormalString(szBuffer);
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
	//变量定义
	IClientUserItem * pUserData=GetTableUserItem(i);
	//成绩输出
	if (pUserData!=NULL)
	{
	if(mystrlen(pUserData->GetNickName())<8)
	myprintf(szBuffer,CountArray(szBuffer),TEXT("%s:\t\t%+d"),pUserData->GetNickName(),pGameEnd->lGameScore[i]);
	else
	myprintf(szBuffer,CountArray(szBuffer),TEXT("%s:\t%+d"),pUserData->GetNickName(),pGameEnd->lGameScore[i]);
	m_pIStringMessage->InsertNormalString(szBuffer);
	}
	else if(m_szNickName[i][0]=='#')
	{
	myprintf(szBuffer,CountArray(szBuffer),TEXT("%s:%+d"),TEXT("用户离开"),pGameEnd->lGameScore[i]);
	m_pIStringMessage->InsertNormalString(szBuffer);
	}
	}
	myprintf(szBuffer,CountArray(szBuffer),TEXT("本局游戏结束。\n--------------------"));
	m_pIStringMessage->InsertNormalString(szBuffer);

	//状态变量
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_szNickName,sizeof(m_szNickName));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	*/
	return true;
}

void GameControlBase::OnUserFree(CCObject *obj){
	//CCMessageBox(Tools::GBKToUTF8("长时间不操作，自动退出！"), Tools::GBKToUTF8("提示"));
	TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_LOGON_ROOM);
	Tools::setTransitionAnimation(0, 0, GameLobbyScene::scene(!isExitGame));
	CCLog("退出 ");
}
//用户进入
void GameControlBase::onUserEnter(){
	std::map<long, tagUserInfo>::iterator iter;
	for (iter = DataModel::sharedDataModel()->mTagUserInfo.begin(); iter != DataModel::sharedDataModel()->mTagUserInfo.end(); iter++)
	{
		if (iter->second.dwUserID != DataModel::sharedDataModel()->userInfo->dwUserID)
		{
			CCLog("ID:%ld otherID:%ld   name:%s<<%s>>", iter->second.dwUserID, DataModel::sharedDataModel()->userInfo->dwUserID,
				Tools::GBKToUTF8(DataModel::sharedDataModel()->userInfo->szNickName), __FUNCTION__);
			getMainScene()->playerLayer->setUserInfo(0, iter->second);
			//DataModel::sharedDataModel()->getMainSceneOxOneByOne()->playerLayer->setUserInfo(0,iter->second);
		}
		else
		{
			getMainScene()->playerLayer->setUserInfo(3, iter->second);
			//DataModel::sharedDataModel()->getMainSceneOxOneByOne()->playerLayer->setUserInfo(3, iter->second);
		}
		//CCLog("--mm----------------nick:%s",Tools::GBKToUTF8(iter->second.szNickName));
	}
	DataModel::sharedDataModel()->mTagUserInfo.clear();
}
void GameControlBase::OnEventUserState(WORD wSubCmdID, const void * pBuffer, WORD wDataSize){

}
void GameControlBase::onSubUserState(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GR_USER_STATUS:
	{
		CMD_GR_UserStatus *info = (CMD_GR_UserStatus*)pDataBuffer;
		switch (info->UserStatus.cbUserStatus)
		{
		case US_SIT://坐下
		{
			std::map<long, tagUserInfo>::iterator iterUser = DataModel::sharedDataModel()->mTagUserInfo.find(info->dwUserID);
			if (iterUser != DataModel::sharedDataModel()->mTagUserInfo.end())
			{
				iterUser->second.wChairID = info->UserStatus.wChairID;
				iterUser->second.wTableID = info->UserStatus.wTableID;
				onUserEnter();
			}
			//CCLog("state==sit-----------%ld", info->dwUserID);
			if (info->dwUserID == DataModel::sharedDataModel()->userInfo->dwUserID){
				//DataModel::sharedDataModel()->isSit=true;
				//CCLog("======================坐下:table: %d desk:%d", info->UserStatus.wTableID, info->UserStatus.wChairID);
				DataModel::sharedDataModel()->userInfo->wTableID = info->UserStatus.wTableID;
				DataModel::sharedDataModel()->userInfo->wChairID = info->UserStatus.wChairID;
				if (getMainScene()->getGameState() == MainSceneBase::STATE_OBSERVER)
				{
					getMainScene()->setGameStateWithUpdate(MainSceneBase::STATE_DOWN);
					//构造数据
					CMD_GF_GameOption GameOption;
					GameOption.dwFrameVersion = VERSION_FRAME;
					GameOption.cbAllowLookon = 0;
					GameOption.dwClientVersion = VERSION_CLIENT;
					//发送
					bool isSend =getSocket()->SendData(MDM_GF_FRAME, SUB_GF_GAME_OPTION, &GameOption, sizeof(GameOption));
					if (isSend)
					{
					}
				}
			}
			else
			{

			}
		}
		break;
		case US_FREE://站立
		{
			//
			//onUserEnter();

			if (info->dwUserID == DataModel::sharedDataModel()->userInfo->dwUserID)
			{
				OnUserFree(NULL);
			}
			else
			{
				hidePlayer(info);
			}
		}
		break;
		case US_READY://同意
		{
			onUserReady(info);
		}
		break;
		case US_PLAYING:
		{
			if (info->dwUserID == DataModel::sharedDataModel()->userInfo->dwUserID)
			{

				//MTNotificationQueue::sharedNotificationQueue()->postNotification(S_L_CONFIG_FINISH,NULL);
			}
			//CCLog("state==playing-----------%ld",info->dwUserID);
		}
		break;
		default:
			CCLog("state==Other userID:%ld 状态：%d", info->dwUserID, info->UserStatus.cbUserStatus);
			break;
		}
	}
	break;
	case SUB_GR_USER_ENTER://用户进入
	{
		onSubUserEnter(pDataBuffer, wDataSize);
		onUserEnter();
		//MTNotificationQueue::sharedNotificationQueue()->postNotification(S_L_US_ENTER, NULL);
	}
	break;
	case SUB_GR_USER_SCORE://用户分数
		break;
	default:
		CCLog("-------------------subCmdID: %d<<%s>>", wSubCmdID, __FUNCTION__);
		break;
	}

}
//用户准备
void GameControlBase::onUserReady(CMD_GR_UserStatus *info){
	if (info->dwUserID == DataModel::sharedDataModel()->userInfo->dwUserID)
	{
		getMainScene()->playerLayer->pPlayerData[3]->showActionType(PlayerData::ACTION_READY);
	}
	else
	{
		getMainScene()->playerLayer->pPlayerData[0]->showActionType(PlayerData::ACTION_READY);
	}
	//CCLog("state==ready-----------%ld",info->dwUserID);
}
//用户进入
void GameControlBase::onSubUserEnter(void * pDataBuffer, unsigned short wDataSize){
#if (DEBUG_TEST==0)
	//效验参数
	assert(wDataSize >= sizeof(tagUserInfoHead));
	if (wDataSize < sizeof(tagUserInfoHead)) return false;
	//消息处理
	tagUserInfoHead * pUserInfoHead = (tagUserInfoHead *)pDataBuffer;

	/*if (pUserInfoHead->wTableID==DataModel::sharedDataModel()->userInfo->wTableID)
	{
	}*/
	int wPacketSize = 0;
	//变量定义
	tagUserInfo UserInfo;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET + sizeof(TCP_Head)];
	CopyMemory(cbDataBuffer, pDataBuffer, wDataSize);
	//CCLog("--------------------------");
	wPacketSize += sizeof(tagUserInfoHead);
	while (true)
	{
		void * pDataBuffer1 = cbDataBuffer + wPacketSize;
		tagDataDescribe *DataDescribe = (tagDataDescribe*)pDataBuffer1;
		wPacketSize += sizeof(tagDataDescribe);
		switch (DataDescribe->wDataDescribe)
		{
		case DTP_GR_NICK_NAME:		//用户昵称
		{
			CopyMemory(&UserInfo.szNickName, cbDataBuffer + wPacketSize, DataDescribe->wDataSize);
			UserInfo.szNickName[CountArray(UserInfo.szNickName) - 1] = 0;
			//std::string name=UserInfo.szNickName;
			//CCLog("gameId:%ld nick:%s",pUserInfoHead->dwGameID,Tools::GBKToUTF8(UserInfo.szNickName));
			//if (strcmp(Tools::GBKToUTF8(UserInfo.szNickName),"(null)")==0)
			{
				//CCLog("null"); 
			}
		}
		break;
		case DTP_GR_GROUP_NAME:
		{
			CCLog("社团");
		}
		break;
		case DTP_GR_UNDER_WRITE:
		{
			CopyMemory(UserInfo.szUnderWrite, cbDataBuffer + wPacketSize, DataDescribe->wDataSize);
			UserInfo.szUnderWrite[CountArray(UserInfo.szUnderWrite) - 1] = 0;
			CCLog("签名:%s", Tools::GBKToUTF8(UserInfo.szUnderWrite));
		}
		break;
		}
		wPacketSize += DataDescribe->wDataSize;
		if (wPacketSize >= wDataSize)
		{
			break;
		}
	}
#endif
#if (DEBUG_TEST==1)
	//效验参数
	assert(wDataSize >= sizeof(tagMobileUserInfoHead));
	if (wDataSize < sizeof(tagMobileUserInfoHead)) return;
	int size = sizeof(tagMobileUserInfoHead);
	//消息处理
	tagMobileUserInfoHead * pUserInfoHead = (tagMobileUserInfoHead *)pDataBuffer;
	int wPacketSize = 0;
	//变量定义
	tagUserInfo UserInfo;
	UserInfo.dwUserID = pUserInfoHead->dwUserID;
	UserInfo.lScore = pUserInfoHead->lScore;
	UserInfo.wChairID = pUserInfoHead->wChairID;
	UserInfo.wTableID = pUserInfoHead->wTableID;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET + sizeof(TCP_Head)];
	CopyMemory(cbDataBuffer, pDataBuffer, wDataSize);
	//CCLog("-------------------------%d",wDataSize);
	//CCLog("userID:%ld , state:%d <<%s>>",pUserInfoHead->dwUserID,pUserInfoHead->cbUserStatus,__FUNCTION__);
	wPacketSize += sizeof(tagMobileUserInfoHead);
	while (true)
	{
		void * pDataBuffer1 = cbDataBuffer + wPacketSize;
		tagDataDescribe *DataDescribe = (tagDataDescribe*)pDataBuffer1;
		wPacketSize += sizeof(tagDataDescribe);
		switch (DataDescribe->wDataDescribe)
		{
		case DTP_GR_NICK_NAME:		//用户昵称
		{
			CopyMemory(&UserInfo.szNickName, cbDataBuffer + wPacketSize, DataDescribe->wDataSize);
			UserInfo.szNickName[CountArray(UserInfo.szNickName) - 1] = 0;
		}
		break;
		case DTP_GR_GROUP_NAME:
		{
			CCLog("社团");
		}
		break;
		case DTP_GR_UNDER_WRITE:
		{
			CopyMemory(UserInfo.szUnderWrite, cbDataBuffer + wPacketSize, DataDescribe->wDataSize);
			UserInfo.szUnderWrite[CountArray(UserInfo.szUnderWrite) - 1] = 0;
			CCLog("签名:%s", Tools::GBKToUTF8(UserInfo.szUnderWrite));
		}
		break;
		}
		wPacketSize += DataDescribe->wDataSize;
		if (wPacketSize >= wDataSize)
		{
			break;
		}
	}

	//onUserEnterWithUpdate(&UserInfo);

	std::map<long, tagUserInfo >::iterator l_it;
	l_it = DataModel::sharedDataModel()->mTagUserInfo.find(pUserInfoHead->dwUserID);
	if (DataModel::sharedDataModel()->mTagUserInfo.end() != l_it)
	{
		l_it->second = UserInfo;
	}
	else
	{
		DataModel::sharedDataModel()->mTagUserInfo.insert(std::map<long, tagUserInfo>::value_type(pUserInfoHead->dwUserID, UserInfo));
	}
#endif
}

void GameControlBase::goldJump(int index, CCPoint beginPos, CCPoint endPos){
	beginPos = ccpAdd(beginPos, ccp(rand() % 140, rand() % 80));

	endPos = ccpAdd(endPos, ccp(rand() % 30, rand() % 30));

	//float fDistance=ccpDistance(beginPos,endPos);
	//float fTime=fDistance/300;
	float fTime = 0.5 + 0.1*(rand() % 5);
	float fHeight = 100 + rand() % 30;
	CCSprite *gold = CCSprite::create("res/g_gold.png");
	this->addChild(gold, 200);
	gold->setPosition(beginPos);
	gold->setScale(0.6);
	//CCEaseOut *eOut=CCEaseOut::create(CCMoveTo::create(fTime,endPos),0.1);
	CCSpawn *spa = CCSpawn::create(
		//CCJumpTo::create(fTime,endPos,fHeight,1),
		CCMoveTo::create(fTime, endPos),
		//CCScaleTo::create(fTime,1.4),
		NULL);
	gold->runAction(CCSequence::create(
		spa,
		CCCallFuncN::create(this, SEL_CallFuncN(&GameControlBase::onGoldJump))
		, NULL));
}
void GameControlBase::onGoldJump(CCNode *node){
	node->removeFromParentAndCleanup(true);
}

/*
*/
MainSceneBase*GameControlBase::getMainScene(){
	return (MainSceneBase*)this->getParent();
}
//站立并退出
void GameControlBase::standUpWithExit(){
	isExitGame = true;
	//tagUserInfo userInfo=DataModel::sharedDataModel()->mTagUserInfo.find(DataModel::sharedDataModel()->userInfo.dwUserID);
	CMD_GR_UserStandUp  userStandUp;
	userStandUp.wTableID = DataModel::sharedDataModel()->userInfo->wTableID;
	userStandUp.wChairID = DataModel::sharedDataModel()->userInfo->wChairID;
	userStandUp.cbForceLeave = true;
	if (userStandUp.wChairID == -10 || userStandUp.wChairID > MAX_CHAIR)
	{
		TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_LOGON_ROOM);
		Tools::setTransitionAnimation(0, 0, GameLobbyScene::scene(false));
	}
	else
	{
		getMainScene()->addLoadingLayer(SOCKET_LOGON_ROOM);
		CCLog("-------userStandUp.wChairID :%d<<%s>>", userStandUp.wChairID, __FUNCTION__);
		//发送消息
		TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_LOGON_ROOM)->SendData(MDM_GR_USER, SUB_GR_USER_STANDUP, &userStandUp, sizeof(userStandUp));
	}
}