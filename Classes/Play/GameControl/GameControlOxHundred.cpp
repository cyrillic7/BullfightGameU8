//
//
//
//

#include "GameControlOxHundred.h"
#include "../../Tools/GameConfig.h"
#include "../../Tools/Tools.h"
#include "../../Tools/SoundConfig.h"
#include "../../Tools/BaseAttributes.h"
#include "../../GameLobby/GameLobbyScene.h"
#include "../../Tools/DataModel.h"
#include "../../Network/CMD_Server/cmd_game.h"
#include "../../Network/CMD_Server/PacketAide.h"
#include "../../Network/CMD_Server/CMD_Commom.h"
#include "../../PopDialogBox/PopDialogBoxUpBank.h"
#include "../../PopDialogBox/PopDialogBoxOnLine.h"
#include "../../PopDialogBox/PopDialogBoxTrend.h"
#include "../../PopDialogBox/PopDialogBoxLoading.h"
#include "../../PopDialogBox/PopDialogBoxMakeText.h"
#include "../../Network/SEvent.h"
#include "../../MTNotificationQueue/MTNotificationQueue.h"
#include "../../PopDialogBox/PopDialogBoxRecharge.h"
#include "../../Network/CMD_Server/Packet.h"
using namespace std;
GameControlOxHundred::GameControlOxHundred()
	:iCurSelectJettonIndex(0)
	, m_lMeMaxScore(0)
	, m_bMeApplyBanker(0)
	, m_wBankerUser(-100)
	, isChangeUpBank(true)
	, lUserScore(0)
	, lBankerScore(0)
{
	nAllJetton[0] = 10;
	nAllJetton[1] = 50;
	nAllJetton[2] = 100;
	nAllJetton[3] = 1000;
	nAllJetton[4] = 5000;
	nAllJetton[5] = 10000;
	nAllJetton[6] = 100000;
	nAllJetton[7] = 500000;

	/*nJetton[0] = 1000;
	nJetton[1] = 5000;
	nJetton[2] = 10000;
	nJetton[3] = 100000;
	nJetton[4] = 500000;*/
	//庄家信息
	m_wBankerUser = INVALID_CHAIR;
	isPalySoundWarn = false;
	resetData();
	DataModel::sharedDataModel()->userInfo->wChairID = -10;
}
GameControlOxHundred::~GameControlOxHundred(){
	
	DataModel::sharedDataModel()->mTagUserInfo.clear();
	DataModel::sharedDataModel()->vecJettonNode.clear();
}
void GameControlOxHundred::onEnter(){
	CCLayer::onEnter();
	pWidget = UILayer::create();
	this->addChild(pWidget);

	UILayout *pLayout = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIGameIngHundred.ExportJson)));
	pWidget->addWidget(pLayout);

	UIButton* button = static_cast<UIButton*>(pWidget->getWidgetByName("ButtonBack"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControlOxHundred::onMenuBack));
	//充值
	button = static_cast<UIButton*>(pWidget->getWidgetByName("ButtonAdd"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControlOxHundred::onMenuOnRecharge));
	//趋势图按键
	button = static_cast<UIButton*>(pWidget->getWidgetByName("ButtonTrend"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControlOxHundred::onMenuTrend));
	//在线用户按键
	pBOnline = static_cast<UIButton*>(pWidget->getWidgetByName("ButtonOnline"));
	pBOnline->addTouchEventListener(this, SEL_TouchEvent(&GameControlOxHundred::onMenuOnLine));
	posPlayerJetton = pBOnline->getPosition();
	//筹码按钮
	for (int i = 0; i < MAX_JETTON_BUTTON_COUNT; i++)
	{
		pIJettonButton[i] = static_cast<UIImageView*>(pWidget->getWidgetByName(CCString::createWithFormat("ImageJetton%d", i)->getCString()));
		pIJettonButton[i]->addTouchEventListener(this, SEL_TouchEvent(&GameControlOxHundred::onMenuSelectJetton));
		//隐藏
		pIJettonButton[i]->setColor(ccc3(100, 100, 100));
		pIJettonButton[i]->setTouchEnabled(false);
		pIJettonButton[i]->setVisible(false);
	}
	//筹码选择光标
	pIJettonSelect = static_cast<UIImageView*>(pWidget->getWidgetByName("ImageSelectJetton"));
	pIJettonSelect->getChildByName("ImageSelectJettonBg")->runAction(CCRepeatForever::create(CCRotateBy::create(0.8, 360)));
	pIJettonSelect->setVisible(false);

	//庄家牌背景
	UIImageView *pIBankCardBg = static_cast<UIImageView*>(pWidget->getWidgetByName("ImageBankCardBg"));
	getMainScene()->posChair[0] = ccpAdd(pIBankCardBg->getPosition(), ccp(0, -pIBankCardBg->getContentSize().height / 2));
	//庄家头像
	UIImageView * pBankIcon= static_cast<UIImageView*>(pWidget->getWidgetByName("ImageIcon1"));
	posBankJetton = pBankIcon->getWorldPosition();
	//用户数据
	for (int i = 0; i < MAX_PLAYER_HUNDRED_COUNT; i++)
	{
		pPlayerData[i] = PlayerDataHundred::create();
		this->addChild(pPlayerData[i]);
		pPlayerData[i]->pIPlayerBg = static_cast<UIImageView*>(pWidget->getWidgetByName(CCString::createWithFormat("ImageIcon%d", i)->getCString()));
		pPlayerData[i]->pIPlayerBg->setVisible(false);
		//结算数字
		pPlayerData[i]->pLResult = static_cast<UILabelAtlas*>(pPlayerData[i]->pIPlayerBg->getChildByName("AtlasLabelResult"));
		pPlayerData[i]->pLResult->setVisible(false);
		//玩家名称
		pPlayerData[i]->pLUserName = static_cast<UILabel*>(pPlayerData[i]->pIPlayerBg->getChildByName("LabelName"));
		//玩家金币
		pPlayerData[i]->pLGoldCount = static_cast<UILabel*>(pPlayerData[i]->pIPlayerBg->getChildByName("LabelGold"));
	}
	pPlayerData[0]->setUserInfo(*DataModel::sharedDataModel()->userInfo);
	//上庄按键
	pIUpBank = static_cast<UIImageView*>(pWidget->getWidgetByName("ImageUpBank"));
	pIUpBank->addTouchEventListener(this, SEL_TouchEvent(&GameControlOxHundred::onMenuUpBank));
	pIUpBank->setEnabled(false);
	//我要上庄动画
	CCSprite *pSUpBank = CCSprite::create();
	CCArray *animFrames = CCArray::create();

	CCSpriteFrame *pSFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("u_gih_add_score.png");
	animFrames->addObject(pSFrame);
	CCSpriteFrame *pSFrame1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("u_gih_add_score_pre.png");
	animFrames->addObject(pSFrame1);

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.1f);
	animation->setLoops(-1);
	CCAnimate *action = CCAnimate::create(animation);
	//pSUpBank->runAction(action);

	pIUpBank->addNode(pSUpBank);

	initTimer(pWidget);
	initSeatData(pWidget);
	initUserChair(pWidget);
}
void GameControlOxHundred::onExit(){
	CCLayer::onExit();
}
void GameControlOxHundred::resetData(){
	//个人下注
	memset(m_lUserJettonScore, 0, sizeof(m_lUserJettonScore));
	//全体下注
	memset(m_lAllJettonScore, 0, sizeof(m_lAllJettonScore));

	//m_wBankerTime=0;
	m_lBankerScore = 0L;
	//m_lBankerWinScore=0L;
	//m_lTmpBankerWinScore=0;
	//m_blCanStore=false;
	/*
	//当局成绩
	m_lMeCurGameScore=0L;
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;

	//状态信息
	m_lCurrentJetton=0L;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;
	m_bWinTianMen=false;
	m_bWinHuangMen=false;
	m_bWinXuanMen=false;
	m_bFlashResult=false;
	m_blMoveFinish = false;
	m_blAutoOpenCard = true;
	m_enDispatchCardTip=enDispatchCardTip_NULL;

	m_lMeCurGameScore=0L;
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	*/
	//m_lAreaLimitScore=0L;	
	/*
	//位置信息
	m_nScoreHead = 0;
	m_nRecordFirst= 0;
	m_nRecordLast= 0;

	//历史成绩
	m_lMeStatisticScore=0;
	*/
}

//初始化计时器
void GameControlOxHundred::initTimer(UILayer *pWidget){
	pITimer = static_cast<UIImageView*>(pWidget->getWidgetByName("ImageTimer"));
	pITimer->setVisible(false);

	pLTimerNum = static_cast<UILabelAtlas*>(pWidget->getWidgetByName("AtlasLabelTimer"));
	iTimerCount = -1;

	pLTimerPromptContent = static_cast<UILabel*>(pWidget->getWidgetByName("LabelPromptContent"));
}
//初始化座位
void GameControlOxHundred::initSeatData(UILayer *pWidget){
	for (int i = 0; i < MAX_SEAT_COUNT; i++)
	{
		pSeatData[i] = SeatData::create();
		this->addChild(pSeatData[i]);
		UIImageView *bg = static_cast<UIImageView*>(pWidget->getWidgetByName(CCString::createWithFormat("ImageSeatBg%d", i)->getCString()));
		bg->addTouchEventListener(this, SEL_TouchEvent(&GameControlOxHundred::onMenuPlaceJetton));
		//设置中心点
		pSeatData[i]->posCenter = bg->getPosition();
		getMainScene()->posChair[i + 1] = ccpAdd(bg->getPosition(), ccp(0, -bg->getContentSize().height / 2 - 50));
		//设置座位大小
		pSeatData[i]->seatSize = bg->getContentSize();
		//所有筹码
		pSeatData[i]->pLAllJetton = static_cast<UILabelAtlas*>(bg->getChildByName("AtlasLabelAllScore"));
		pSeatData[i]->pIFontWan = static_cast<UIImageView*>(bg->getChildByName("AtlasLabelAllScore")->getChildByName("ImageFontWan"));
		//用户筹码
		pSeatData[i]->pLUserJetton = static_cast<UILabelAtlas*>(bg->getChildByName("ImageB")->getChildByName("AtlasLabelAddScore"));
		pSeatData[i]->pIUserFontWan = static_cast<UIImageView*>(bg->getChildByName("ImageB")->getChildByName("AtlasLabelAddScore")->getChildByName("ImageFontWan1"));
		//未下注
		pSeatData[i]->pINotAddJetton = static_cast<UIImageView*>(bg->getChildByName("ImageNotAdd"));
		//结算数字
		pSeatData[i]->pLResult = static_cast<UILabelAtlas*>(bg->getChildByName("AtlasLabelResultNum"));

		pSeatData[i]->resetData();
	}
}
//初始化用户椅子
void GameControlOxHundred::initUserChair(UILayer *pWidget){
	for (int i = 0; i < MAX_USER_CHAIR; i++)
	{
		pChairData[i] = ChairData::create();
		this->addChild(pChairData[i]);

		UIImageView *pIVPlayer = static_cast<UIImageView*>(pWidget->getWidgetByName(CCString::createWithFormat("ImagePlayer%d",i)->getCString()));
		
		pChairData[i]->pBDonw = static_cast<UIButton*>(pIVPlayer->getChildByName("ButtonDown"));
		pChairData[i]->pBDonw->addTouchEventListener(this, SEL_TouchEvent(&GameControlOxHundred::onMenuChangeChair));
		pChairData[i]->pBDonw->setTag(i);
		//用户昵称
		pChairData[i]->pLUserName = static_cast<UILabel*>(pIVPlayer->getChildByName("LabelUserName"));
		//用户头像
		pChairData[i]->pIVUserIcon = static_cast<UIImageView*>(pIVPlayer->getChildByName("ImagePlayerIcon"));
	}
}
//设置座位结算
void GameControlOxHundred::setSeatResult(int iSeatIndex, int iOXType){
	int oxType = iOXType;
	if (oxType == 0)
	{
		oxType == 1;
	}
	long long lMeJetton = m_lUserJettonScore[iSeatIndex] * oxType;
	pSeatData[iSeatIndex - 1]->setResult(lMeJetton);
}
//显示总结果
void GameControlOxHundred::showAllResult(){
	for (int i = 0; i < MAX_PLAYER_HUNDRED_COUNT; i++)
	{
		pPlayerData[i]->showResultAnimation();
	}
	//更新趋势图
	MTNotificationQueue::sharedNotificationQueue()->postNotification(UPDATE_LIST, NULL);
}
//获取用户信息通过椅子号
tagUserInfo* GameControlOxHundred::getUserInfo(int iChair){
	map<long, tagUserInfo>::iterator it;
	for (it = DataModel::sharedDataModel()->mTagUserInfo.begin(); it != DataModel::sharedDataModel()->mTagUserInfo.end(); it++)
	{
		if (it->second.wChairID == iChair)
		{
			return &it->second;
		}
	}
	return NULL;
}
//创建初始筹码
void GameControlOxHundred::createJettonInit(int index,long long num){
	//
	long long llTempJettonScore = num;

	while (llTempJettonScore>0)
	{
		bool isMoneyEnough = false;
		for (int i = MAX_JETTON_BUTTON_COUNT-1; i >= 0; i--)
		{
			if (llTempJettonScore >= nJetton[i])
			{
				llTempJettonScore -= nJetton[i];

				int fWidth = pSeatData[index]->seatSize.width - 31;
				int fHeight = pSeatData[index]->seatSize.height - 70 - 31;

				int offsetX = rand() % fWidth;
				int offsetY = rand() % fHeight;
				CCPoint randPos = ccp(offsetX, offsetY);
				CCPoint pos = pSeatData[index]->posCenter;
				pos = ccpAdd(pos, randPos);
				pos = ccpSub(pos, ccp(fWidth / 2, fHeight / 2));

				JettonNode *pJetton = getJettonNode();
				pJetton->iBetArea = index;
				pJetton->setJettonTypeWithPos(nJetton[i], pos);
				isMoneyEnough = true;
				break;
			}
		}
		if (!isMoneyEnough)
		{
			return;
		}
	}

	
}
//获得筹码对象
JettonNode *GameControlOxHundred::getJettonNode(){
	for (int i = 0; i < DataModel::sharedDataModel()->vecJettonNode.size(); i++)
	{
		JettonNode *tempJetton = DataModel::sharedDataModel()->vecJettonNode[i];
		if (tempJetton->isReuse)
		{
			tempJetton->resetData();
			return tempJetton;
		}
	}
	JettonNode *pJetton = JettonNode::create();
	this->addChild(pJetton, 1000);
	DataModel::sharedDataModel()->vecJettonNode.push_back(pJetton);
	return pJetton;
}
int GameControlOxHundred::getChairIndex(int meChairID, int chairID){
	if (meChairID == 0)
	{
		if (chairID == 0)
		{
			return 3;
		}
	}
	else
	{
		if (chairID == 1)
		{
			return 3;
		}
	}
	return 0;
}
//充值
void GameControlOxHundred::onMenuOnRecharge(CCObject* pSender, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		PopDialogBoxRecharge *pPDBRecharge = PopDialogBoxRecharge::create();
		pPDBRecharge->isShowExchangeGold = false;
		getMainScene()->addChild(pPDBRecharge, K_Z_ORDER_POP);
	}
}
//菜单回调（返回）
void GameControlOxHundred::onMenuBack(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		standUpWithExit();
	}
	break;
	default:
		break;
	}
}
//加注区域
void GameControlOxHundred::onMenuPlaceJetton(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		if (iCurSelectJettonIndex == -1 ||
			DataModel::sharedDataModel()->getMainSceneOxHundred()->getGameState() != MainSceneOxHundred::STATE_GAME_PLACE_JETTON
			|| m_wBankerUser == DataModel::sharedDataModel()->userInfo->wChairID)
		{
			CCLOG("nonononon<<%s>>", __FUNCTION__);
			return;
		}
		UIImageView *pIButton = (UIImageView*)pSender;
		//变量定义
		CMD_C_PlaceJetton PlaceJetton;
		memset(&PlaceJetton, 0, sizeof(PlaceJetton));
		//构造变量
		PlaceJetton.cbJettonArea = pIButton->getTag();
		PlaceJetton.lJettonScore = nJetton[iCurSelectJettonIndex];
		//个人下注
		m_lUserJettonScore[PlaceJetton.cbJettonArea] += PlaceJetton.lJettonScore;
		m_lAllJettonScore[PlaceJetton.cbJettonArea] += PlaceJetton.lJettonScore;

		pSeatData[PlaceJetton.cbJettonArea - 1]->setUserJetton(m_lUserJettonScore[PlaceJetton.cbJettonArea]);


		//发送消息
		//TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_LOGON_ROOM)->SendData(MDM_GF_GAME, SUB_C_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
		GameIngMsgHandler::sharedGameIngMsgHandler()->gameSocket.SendData(MDM_GF_GAME, SUB_C_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
	}
	break;
	default:
		break;
	}
}
//选择筹码
void GameControlOxHundred::onMenuSelectJetton(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		UIImageView *pIButton = (UIImageView*)pSender;
		pIJettonSelect->setPosition(pIButton->getPosition());
		iCurSelectJettonIndex = pIButton->getTag() - 1;
	}
	break;
	default:
		break;
	}
}
//在线用户
void GameControlOxHundred::onMenuOnLine(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		PopDialogBoxOnLine *pDBUpBank = PopDialogBoxOnLine::create();
		getParent()->addChild(pDBUpBank, K_Z_ORDER_POP, TAG_ONLINE);
	}
	break;
	default:
		break;
	}
}
//趋势图
void GameControlOxHundred::onMenuTrend(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		PopDialogBoxTrend *pDBUpBank = PopDialogBoxTrend::create();
		getParent()->addChild(pDBUpBank, K_Z_ORDER_POP);
	}
	break;
	default:
		break;
	}
}
//用户上庄
void GameControlOxHundred::onMenuUpBank(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		PopDialogBoxUpBank *pDBUpBank = PopDialogBoxUpBank::create();
		getParent()->addChild(pDBUpBank, K_Z_ORDER_POP, TAG_UP_BANK);
	}
	break;
	default:
		break;
	}
}
//换椅子
void GameControlOxHundred::onMenuChangeChair(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		UIButton *pBTemp = (UIButton*)pSender;

		CMD_C_UserSit userSit;
		userSit.wUserID = DataModel::sharedDataModel()->userInfo->dwUserID;
		userSit.cbSitID = pBTemp->getTag();
		//发送消息
		GameIngMsgHandler::sharedGameIngMsgHandler()->gameSocket.SendData(MDM_GF_GAME, SUB_C_USER_SIT, &userSit, sizeof(userSit));

	}
		break;
	default:
		break;
	}
}
void GameControlOxHundred::update(float delta){
	MessageQueueGameIng::update(delta);
}
void GameControlOxHundred::delayedAction(){
	switch (DataModel::sharedDataModel()->getMainSceneOxHundred()->getGameState())
	{
	case MainSceneOxHundred::STATE_GAME_FREE:
	{
		hideTimer(true);

	}
	break;
	default:
		break;
	}
}
//最大下注
long long GameControlOxHundred::getUserMaxJetton()
{
	int iTimer = 10;
	//已下注额
	long long lNowJetton = 0;
	assert(AREA_COUNT <= CountArray(m_lUserJettonScore));
	for (int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; ++nAreaIndex)lNowJetton += m_lUserJettonScore[nAreaIndex] * iTimer;

	//庄家金币
	long long lBankerScore = 2147483647;
	//if (m_wBankerUser!=INVALID_CHAIR) 
	lBankerScore = m_lBankerScore;
	//CCLOG("=lBankerScore:%lld    %lld<<%s>>",lBankerScore,m_lBankerScore,__FUNCTION__);
	for (int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; ++nAreaIndex)
	{
		lBankerScore -= m_lAllJettonScore[nAreaIndex] * iTimer;
		//CCLOG("%d ---------%lld          <<%s>>",nAreaIndex,m_lAllJettonScore[nAreaIndex],__FUNCTION__);
	}
	//CCLOG("===lBankerScore:%lld<<%s>>",lBankerScore,__FUNCTION__);
	//区域限制
	long long lMeMaxScore = 0;
	if ((m_lMeMaxScore - lNowJetton) / iTimer > m_lAreaLimitScore)
	{
		lMeMaxScore = m_lAreaLimitScore*iTimer;

	}
	else
	{
		lMeMaxScore = m_lMeMaxScore - lNowJetton;
		lMeMaxScore = lMeMaxScore;
	}

	//庄家限制
	lMeMaxScore = MIN(lMeMaxScore, lBankerScore);

	lMeMaxScore /= iTimer;

	//非零限制
	//ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = MAX(lMeMaxScore, 0);

	return lMeMaxScore;
}
//更新控制
void GameControlOxHundred::updateButtonContron(){
	bool bEnablePlaceJetton = true;
	switch (DataModel::sharedDataModel()->getMainSceneOxHundred()->getGameState())
	{
	case MainSceneOxHundred::STATE_GAME_FREE:
	{
		bEnablePlaceJetton = false;
		if (m_wBankerUser == DataModel::sharedDataModel()->userInfo->wChairID)
		{
			PopDialogBoxUpBank *pUpBank = (PopDialogBoxUpBank*)(getParent()->getChildByTag(TAG_UP_BANK));
			if (pUpBank)
			{
				pUpBank->updateUpBankState();
			}
		}
		
	}
	break;
	case MainSceneOxHundred::STATE_GAME_PLACE_JETTON:
	{
		if (m_wBankerUser == DataModel::sharedDataModel()->userInfo->wChairID)
		{
			PopDialogBoxUpBank *pUpBank = (PopDialogBoxUpBank*)(getParent()->getChildByTag(TAG_UP_BANK));
			if (pUpBank)
			{
				pUpBank->updateUpBankState();
			}
		}
	}
		break;
	case MainSceneOxHundred::STATE_GAME_SEND_CARD:
	{
		bEnablePlaceJetton = false;
	}
	break;
	default:
		break;
	}
	//if (m_bMeApplyBanker)
	if (m_bMeApplyBanker==1&&m_wBankerUser == DataModel::sharedDataModel()->userInfo->wChairID)
	{
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	if (bEnablePlaceJetton)
	{
		for (int i = 0; i < MAX_JETTON_BUTTON_COUNT; i++)
		{
			pIJettonButton[i]->setColor(ccc3(255, 255, 255));
			pIJettonButton[i]->setTouchEnabled(true);
			pIJettonButton[0]->setVisible(true);
		}
		if (iCurSelectJettonIndex < 0)
		{
			iCurSelectJettonIndex = 0;
		}
		pIJettonSelect->setVisible(true);
		//////////////////////////////////////////////////////////////////////////
		//计算积分
		long long lCurrentJetton = nJetton[iCurSelectJettonIndex];//当前筹码
		long long lLeaveScore = m_lMeMaxScore;//我的金币
		for (int nAreaIndex = 1; nAreaIndex <= MAX_AREA_COUNT; ++nAreaIndex) lLeaveScore -= m_lUserJettonScore[nAreaIndex];

		//最大下注
		long long lUserMaxJetton = getUserMaxJetton();
		//CCLOG("---最大下注:%lld<<%s>>",lUserMaxJetton,__FUNCTION__);
		//设置光标
		lLeaveScore = MIN((lLeaveScore / 10), lUserMaxJetton); //用户可下分 和最大分比较 由于是五倍 
		//CCLOG("---lLeaveScore:%lld<<%s>>",lLeaveScore,__FUNCTION__);
		if (lCurrentJetton > lLeaveScore)
		{
			/*if (lLeaveScore>=5000000L) m_GameClientView.SetCurrentJetton(5000000L);
			else if (lLeaveScore>=1000000L) m_GameClientView.SetCurrentJetton(1000000L);
			else if (lLeaveScore>=500000L) m_GameClientView.SetCurrentJetton(500000L);
			else if (lLeaveScore>=100000L) m_GameClientView.SetCurrentJetton(100000L);
			else if (lLeaveScore>=50000L) m_GameClientView.SetCurrentJetton(50000L);
			else if (lLeaveScore>=10000L) m_GameClientView.SetCurrentJetton(10000L);
			else if (lLeaveScore>=1000L) m_GameClientView.SetCurrentJetton(1000L);
			else if (lLeaveScore>=100L) m_GameClientView.SetCurrentJetton(100L);
			else m_GameClientView.SetCurrentJetton(0L);*/

		}


		//控制按钮
		int iTimer = 1;
		for (int i = 0; i < MAX_JETTON_BUTTON_COUNT; i++)
		{
			if (lLeaveScore < nJetton[i] * iTimer || lUserMaxJetton < nJetton[i] * iTimer)
			{
				pIJettonButton[i]->setColor(ccc3(100, 100, 100));
				pIJettonButton[i]->setTouchEnabled(false);
				if (iCurSelectJettonIndex == i)
				{
					iCurSelectJettonIndex--;
					if (iCurSelectJettonIndex >= 0)
					{
						pIJettonSelect->setPosition(pIJettonButton[iCurSelectJettonIndex]->getPosition());
					}
					else
					{
						pIJettonSelect->setVisible(false);
					}
				}
			}
		}
		/**/
		/*pIJettonButton[0]->setVisible((lLeaveScore>=100*iTimer && lUserMaxJetton>=100*iTimer)?true:false);
		pIJettonButton[1]->setVisible((lLeaveScore>=5000*iTimer && lUserMaxJetton>=5000*iTimer)?true:false);
		pIJettonButton[2]->setVisible((lLeaveScore>=10000*iTimer && lUserMaxJetton>=10000*iTimer)?true:false);
		pIJettonButton[3]->setVisible((lLeaveScore>=100000*iTimer && lUserMaxJetton>=100000*iTimer)?true:false);
		pIJettonButton[4]->setVisible((lLeaveScore>=500000*iTimer && lUserMaxJetton>=500000*iTimer)?true:false);
		CCLOG("%lld    %lld<<%s>>",lLeaveScore,lUserMaxJetton,__FUNCTION__);*/
		/*m_GameClientView.m_btJetton100.EnableWindow((lLeaveScore>=100*iTimer && lUserMaxJetton>=100*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000.EnableWindow((lLeaveScore>=1000*iTimer && lUserMaxJetton>=1000*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow((lLeaveScore>=10000*iTimer && lUserMaxJetton>=10000*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton50000.EnableWindow((lLeaveScore>=50000*iTimer && lUserMaxJetton>=50000*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton100000.EnableWindow((lLeaveScore>=100000*iTimer && lUserMaxJetton>=100000*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton500000.EnableWindow((lLeaveScore>=500000*iTimer && lUserMaxJetton>=500000*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow((lLeaveScore>=1000000*iTimer && lUserMaxJetton>=1000000*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton5000000.EnableWindow((lLeaveScore>=5000000*iTimer && lUserMaxJetton>=5000000*iTimer)?TRUE:FALSE);
		*/


	}
	else
	{
		for (int i = 0; i < MAX_JETTON_BUTTON_COUNT; i++)
		{
			pIJettonButton[i]->setColor(ccc3(100, 100, 100));
			pIJettonButton[i]->setTouchEnabled(false);
		}
		pIJettonSelect->setVisible(false);
	}
}
//更新状态
void GameControlOxHundred::updateState(){
	isPalySoundHundedWarn = false;
	switch (DataModel::sharedDataModel()->getMainSceneOxHundred()->getGameState())
	{
	case MainSceneOxHundred::STATE_GAME_SHOW_CARE_FINISH:
	{
		//分发筹码
		scheduleOnce(SEL_SCHEDULE(&GameControlOxHundred::bankJettonIn), 1);
	}
	break;
	case MainSceneOxHundred::STATE_GAME_PLACE_JETTON:
	{
		//CCSize dSize = DataModel::sharedDataModel()->deviceSize;
		//showAnimationByName(this, ccp(dSize.width/2,dSize.height/2-60), "addScore");
		isPalySoundHundedWarn = true;
		beginAddScoreEffect();
	}
		break;
	case MainSceneOxHundred::STATE_GAME_END:
	{
		resetData();
		for (int i = 0; i < MAX_SEAT_COUNT; i++)
		{
			pSeatData[i]->resetData();
		}
		//隐藏所有筹码
		for (int i = 0; i < DataModel::sharedDataModel()->vecJettonNode.size(); i++)
		{
			DataModel::sharedDataModel()->vecJettonNode[i]->hideJetton();
		}
		//隐藏牌
		getMainScene()->cardLayer->resetCard();
		showTimer();

	}
	break;
	default:
		break;
	}
}
//设置庄家
void GameControlOxHundred::setBankerInfo(unsigned short  wBanker, long long lScore){
	m_wCurrentBanker = wBanker;
	//庄家椅子号
	WORD wBankerUser = INVALID_CHAIR;

	//查找椅子号
	if (0 != wBanker)
	{
		map<long, tagUserInfo>::iterator it;
		for (it = DataModel::sharedDataModel()->mTagUserInfo.begin(); it != DataModel::sharedDataModel()->mTagUserInfo.end(); it++)
		{
			if (it->second.wChairID == wBanker)
			{
				wBankerUser = wBanker;
			}
		}
		/*for (WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
		{
		IClientUserItem *pUserItem = GetClientUserItem(wChairID);
		if(pUserItem == NULL) continue;
		tagUserInfo  *pUserData=pUserItem->GetUserInfo();
		if (NULL!=pUserData && dwBankerUserID==pUserData->dwUserID)
		{
		wBankerUser=wChairID;
		break;
		}
		}*/
	}

	//切换判断
	if (m_wBankerUser != wBankerUser)
	{
		m_wBankerUser = wBankerUser;
		//m_wBankerTime=0L;
		//m_lBankerWinScore=0L;	
		//m_lTmpBankerWinScore=0L;
	}
	m_lBankerScore = lScore;
}
/*//站立并退出
void GameControlOxHundred::standUpWithExit(){

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
		CCLOG("-------userStandUp.wChairID :%d<<%s>>", userStandUp.wChairID, __FUNCTION__);
		//发送消息
		TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_LOGON_ROOM)->SendData(MDM_GR_USER, SUB_GR_USER_STANDUP, &userStandUp, sizeof(userStandUp));
	}
}*/
//申请庄家
void GameControlOxHundred::onApplyBanker(bool bApplyBanker){
	//当前判断
	if (m_wBankerUser == DataModel::sharedDataModel()->userInfo->wChairID && bApplyBanker){
		return;
	}

	if (bApplyBanker)
	{
		//发送消息
		GameIngMsgHandler::sharedGameIngMsgHandler()->gameSocket.SendData(MDM_GF_GAME, SUB_C_APPLY_BANKER);
		//m_bMeApplyBanker=true;
	}
	else
	{
		//发送消息
		GameIngMsgHandler::sharedGameIngMsgHandler()->gameSocket.SendData(MDM_GF_GAME, SUB_C_CANCEL_BANKER, NULL, 0);
		//m_bMeApplyBanker = false;
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void GameControlOxHundred::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_MB_UNCONNECT:
		onUnconnect(wSubCmdID);
		break;
	case MDM_GR_USER://用户信息
		onSubUserInfo(wSubCmdID, pDataBuffer, wDataSize);
		break;
	case MDM_GF_GAME://游戏命令
		onEventGameIng(wSubCmdID, pDataBuffer, wDataSize);
		break;
	case MDM_GF_FRAME://框架命令
		onSubGameFrame(wSubCmdID, pDataBuffer, wDataSize);
		break;
	default:
		CCLOG("----------------main:%d sub:%d<<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
		break;
	}
}
//框架命令
void GameControlOxHundred::onSubGameFrame(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GF_GAME_STATUS://游戏状态
	{
		//效验参数
		assert(wDataSize == sizeof(CMD_GF_GameStatus));
		if (wDataSize != sizeof(CMD_GF_GameStatus)) return;
		//消息处理
		CMD_GF_GameStatus * pGameStatus = (CMD_GF_GameStatus *)pDataBuffer;
		//设置变量
		m_cbGameStatus = pGameStatus->cbGameStatus;
		m_bAllowLookon = pGameStatus->cbAllowLookon ? true : false;
	}
	break;
	case SUB_GF_GAME_SCENE://游戏场景
	{
		switch (m_cbGameStatus)
		{
		case GAME_SCENE_FREE:
		{
			int ss = sizeof(CMD_S_StatusFree);
			//效验数据  
			assert(wDataSize == sizeof(CMD_S_StatusFree));
			if (wDataSize != sizeof(CMD_S_StatusFree)) return;

			//消息处理
			CMD_S_StatusFree * pStatusFree = (CMD_S_StatusFree *)pDataBuffer;
			//设置筹码起始下标
			lJettonState = pStatusFree->lJettonState;
			for (int i = 0; i < MAX_JETTON_BUTTON_COUNT; i++)
			{
				nJetton[i] = nAllJetton[lJettonState + i];
				pIJettonButton[i] = static_cast<UIImageView*>(pWidget->getWidgetByName(CCString::createWithFormat("ImageJetton%d", i)->getCString()));
				pIJettonButton[i]->loadTexture(CCString::createWithFormat("u_gih_jetton%d.png",i+lJettonState)->getCString(), UI_TEX_TYPE_PLIST);
				pIJettonButton[i]->setVisible(true);
			}
		

			//设置时间
			resetTimer(pStatusFree->cbTimeLeave, BaseAttributes::sharedAttributes()->sGameFree.c_str());

			//庄家信息
			setBankerInfo(pStatusFree->wBankerUser, pStatusFree->lBankerScore);
			//设置用户信息
			tagUserInfo *uInfo = getUserInfo(pStatusFree->wBankerUser);
			if (uInfo)
			{
				pPlayerData[1]->setUserInfo(*uInfo);
			}
			else
			{
				pPlayerData[1]->hidePlayer();
			}
			//上庄条件
			llApplyBankerCondition = pStatusFree->lApplyBankerCondition;
			/*
			//玩家信息
			m_lMeMaxScore=pStatusFree->lUserMaxScore;
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);

			const tagUserInfo *pMeUserData=GetMeUserItem()->GetUserInfo();
			m_GameClientView.SetMeChairID(SwitchViewChairID(GetMeChairID()));

			//庄家信息
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->lBankerWinScore);
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//控制信息
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			//播放声音
			PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));

			//设置状态
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
			m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

			if(IsLookonMode()==false && GetMeChairID() == m_wCurrentBanker)
			{
			m_bMeApplyBanker =true;
			}

			//更新控制
			UpdateButtonContron();
			m_GameClientView.InvalidGameView(0,0,0,0);
			*/

			m_lAreaLimitScore = pStatusFree->lAreaLimitScore;
			//更新控制
			updateButtonContron();
		}
		break;
		case GAME_SCENE_PLACE_JETTON:		//游戏状态
		case GAME_SCENE_GAME_END:		//结束状态
		{
			//效验数据
			assert(wDataSize == sizeof(CMD_S_StatusPlay));
			if (wDataSize != sizeof(CMD_S_StatusPlay)) return;

			//消息处理
			CMD_S_StatusPlay * pStatusPlay = (CMD_S_StatusPlay *)pDataBuffer;
			//设置筹码起始下标
			lJettonState = pStatusPlay->lJettonState;
			for (int i = 0; i < MAX_JETTON_BUTTON_COUNT; i++)
			{
				nJetton[i] = nAllJetton[lJettonState + i];
				pIJettonButton[i] = static_cast<UIImageView*>(pWidget->getWidgetByName(CCString::createWithFormat("ImageJetton%d", i)->getCString()));
				pIJettonButton[i]->loadTexture(CCString::createWithFormat("u_gih_jetton%d.png", i + lJettonState)->getCString(), UI_TEX_TYPE_PLIST);
				pIJettonButton[i]->setVisible(true);
			}
			m_lAreaLimitScore = pStatusPlay->lAreaLimitScore;
			//下注信息
			for (int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; ++nAreaIndex)
			{
				m_lAllJettonScore[nAreaIndex] += pStatusPlay->lAllJettonScore[nAreaIndex];
				pSeatData[nAreaIndex - 1]->setAllJettonByAdd(pStatusPlay->lAllJettonScore[nAreaIndex]);
				createJettonInit(nAreaIndex-1, pStatusPlay->lAllJettonScore[nAreaIndex]);
			}
			//上庄条件
			llApplyBankerCondition = pStatusPlay->lApplyBankerCondition;
			CCLOG("GAME_SCENE_PLACE_JETTON|GAME_SCENE_GAME_END<<%s>>", __FUNCTION__);
			/*


			//玩家积分
			m_lMeMaxScore=pStatusPlay->lUserMaxScore;
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			const tagUserInfo *pMeUserData=GetMeUserItem()->GetUserInfo();
			m_GameClientView.SetMeChairID(SwitchViewChairID(GetMeChairID()));

			//控制信息
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);
			*/
			


			if (pStatusPlay->cbGameStatus==GAME_SCENE_GAME_END)
			{
				//扑克信息
				//@m_GameClientView.SetCardInfo(pStatusPlay->cbTableCardArray);
				//@m_GameClientView.FinishDispatchCard(false);

				//@m_GameClientView.m_bShowGameResult = true;
				//@m_GameClientView.m_blMoveFinish = true;
				for (int i = 0;i<5;i++)
				{
					//@m_GameClientView.m_CardControl[i].m_blGameEnd = true;
					//@m_GameClientView.m_CardControl[i].m_blhideOneCard = false;

					//BYTE bcTmp[5];
					for (int j = 0; j < 5; j++)
					{
						getMainScene()->cardLayer->card[i][j] = pStatusPlay->cbTableCardArray[i][j];
					}
					getMainScene()->cardLayer->showCard(false,i,i);
					//@int iType = m_GameLogic.GetCardType(pStatusPlay->cbTableCardArray[i],5,bcTmp);

					//@m_GameClientView.m_lUserCardType[i] = iType;
					/*if(iType==CT_POINT||iType==CT_SPECIAL_BOMEBOME)
					{
						CopyMemory(m_GameClientView.m_cbTableSortCardArray[i],bcTmp,5);
						m_GameClientView.m_CardControl[i].m_blShowLineResult = true;
					}
					else
					{
						CopyMemory(m_GameClientView.m_cbTableSortCardArray[i],bcTmp+3,2);
						CopyMemory(m_GameClientView.m_cbTableSortCardArray[i]+2,bcTmp,3);
					m_GameClientView.m_CardControl[i].m_blShowLineResult = false;*/
				}

				//@m_GameClientView.m_CardControl[i].SetCardData(m_GameClientView.m_cbTableSortCardArray[i],5,false);
				//@m_GameClientView.m_CardControl[i].m_blShowResult = true;
				//推断赢家
				bool bWinTianMen, bWinDiMen, bWinXuanMen, bWinHuang;
				BYTE TianMultiple, diMultiple, TianXuanltiple, HuangMultiple;
				deduceWinner(bWinTianMen, bWinDiMen, bWinXuanMen, bWinHuang, TianMultiple, diMultiple, TianXuanltiple, HuangMultiple);
				//m_GameClientView.SetGameHistory(pServerGameRecord->bWinShunMen, pServerGameRecord->bWinDuiMen, pServerGameRecord->bWinDaoMen,pServerGameRecord->bWinHuang);

				tagGameRecord tagRecord;
				tagRecord.bWinShunMen = bWinTianMen;
				tagRecord.bWinDuiMen = bWinDiMen;
				tagRecord.bWinDaoMen = bWinXuanMen;
				tagRecord.bWinHuang = bWinHuang;

				insertGameHistory(tagRecord);
				//分发筹码
				scheduleOnce(SEL_SCHEDULE(&GameControlOxHundred::bankJettonIn), 1);
			}
			
		
			

			//设置总结算
			lUserScore = pStatusPlay->lEndUserScore;
			lBankerScore = pStatusPlay->lEndBankerScore;
			
			
			/*
			//设置成绩
			m_GameClientView.SetCurGameScore(pStatusPlay->lEndUserScore,pStatusPlay->lEndUserReturnScore,pStatusPlay->lEndBankerScore,pStatusPlay->lEndRevenue);
			}
			else
			{
			m_GameClientView.SetCardInfo(NULL);

			for (int i = 0;i<5;i++)
			{
			m_GameClientView.m_CardControl[i].m_CardItemArray.SetSize(0);
			}

			}

			//播放声音
			PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));
			*/
			//庄家信息
			setBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
			//设置用户信息
			tagUserInfo *uInfo = getUserInfo(pStatusPlay->wBankerUser);
			if (uInfo)
			{
				pPlayerData[1]->setUserInfo(*uInfo);
			}
			else
			{
				pPlayerData[1]->hidePlayer();
			}
			/*m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->lBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
			m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			*/
			if(IsLookonMode()==false && DataModel::sharedDataModel()->userInfo->dwUserID == m_wCurrentBanker)
			{
				m_bMeApplyBanker =1;
			}
			

			//更新按钮
			updateButtonContron();
			/*

			//设置状态
			SetGameStatus(pStatusPlay->cbGameStatus);
			*/
			//设置时间
			if (pStatusPlay->cbGameStatus == GAME_SCENE_GAME_END)
			{
				//resetTimer(pStatusPlay->cbTimeLeave, BaseAttributes::sharedAttributes()->sGameEnd.c_str());
			}
			else
			{
				resetTimer(pStatusPlay->cbTimeLeave, BaseAttributes::sharedAttributes()->sGameStart.c_str());
			}
			/*
			m_GameClientView.InvalidGameView(0,0,0,0);
			*/
		}
		break;
		default:
			break;
		}

	}
	break;
	case SUB_GF_SYSTEM_MESSAGE://系统消息
	{
		onSocketSubSystemMessage(pDataBuffer, wDataSize);
	}
	break;
	default:
		CCLOG("-:%d<<%s>>", wSubCmdID, __FUNCTION__);
		break;
	}
}
//系统消息
void GameControlOxHundred::onSocketSubSystemMessage(void * pData, unsigned short wDataSize){
	//变量定义
	CMD_CM_SystemMessage * pSystemMessage = (CMD_CM_SystemMessage *)pData;
	WORD wHeadSize = sizeof(CMD_CM_SystemMessage) - sizeof(pSystemMessage->szString);


	//效验参数
	assert((wDataSize > wHeadSize) && (wDataSize == (wHeadSize + pSystemMessage->wLength*sizeof(TCHAR))));
	if ((wDataSize <= wHeadSize) || (wDataSize != (wHeadSize + pSystemMessage->wLength*sizeof(TCHAR)))) return;

	//关闭处理
	if ((pSystemMessage->wType&SMT_CLOSE_GAME) != 0)
	{
		/*//设置变量
		m_bService = false;

		//删除时间
		KillGameClock(0);

		//中断连接
		IntermitConnect();*/
		CCLOG("%s<<%s>>", Tools::GBKToUTF8("关闭处理").c_str(), __FUNCTION__);
	}
	//显示消息
	if ((pSystemMessage->wType&SMT_CHAT))
		//if ((pSystemMessage->wType&SMT_CHAT) && (m_pIStringMessage != NULL))
	{
		PopDialogBoxMakeText *popMakeText = PopDialogBoxMakeText::create();
		getMainScene()->addChild(popMakeText, K_Z_ORDER_POP);

		std::string sContent = Tools::GBKToUTF8(pSystemMessage->szString);
		sContent.erase(sContent.find_last_not_of("\n") + 1);

		popMakeText->showMakeText(sContent);
		CCLOG("makeTextSize:%d <<%s>>", DataModel::sharedDataModel()->m_aMakeText->count(), __FUNCTION__);
		//
		CCObject *object = NULL;
		CCARRAY_FOREACH(DataModel::sharedDataModel()->m_aMakeText,object)
		{
			PopDialogBoxMakeText *tempPopMakeText = (PopDialogBoxMakeText*)object;
			tempPopMakeText->movePlane(popMakeText->pIVBg->getSize().height + 6);
		}
	
		DataModel::sharedDataModel()->m_aMakeText->addObject(popMakeText);
		CCLOG("showTip:%s<<%s>>", Tools::GBKToUTF8(pSystemMessage->szString).c_str(), __FUNCTION__);
		//m_pIStringMessage->InsertSystemString(pSystemMessage->szString);
	}

	//弹出消息
	if (pSystemMessage->wType&SMT_EJECT)
	{
		CCLOG("----%s<<%s>>", Tools::GBKToUTF8("弹出消息").c_str(), __FUNCTION__);
		/*CString sErrorMsg = pSystemMessage->szString;
		CInformation Information(AfxGetMainWnd());
		if (pSystemMessage->wType&SMT_NOGOLD)
		{
		if (Information.ShowMessageBox(pSystemMessage->szString, MB_ICONINFORMATION, 30L, 1) == IDOK)
		{
		//构造地址
		CGlobalServer * pGlobalServer = CGlobalServer::GetInstance();
		ShellExecute(NULL, TEXT("OPEN"), pGlobalServer->GetPlatformServer(6), NULL, NULL, SW_NORMAL);
		}
		}
		else
		{
		Information.ShowMessageBox(pSystemMessage->szString, MB_ICONINFORMATION, 0);
		}*/
	}

	//关闭房间
	if (pSystemMessage->wType&SMT_CLOSE_GAME)
	{
		CCLOG("----%s<<%s>>", Tools::GBKToUTF8("关闭房间").c_str(), __FUNCTION__);
		//m_pIClientKernelSink->CloseGameClient();
	}
}
void GameControlOxHundred::onEventGameIng(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE://游戏空闲
		onSubGameFree(pDataBuffer, wDataSize);
		break;
	case SUB_S_GAME_START://游戏开始
		onSubGameStart(pDataBuffer, wDataSize);
		break;
	case SUB_S_PLACE_JETTON://用户下注
		onSubPlaceJetton(pDataBuffer, wDataSize, true);
		break;
	case SUB_S_GAME_END://游戏结束
		onSubGameEnd(pDataBuffer, wDataSize);
		break;
	case SUB_S_APPLY_BANKER://申请庄家
		onSubUserApplyBanker(pDataBuffer, wDataSize);
		break;
	case SUB_S_CHANGE_BANKER://切换庄家
		onSubChangeBanker(pDataBuffer, wDataSize);
		break;
	case SUB_S_CHANGE_USER_SCORE://更新积分
		break;
	case SUB_S_SEND_RECORD://游戏记录
		onSubGameRecord(pDataBuffer, wDataSize);
		break;
	case SUB_S_PLACE_JETTON_FAIL://下注失败
		onSubPlaceJettonFail(pDataBuffer, wDataSize);
		break;
	case SUB_S_CANCEL_BANKER://取消申请
		onSubUserCancelBanker(pDataBuffer, wDataSize);
		break;
	case SUB_S_SEND_ACCOUNT://发送账号
		break;
	case SUB_S_ADMIN_CHEAK://查询账号
		break;
	case SUB_S_QIANG_ZHUAN://抢庄
		onQiangZhuanRet(pDataBuffer, wDataSize);
		break;
	case SUB_S_AMDIN_COMMAND://管理员命令
		break;
	case SUB_S_USER_SIT://用户坐下椅子
		onSubUserSitState(pDataBuffer, wDataSize);
		break;
	case SUB_S_USER_LEAVE://用户离开椅子
		onSubUserUpState(pDataBuffer, wDataSize);
		break;
	default:
		CCLOG("===========sub:%d<<%s>>", wSubCmdID, __FUNCTION__);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
//游戏空闲
void GameControlOxHundred::onSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	assert(wDataSize == sizeof(CMD_S_GameFree));
	if (wDataSize != sizeof(CMD_S_GameFree)) return;
	//消息处理
	CMD_S_GameFree * pGameFree = (CMD_S_GameFree *)pBuffer;
	//设置时间
	resetTimer(pGameFree->cbTimeLeave, BaseAttributes::sharedAttributes()->sGameFree.c_str());
	CCLOG("FreeTime:%s <<%s>>", Tools::get_date_now().c_str(), __FUNCTION__);
	CCLOG("FreeTime:%d   count:%lld<<%s>>", pGameFree->cbTimeLeave, pGameFree->nListUserCount, __FUNCTION__);
	resetData();
	for (int i = 0; i < MAX_SEAT_COUNT; i++)
	{
		pSeatData[i]->resetData();
	}
	//隐藏所有筹码
	for (int i = 0; i < DataModel::sharedDataModel()->vecJettonNode.size(); i++)
	{
		DataModel::sharedDataModel()->vecJettonNode[i]->hideJetton();
	}
	//隐藏牌
	getMainScene()->cardLayer->resetCard();
	//设置空闲状态
	DataModel::sharedDataModel()->getMainSceneOxHundred()->setGameStateWithUpdate(MainSceneOxHundred::STATE_GAME_FREE);
	//操控更新
	updateButtonContron();
}
//游戏开始
void GameControlOxHundred::onSubGameStart(const void * pBuffer, WORD wDataSize){
	//效验数据
	assert(wDataSize == sizeof(CMD_S_GameStart));
	if (wDataSize != sizeof(CMD_S_GameStart)) return;
	Tools::playSound(kSoundHundredStart);
	//消息处理
	CMD_S_GameStart * pGameStart = (CMD_S_GameStart *)pBuffer;
	m_lMeMaxScore = pGameStart->lUserMaxScore;
	//设置用户信息
	tagUserInfo *uInfo = getUserInfo(pGameStart->wBankerUser);
	if (uInfo)
	{
		pPlayerData[1]->setUserInfo(*uInfo);
	}
	else
	{
		pPlayerData[1]->hidePlayer();
	}

	CCLOG("gameStart=time--:%d<<%s>>", pGameStart->cbTimeLeave, __FUNCTION__);
	//设置时间
	resetTimer(pGameStart->cbTimeLeave, BaseAttributes::sharedAttributes()->sGameStart.c_str());
	hideTimer(false);
    
	//设置下注状态
    getMainScene()->setGameStateWithUpdate(MainSceneOxHundred::STATE_GAME_PLACE_JETTON);
	//DataModel::sharedDataModel()->getMainSceneOxHundred()->setGameStateWithUpdate(MainSceneOxHundred::STATE_GAME_PLACE_JETTON);
	setBankerInfo(pGameStart->wBankerUser, pGameStart->lBankerScore);
	//操控更新
	updateButtonContron();
    
}
//用户下注
void GameControlOxHundred::onSubPlaceJetton(const void * pBuffer, WORD wDataSize, bool bGameMes){
	//效验数据
	assert(wDataSize == sizeof(CMD_S_PlaceJetton));
	if (wDataSize != sizeof(CMD_S_PlaceJetton)) return;
	Tools::playSound(kSoundHundredAddGold);
	//消息处理
	CMD_S_PlaceJetton * pPlaceJetton = (CMD_S_PlaceJetton *)pBuffer;

	pSeatData[pPlaceJetton->cbJettonArea - 1]->setAllJettonByAdd(pPlaceJetton->lJettonScore);

	CCPoint posBegin = pBOnline->getPosition();
	if (DataModel::sharedDataModel()->userInfo->wChairID != pPlaceJetton->wChairID)
	{
		m_lAllJettonScore[pPlaceJetton->cbJettonArea] += pPlaceJetton->lJettonScore;
	}
	else
	{
		posBegin = pPlayerData[0]->pIPlayerBg->getPosition();
	}
	//////////////////////////////////////////////////////////////////////////


	//int fWidth=pSeatData[pPlaceJetton->cbJettonArea-1]->seatSize.width-jetton->getContentSize().width/2;
	//int fHeight=pSeatData[pPlaceJetton->cbJettonArea-1]->seatSize.height-jetton->getContentSize().height/2-70;
	int fWidth = pSeatData[pPlaceJetton->cbJettonArea - 1]->seatSize.width - 31;
	int fHeight = pSeatData[pPlaceJetton->cbJettonArea - 1]->seatSize.height - 70 - 31;

	int offsetX = rand() % fWidth;
	int offsetY = rand() % fHeight;
	CCPoint randPos = ccp(offsetX, offsetY);
	CCPoint pos = pSeatData[pPlaceJetton->cbJettonArea - 1]->posCenter;
	pos = ccpAdd(pos, randPos);
	pos = ccpSub(pos, ccp(fWidth / 2, fHeight / 2));

	JettonNode *pJetton = getJettonNode();
	pJetton->iBetArea = pPlaceJetton->cbJettonArea - 1;
    Tools::playSound(kSoundHundredAddGold);
	pJetton->setJettonTypeWithMove(pPlaceJetton->lJettonScore, posBegin, pos);
	//CCLOG("chair:%d jettonScore: %lld<<%s>>",pPlaceJetton->wChairID,pPlaceJetton->lJettonScore,__FUNCTION__);
	updateButtonContron();
}
//申请庄家
void GameControlOxHundred::onSubUserApplyBanker(const void * pBuffer, WORD wDataSize){
	//效验数据
	assert(wDataSize == sizeof(CMD_S_ApplyBanker));
	if (wDataSize != sizeof(CMD_S_ApplyBanker)) return;

	//消息处理
	CMD_S_ApplyBanker * pApplyBanker = (CMD_S_ApplyBanker *)pBuffer;
	//自己判断
	if (DataModel::sharedDataModel()->userInfo->wChairID == pApplyBanker->wApplyUser) {
		m_bMeApplyBanker = 1;
	}
	//插入庄家列表
	if (m_wBankerUser != pApplyBanker->wApplyUser)
	{
		map<long, tagUserInfo>::iterator it;
		bool isFind = false;
		for (it = DataModel::sharedDataModel()->mTagUserInfo.begin(); it != DataModel::sharedDataModel()->mTagUserInfo.end(); it++)
		{
			if (it->second.wChairID == pApplyBanker->wApplyUser)
			{
				tagApplyUser ApplyUser;
				ApplyUser.strUserName = it->second.szNickName;
				ApplyUser.lUserScore = it->second.lScore;
				listApplyUser.push_back(ApplyUser);
				isFind = true;
			}

		}
		if (!isFind)
		{
			//CCLOG("=no %d   %d-----------------------------<<%s>>", pApplyBanker->wApplyUser, DataModel::sharedDataModel()->userInfo->wChairID, __FUNCTION__);
			//CCLOG("<<%s>>", __FUNCTION__);
		}
		CCLOG("%s <<%s>>", Tools::GBKToUTF8("上庄切换").c_str(), __FUNCTION__);
		MTNotificationQueue::sharedNotificationQueue()->postNotification(UPDATE_BANK_LIST, NULL);
	}


	//CCLOG("11============================  %d<<%s>>",pApplyBanker->wApplyUser,__FUNCTION__);
	/*
	IClientUserItem *pUserItem = GetTableUserItem(pApplyBanker->wApplyUser);
	tagUserInfo		*pUserData = NULL;
	if(pUserItem != NULL)
	pUserData = pUserItem->GetUserInfo();

	//插入玩家
	if (m_wCurrentBanker!=pApplyBanker->wApplyUser && pUserData!=NULL)
	{
	tagApplyUser ApplyUser;
	ApplyUser.strUserName=pUserData->szNickName;
	ApplyUser.lUserScore=pUserData->lScore;
	m_GameClientView.m_ApplyUser.InserUser(ApplyUser);
	m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(TRUE);

	if(m_GameClientView.m_ApplyUser.GetItemCount()>MAX_APPLY_DISPLAY)
	{
	m_GameClientView.m_btUp.EnableWindow(true);
	m_GameClientView.m_btDown.EnableWindow(true);

	}else
	{
	m_GameClientView.m_btUp.EnableWindow(false);
	m_GameClientView.m_btDown.EnableWindow(false);
	}
	}
	*/
	
	/*
	//更新控件
	UpdateButtonContron();
	m_GameClientView.m_btCancelBanker.EnableWindow(TRUE);
	*/
}
//取消做庄
void GameControlOxHundred::onSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	assert(wDataSize == sizeof(CMD_S_CancelBanker));
	if (wDataSize != sizeof(CMD_S_CancelBanker)) return;

	//消息处理
	CMD_S_CancelBanker * pCancelBanker = (CMD_S_CancelBanker *)pBuffer;
	tagApplyUser ApplyUser;
	ApplyUser.strUserName = pCancelBanker->szCancelUser;

	//remove_if(listApplyUser.begin(),listApplyUser.end(),(std::string i){return strcmp(i.c_str(),ApplyUser.strUserName.c_str());});
	CCLOG("%d<<%s>>", listApplyUser.size(), __FUNCTION__);
	std::list <tagApplyUser> ::iterator iter;
	for (iter = listApplyUser.begin(); iter != listApplyUser.end();)
	{
		if (strcmp(iter->strUserName.c_str(), ApplyUser.strUserName.c_str()) == 0)
		{
			//std::list<tagApplyUser>::iterator iter_e=iter;

			//listApplyUser.remove(ApplyUser);
			//CCLOG("%s<<%s>>",iter->strUserName.c_str(),__FUNCTION__);
			listApplyUser.erase(iter++);
		}
		else
		{
			iter++;
		}
	}
	//自己判断
	const tagUserInfo *pMeUserData = DataModel::sharedDataModel()->userInfo;
	if (strcmp(pMeUserData->szNickName, pCancelBanker->szCancelUser) == 0)
	{
		m_bMeApplyBanker = 0;
	}
	CCLOG("%s <<%s>>", Tools::GBKToUTF8("下庄切换").c_str(), __FUNCTION__);
	//发送更新列表通知
	MTNotificationQueue::sharedNotificationQueue()->postNotification(UPDATE_BANK_LIST, NULL);

	/*tagApplyUser ApplyUser;
	ApplyUser.strUserName=pCancelBanker->szCancelUser;


	ApplyUser.lUserScore=0;


	m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);
	m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(TRUE);

	if(m_GameClientView.m_ApplyUser.GetItemCount()>MAX_APPLY_DISPLAY)
	{
	m_GameClientView.m_btUp.EnableWindow(true);
	m_GameClientView.m_btDown.EnableWindow(true);

	}else
	{
	m_GameClientView.m_btUp.EnableWindow(false);
	m_GameClientView.m_btDown.EnableWindow(false);
	}*/

	



	//更新控件
	updateButtonContron();
	//m_GameClientView.m_btApplyBanker.EnableWindow(TRUE);
}
//切换庄家
void GameControlOxHundred::onSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	assert(wDataSize == sizeof(CMD_S_ChangeBanker));
	if (wDataSize != sizeof(CMD_S_ChangeBanker)) return;

	//消息处理
	CMD_S_ChangeBanker * pChangeBanker = (CMD_S_ChangeBanker *)pBuffer;

	//显示图片
	//m_GameClientView.ShowChangeBanker(m_wCurrentBanker!=pChangeBanker->wBankerUser);

	//自己判断
	if (m_wBankerUser == DataModel::sharedDataModel()->userInfo->wChairID&& pChangeBanker->wBankerUser != DataModel::sharedDataModel()->userInfo->wChairID)
	{
		m_bMeApplyBanker = 0;
	}
	else if (pChangeBanker->wBankerUser == DataModel::sharedDataModel()->userInfo->wChairID)
	{
		isChangeUpBank = false;
		m_bMeApplyBanker = 1;
	}

	//庄家信息
	setBankerInfo(pChangeBanker->wBankerUser, pChangeBanker->lBankerScore);
	//m_GameClientView.SetBankerScore(0,0);
	//设置用户信息
	tagUserInfo *uInfo = getUserInfo(pChangeBanker->wBankerUser);
	if (uInfo)
	{
		pPlayerData[1]->setUserInfo(*uInfo);
	}
	else
	{
		pPlayerData[1]->hidePlayer();
	}
	//删除玩家
	//if (m_wBankerUser != INVALID_CHAIR)
	{
		if (listApplyUser.size() > 0)
		{
			listApplyUser.pop_front();
			CCLOG("%s <<%s>>", Tools::GBKToUTF8("切庄切换").c_str(), __FUNCTION__);
			MTNotificationQueue::sharedNotificationQueue()->postNotification(UPDATE_BANK_LIST, NULL);
		}
		/*IClientUserItem *pUserItem = GetTableUserItem(m_wCurrentBanker);
		tagUserInfo  *pBankerUserData = NULL;
		if(pUserItem!=NULL)
		pBankerUserData = pUserItem->GetUserInfo();

		if (pBankerUserData != NULL)
		{
		tagApplyUser ApplyUser;
		ApplyUser.strUserName = pBankerUserData->szNickName;
		m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);
		m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
		}*/
	}
	
	//更新界面
	updateButtonContron();
	//m_GameClientView.InvalidGameView(0,0,0,0);

}
//抢庄消息 
void GameControlOxHundred::onQiangZhuanRet(const void *pBuffer, WORD wDataSize)
{
	assert(wDataSize == sizeof(CMD_S_QiangZhuan));
	if (wDataSize != sizeof(CMD_S_QiangZhuan)) return;

	//消息处理
	CMD_S_QiangZhuan * pCmd = (CMD_S_QiangZhuan *)pBuffer;
	CCLOG("---<<%s>>", __FUNCTION__);
	for (int nIndex = pCmd->wSwap1; nIndex > pCmd->wSwap2; nIndex--)
	{
		list<tagApplyUser>::iterator iterSwap1 = listApplyUser.begin();  advance(iterSwap1, nIndex);
		list<tagApplyUser>::iterator iterSwap2 = listApplyUser.begin();  advance(iterSwap2, nIndex - 1);
		iter_swap(iterSwap1, iterSwap2);
	}
	MTNotificationQueue::sharedNotificationQueue()->postNotification(UPDATE_BANK_LIST, NULL);
	//TCHAR szNameTemp[31],szScoreTemp[128]=TEXT("");
	/*tagApplyUser tagAppUser;
	for(int nIndex=pCmd->wSwap1;nIndex>pCmd->wSwap2;nIndex--)
	{
	//tagAppUser=listApplyUser.begin(1);
	//		crTemp	= m_AppyUserList.GetItemColor(nIndex-1);
	//m_AppyUserList.GetItemText(nIndex-1,0,szNameTemp,31);
	//m_AppyUserList.GetItemText(nIndex-1,1,szScoreTemp,128);

	//m_AppyUserList.SetItemText(nIndex,0,szNameTemp);
	//m_AppyUserList.SetItemText(nIndex,1,szScoreTemp);
	//m_AppyUserList.SetItemColor(nIndex,crTemp);
	}*/
	//_stprintf(szScorePer,_T("%I64d"),lMeMoney);
	//m_AppyUserList.SetItemText(wCurPos,0,szNamePer);
	//m_AppyUserList.SetItemText(wCurPos,1,szScorePer);

	//m_GameClientView.m_ApplyUser.ResetItemPos(pCmd->wSwap1,pCmd->wSwap2,pCmd->lMeMoney);
}
//游戏记录
void GameControlOxHundred::onSubGameRecord(const void * pBuffer, WORD wDataSize){
	//效验参数
	assert(wDataSize%sizeof(tagServerGameRecord) == 0);
	if (wDataSize%sizeof(tagServerGameRecord) != 0) return;



	//设置记录
	WORD wRecordCount = wDataSize / sizeof(tagServerGameRecord);
	for (WORD wIndex = 0; wIndex<wRecordCount; wIndex++)
	{
		tagServerGameRecord * pServerGameRecord = (((tagServerGameRecord *)pBuffer) + wIndex);
		tagGameRecord tRecord;
		memcpy(&tRecord, pServerGameRecord, sizeof(tagServerGameRecord));
		insertGameHistory(tRecord);
		//CCLOG("==--::%d %d %d %d<<%s>>",pServerGameRecord->bWinShunMen, pServerGameRecord->bWinDuiMen, pServerGameRecord->bWinDaoMen,pServerGameRecord->bWinHuang,__FUNCTION__);
		//m_GameClientView.SetGameHistory(pServerGameRecord->bWinShunMen, pServerGameRecord->bWinDuiMen, pServerGameRecord->bWinDaoMen,pServerGameRecord->bWinHuang);
	}
	MTNotificationQueue::sharedNotificationQueue()->postNotification(UPDATE_LIST, NULL);
}
//插入历史记录
void GameControlOxHundred::insertGameHistory(tagGameRecord tagRecord){
	listGameRecord.push_back(tagRecord);
	if (listGameRecord.size()>MAX_SCORE_HISTORY)
	{
		listGameRecord.pop_front();
	}
}
//推断赢家
void GameControlOxHundred::deduceWinner(bool &bWinTian, bool &bWinDi, bool &bWinXuan, bool &bWinHuan, BYTE &TianMultiple, BYTE &diMultiple, BYTE &TianXuanltiple, BYTE &HuangMultiple)
{
	//大小比较
	bWinTian = CompareCard(getMainScene()->cardLayer->card[BANKER_INDEX], 5, getMainScene()->cardLayer->card[SHUN_MEN_INDEX], 5, TianMultiple) == 1 ? true : false;
	bWinDi = CompareCard(getMainScene()->cardLayer->card[BANKER_INDEX], 5, getMainScene()->cardLayer->card[DUI_MEN_INDEX], 5, diMultiple) == 1 ? true : false;
	bWinXuan = CompareCard(getMainScene()->cardLayer->card[BANKER_INDEX], 5, getMainScene()->cardLayer->card[DAO_MEN_INDEX], 5, TianXuanltiple) == 1 ? true : false;
	bWinHuan = CompareCard(getMainScene()->cardLayer->card[BANKER_INDEX], 5, getMainScene()->cardLayer->card[HUAN_MEN_INDEX], 5, HuangMultiple) == 1 ? true : false;
}
//下注失败
void GameControlOxHundred::onSubPlaceJettonFail(const void * pBuffer, WORD wDataSize){
	CCLOG("======================jettonFail------------<<%s>>", __FUNCTION__);
	//效验数据
	assert(wDataSize == sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize != sizeof(CMD_S_PlaceJettonFail)) return;

	//消息处理
	CMD_S_PlaceJettonFail * pPlaceJettonFail = (CMD_S_PlaceJettonFail *)pBuffer;
	CCLOG("%d<<%s>>", pPlaceJettonFail->wPlaceUser, __FUNCTION__);
	m_lAllJettonScore[pPlaceJettonFail->lJettonArea] -= pPlaceJettonFail->lPlaceScore;
	//自己判断
	if (DataModel::sharedDataModel()->userInfo->wChairID == pPlaceJettonFail->wPlaceUser
		//&& false==IsLookonMode()
		)
	{
		//效验参数
		BYTE cbViewIndex = pPlaceJettonFail->lJettonArea;
		long long lJettonCount = pPlaceJettonFail->lPlaceScore;
		//合法校验
		assert(m_lUserJettonScore[cbViewIndex] >= lJettonCount);
		if (lJettonCount > m_lUserJettonScore[cbViewIndex]) return;

		//设置下注
		m_lUserJettonScore[cbViewIndex] -= lJettonCount;
		pSeatData[cbViewIndex - 1]->setUserJetton(m_lUserJettonScore[cbViewIndex]);
	}
}
//游戏结束
void GameControlOxHundred::onSubGameEnd(const void * pBuffer, WORD wDataSize){
	//效验数据
	assert(wDataSize == sizeof(CMD_S_GameEnd));
	if (wDataSize != sizeof(CMD_S_GameEnd)) return;
	//消息处理
	CMD_S_GameEnd * pGameEnd = (CMD_S_GameEnd *)pBuffer;
	CCLOG("end:%lld<<%s>>", pGameEnd->lUserScore, __FUNCTION__);
	isChangeUpBank = true;
	CCLOG("endTime--------------:%s   timeLeave:%d <<%s>>", Tools::get_date_now().c_str(), pGameEnd->cbTimeLeave, __FUNCTION__);
	//设置时间
	resetTimer(pGameEnd->cbTimeLeave, BaseAttributes::sharedAttributes()->sGameEnd.c_str());
	hideTimer(false);
	//设置总结算
	lUserScore = pGameEnd->lUserScore;
	lBankerScore = pGameEnd->lBankerScore;

	


	//设置牌数据
	for (int i = 0; i < sizeof(pGameEnd->cbTableCardArray) / sizeof(pGameEnd->cbTableCardArray[0]); i++)
	{
		for (int j = 0; j < sizeof(pGameEnd->cbTableCardArray[0]); j++)
		{
			getMainScene()->cardLayer->card[i][j] = pGameEnd->cbTableCardArray[i][j];
		}
	}
	//推断赢家
	bool bWinTianMen, bWinDiMen, bWinXuanMen, bWinHuang;
	BYTE TianMultiple, diMultiple, TianXuanltiple, HuangMultiple;
	deduceWinner(bWinTianMen, bWinDiMen, bWinXuanMen, bWinHuang, TianMultiple, diMultiple, TianXuanltiple, HuangMultiple);
	//m_GameClientView.SetGameHistory(pServerGameRecord->bWinShunMen, pServerGameRecord->bWinDuiMen, pServerGameRecord->bWinDaoMen,pServerGameRecord->bWinHuang);

	tagGameRecord tagRecord;
	tagRecord.bWinShunMen = bWinTianMen;
	tagRecord.bWinDuiMen = bWinDiMen;
	tagRecord.bWinDaoMen = bWinXuanMen;
	tagRecord.bWinHuang = bWinHuang;

	insertGameHistory(tagRecord);

	//设置发牌状态
	DataModel::sharedDataModel()->getMainSceneOxHundred()->setGameStateWithUpdate(MainSceneOxHundred::STATE_GAME_SEND_CARD);
	//操控更新
	updateButtonContron();
}
//////////////////////////////////////////////////////////////////////////
//用户信息
void GameControlOxHundred::onSubUserInfo(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GR_USER_ENTER://用户进入
		onSubUserEnter(pDataBuffer, wDataSize);
		break;
	case SUB_GR_USER_STATUS://用户状态
		onSubUserState(pDataBuffer, wDataSize);
		break;
	default:
		break;
	}
}
//用户进入
void GameControlOxHundred::onSubUserEnter(void * pDataBuffer, unsigned short wDataSize){
	//效验参数
	assert(wDataSize >= sizeof(tagMobileUserInfoHead));
	if (wDataSize < sizeof(tagMobileUserInfoHead)) return;
	//消息处理
	tagMobileUserInfoHead * pUserInfoHead = (tagMobileUserInfoHead *)pDataBuffer;
	int wPacketSize = 0;
	//变量定义
	tagUserInfo UserInfo;
	UserInfo.dwUserID = pUserInfoHead->dwUserID;
	UserInfo.dwGameID = pUserInfoHead->dwGameID;
	UserInfo.lScore = pUserInfoHead->lScore;
	UserInfo.wChairID = pUserInfoHead->wChairID;
	UserInfo.wTableID = pUserInfoHead->wTableID;
	UserInfo.wFaceID = pUserInfoHead->wFaceID;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET + sizeof(TCP_Head)];
	CopyMemory(cbDataBuffer, pDataBuffer, wDataSize);

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
			CCLOG("社团");
		}
		break;
		case DTP_GR_UNDER_WRITE:
		{
			CopyMemory(UserInfo.szUnderWrite, cbDataBuffer + wPacketSize, DataDescribe->wDataSize);
			UserInfo.szUnderWrite[CountArray(UserInfo.szUnderWrite) - 1] = 0;
			CCLOG("签名:%s", Tools::GBKToUTF8(UserInfo.szUnderWrite).c_str());
		}
		break;
		}
		wPacketSize += DataDescribe->wDataSize;
		if (wPacketSize >= wDataSize)
		{
			break;
		}
	}
	map<long, tagUserInfo >::iterator l_it;
	l_it = DataModel::sharedDataModel()->mTagUserInfo.find(pUserInfoHead->dwUserID);
	if (l_it != DataModel::sharedDataModel()->mTagUserInfo.end())
	{
		l_it->second = UserInfo;
	}
	else
	{
		DataModel::sharedDataModel()->mTagUserInfo.insert(map<long, tagUserInfo>::value_type(pUserInfoHead->dwUserID, UserInfo));
	}


	/*
	//效验参数
	ASSERT(wDataSize>=sizeof(tagUserInfoHead));
	if (wDataSize<sizeof(tagUserInfoHead)) return false;

	//变量定义
	tagUserInfo UserInfo;
	tagCustomFaceInfo CustomFaceInfo;
	ZeroMemory(&UserInfo,sizeof(UserInfo));
	ZeroMemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

	//消息处理
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)pData;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	bool bHideUserInfo=CServerRule::IsAllowAvertCheatMode(m_dwServerRule);
	bool bMySelfUserItem=pGlobalUserData->dwUserID==pUserInfoHead->dwUserID;
	bool bMasterUserOrder=((pUserInfoHead->cbMasterOrder>0)||((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetMasterOrder()>0)));

	//读取信息
	if ((bHideUserInfo==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	{
	//用户属性
	UserInfo.wFaceID=pUserInfoHead->wFaceID;
	UserInfo.dwGameID=pUserInfoHead->dwGameID;
	UserInfo.dwUserID=pUserInfoHead->dwUserID;
	UserInfo.dwGroupID=pUserInfoHead->dwGroupID;
	UserInfo.dwCustomID=pUserInfoHead->dwCustomID;

	//用户状态
	UserInfo.wTableID=pUserInfoHead->wTableID;
	UserInfo.wChairID=pUserInfoHead->wChairID;
	UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

	//用户属性
	UserInfo.cbGender=pUserInfoHead->cbGender;
	UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
	UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;

	//用户积分
	UserInfo.lScore=pUserInfoHead->lScore;
	UserInfo.lGrade=pUserInfoHead->lGrade;
	UserInfo.lInsure=pUserInfoHead->lInsure;
	UserInfo.dwWinCount=pUserInfoHead->dwWinCount;
	UserInfo.dwLostCount=pUserInfoHead->dwLostCount;
	UserInfo.dwDrawCount=pUserInfoHead->dwDrawCount;
	UserInfo.dwFleeCount=pUserInfoHead->dwFleeCount;
	UserInfo.dwUserMedal=pUserInfoHead->dwUserMedal;
	UserInfo.dwExperience=pUserInfoHead->dwExperience;
	UserInfo.lLoveLiness=pUserInfoHead->lLoveLiness;

	//变量定义
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pUserInfoHead+1,wDataSize-sizeof(tagUserInfoHead));

	//扩展信息
	while (true)
	{
	pDataBuffer=RecvPacket.GetData(DataDescribe);
	if (DataDescribe.wDataDescribe==DTP_NULL) break;
	switch (DataDescribe.wDataDescribe)
	{
	case DTP_GR_NICK_NAME:		//用户昵称
	{
	ASSERT(pDataBuffer!=NULL);
	ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szNickName));
	if (DataDescribe.wDataSize<=sizeof(UserInfo.szNickName))
	{
	CopyMemory(&UserInfo.szNickName,pDataBuffer,DataDescribe.wDataSize);
	UserInfo.szNickName[CountArray(UserInfo.szNickName)-1]=0;
	}
	break;
	}
	case DTP_GR_GROUP_NAME:		//用户社团
	{
	ASSERT(pDataBuffer!=NULL);
	ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szGroupName));
	if (DataDescribe.wDataSize<=sizeof(UserInfo.szGroupName))
	{
	CopyMemory(&UserInfo.szGroupName,pDataBuffer,DataDescribe.wDataSize);
	UserInfo.szGroupName[CountArray(UserInfo.szGroupName)-1]=0;
	}
	break;
	}
	case DTP_GR_UNDER_WRITE:	//个性签名
	{
	ASSERT(pDataBuffer!=NULL);
	ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szUnderWrite));
	if (DataDescribe.wDataSize<=sizeof(UserInfo.szUnderWrite))
	{
	CopyMemory(UserInfo.szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
	UserInfo.szUnderWrite[CountArray(UserInfo.szUnderWrite)-1]=0;
	}
	break;
	}
	}
	}

	//自定头像
	if (pUserInfoHead->dwCustomID!=0L)
	{
	//加载头像
	CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
	bool bSuccess=pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID,CustomFaceInfo);

	//下载头像
	if (bSuccess==false)
	{
	pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID);
	}
	}
	}
	else
	{
	//用户信息
	UserInfo.dwUserID=pUserInfoHead->dwUserID;
	lstrcpyn(UserInfo.szNickName,TEXT("游戏玩家"),CountArray(UserInfo.szNickName));

	//用户状态
	UserInfo.wTableID=pUserInfoHead->wTableID;
	UserInfo.wChairID=pUserInfoHead->wChairID;
	UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

	//用户属性
	UserInfo.cbGender=pUserInfoHead->cbGender;
	UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
	UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;
	}
	//if (pUserInfoHead->wTableID !=INVALID_TABLE)
	//{
	//	CString sUserInfo;
	//	sUserInfo.Format("%s | %d |  %d",UserInfo.szNickName,pUserInfoHead->wTableID,pUserInfoHead->wChairID);
	//	m_ChatMessage.InsertSystemString(sUserInfo);
	//}

	//激活用户
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(UserInfo.dwUserID);
	if (pIClientUserItem==NULL)
	{
	pIClientUserItem=m_PlazaUserManagerModule->ActiveUserItem(UserInfo,CustomFaceInfo);
	//m_ChatMessage.InsertSystemString(UserInfo.szNickName);
	}
	else
	{		//删除用户

	m_ChatMessage.InsertSystemString(TEXT("ss1111111111111"));
	m_PlazaUserManagerModule->DeleteUserItem(pIClientUserItem);
	pIClientUserItem=m_PlazaUserManagerModule->ActiveUserItem(UserInfo,CustomFaceInfo);
	}

	//获取对象
	CServerListData * pServerListData=CServerListData::GetInstance();

	//人数更新
	pServerListData->SetServerOnLineCount(m_GameServer.dwServerID,m_PlazaUserManagerModule->GetActiveUserCount());

	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//好友提示
	if (((bHideUserInfo==false)&&(bMySelfUserItem==false))||(bMasterUserOrder==true))
	{
	if(pParameterGlobal->m_bNotifyFriendCome && pIClientUserItem->GetUserCompanion()==CP_FRIEND)
	{
	//提示消息
	CString strDescribe;
	strDescribe.Format(TEXT("您的好友 [%s] 进来了！"),pIClientUserItem->GetNickName());
	m_ChatMessage.InsertSystemString(strDescribe);
	//m_ChatMessage.InsertUserEnter(pIClientUserItem->GetNickName());//InsertNormalString(strDescribe);
	}
	}
	*/
}
//用户状态
void GameControlOxHundred::onSubUserState(void * pDataBuffer, unsigned short wDataSize){
	CMD_GR_UserStatus *info = (CMD_GR_UserStatus*)pDataBuffer;
	switch (info->UserStatus.cbUserStatus)
	{
	case US_SIT://坐下
	{
		if (info->dwUserID == DataModel::sharedDataModel()->userInfo->dwUserID){
			DataModel::sharedDataModel()->userInfo->wTableID = info->UserStatus.wTableID;
			DataModel::sharedDataModel()->userInfo->wChairID = info->UserStatus.wChairID;
			CCLOG("--%s------------------<<%s>>", Tools::GBKToUTF8("百人牛牛坐下").c_str(), __FUNCTION__);
			pIUpBank->setEnabled(true);
		}
	}
	break;
	case US_FREE://站立
	{
		map<long, tagUserInfo >::iterator l_it;
		l_it = DataModel::sharedDataModel()->mTagUserInfo.find(info->dwUserID);
		if (l_it != DataModel::sharedDataModel()->mTagUserInfo.end())
		{
			DataModel::sharedDataModel()->mTagUserInfo.erase(info->dwUserID);
		}
		/*std::vector<long> tVecRemove;
		std::map<long,tagUserInfo>::iterator iter;
		for (iter = DataModel::sharedDataModel()->mTagUserInfo.begin(); iter != DataModel::sharedDataModel()->mTagUserInfo.end(); iter++)
		{
		if (info->dwUserID==iter->second.dwUserID)
		{
		tVecRemove.push_back(info->dwUserID);
		}
		}
		for (int i = 0; i < tVecRemove.size(); i++)
		{
		DataModel::sharedDataModel()->mTagUserInfo.erase(tVecRemove[i]);
		}
		CCLOG("tagSize:%d<<%s>>",DataModel::sharedDataModel()->mTagUserInfo.size(),__FUNCTION__);*/
		/*if (info->dwUserID==4947)
		{
		CCLOG("-----------------------<<%s>>",__FUNCTION__);
		}*/
		if (info->dwUserID == DataModel::sharedDataModel()->userInfo->dwUserID)
		{
			GameIngMsgHandler::sharedGameIngMsgHandler()->gameSocket.Destroy(true);
			//TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_LOGON_ROOM);
			Tools::setTransitionAnimation(0, 0, GameLobbyScene::scene(false,""));
			//MTNotificationQueue::sharedNotificationQueue()->postNotification(S_L_US_FREE,NULL);
		}
		else
		{

		}
	}
	break;
	case US_READY://同意
	{
	}
	break;
	case US_PLAYING:
	{
	}
	break;
	default:
		CCLOG("state==Other userID:%ld 状态：%d<<%s>>", info->dwUserID, info->UserStatus.cbUserStatus, __FUNCTION__);
		break;
	}
}
//用户状态
void GameControlOxHundred::onSubUserState(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
}
//用户坐下状态
void GameControlOxHundred::onSubUserSitState(void * pDataBuffer, unsigned short wDataSize){
	assert(wDataSize >= sizeof(CMD_C_UserSit));
	CMD_C_UserSit *pUserSit = (CMD_C_UserSit*)pDataBuffer;
	
	map<long, tagUserInfo >::iterator l_it;
	l_it = DataModel::sharedDataModel()->mTagUserInfo.find(pUserSit->wUserID);
	if (l_it != DataModel::sharedDataModel()->mTagUserInfo.end())
	{
		pChairData[pUserSit->cbSitID]->setUserSitState(l_it->second, true);
	}
}
//用户起立状态
void GameControlOxHundred::onSubUserUpState(void * pDataBuffer, unsigned short wDataSize){
	assert(wDataSize >= sizeof(CMD_C_UserSit));
	CMD_C_UserSit *pUserSit = (CMD_C_UserSit*)pDataBuffer;

	map<long, tagUserInfo >::iterator l_it;
	l_it = DataModel::sharedDataModel()->mTagUserInfo.find(pUserSit->wUserID);
	if (l_it != DataModel::sharedDataModel()->mTagUserInfo.end())
	{
		pChairData[pUserSit->cbSitID]->setUserSitState(l_it->second, false);
	}
}

//开始下注效果动画
void GameControlOxHundred::beginAddScoreEffect(){
	CCArmature *pAnimate = CCArmature::create("AnimationGameIng");
	this->addChild(pAnimate);
	CCSize dSize = DataModel::sharedDataModel()->deviceSize;
	pAnimate->setPosition(ccp(dSize.width/2,dSize.height/2-100));

	pAnimate->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(GameControlOxHundred::onAnimationEventOver));//动画播完回调用
	pAnimate->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(GameControlOxHundred::onAnimationEventFrame));
	
	pAnimate->getAnimation()->play("addScore");
}
void GameControlOxHundred::onAnimationEventOver(CCArmature *pArmature, MovementEventType movementType, const char *movementID){
	switch (movementType)
	{
	case cocos2d::extension::COMPLETE:
	case cocos2d::extension::LOOP_COMPLETE:
	{
		showTimer();
		pArmature->removeFromParentAndCleanup(true);
	}
	break;
	default:
		break;
	}
}
void GameControlOxHundred::onAnimationEventFrame(CCBone *bone, const char *evt, int originFrameIndex, int currentFrameIndex){
	if (strcmp(evt, "bomb1") == 0)
	{
	
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//分发筹码(庄家收)
void GameControlOxHundred::bankJettonIn(float dt){
	if (listGameRecord.empty())
	{
		return;
	}
	tagGameRecord gameRecord =listGameRecord.back();
	
	bool bankAllLost = true;
	bool bankAllWin = true;
	if (!gameRecord.bWinShunMen)
	{
		bankAllLost = false;
		moveJettonBankIn(0, posBankJetton);
	}
	else
	{
		bankAllWin = false;
	}
	//////////////////////////////////////////////////////////////////////////
	if (!gameRecord.bWinDuiMen)
	{
		bankAllLost = false;
		moveJettonBankIn(1, posBankJetton);
	}
	else
	{
		bankAllWin = false;
	}
	//////////////////////////////////////////////////////////////////////////
	if (!gameRecord.bWinDaoMen)
	{
		bankAllLost = false;
		moveJettonBankIn(2, posBankJetton);
	}
	else
	{
		bankAllWin = false;
	}
	//////////////////////////////////////////////////////////////////////////
	if (!gameRecord.bWinHuang)
	{
		bankAllLost = false;
		moveJettonBankIn(3, posBankJetton);
	}
	else
	{
		bankAllWin = false;
	}
	//庄家全输直接出币
	if (bankAllLost)
	{
		bankOutJetton();
	}
	//庄家全赢结束
	if (bankAllWin)
	{
		splitJettonFinish();
	}
}
//移动相同筹码(庄家收)
void GameControlOxHundred::moveJettonBankIn(int chairID, CCPoint pos){
	bool notJetton = true;//没有币
	for (int i = 0; i < DataModel::sharedDataModel()->vecJettonNode.size(); i++)
	{
		JettonNode *tempJetton = DataModel::sharedDataModel()->vecJettonNode[i];
		if (!tempJetton->isReuse)
		{
			if (tempJetton->iBetArea==chairID)
			{
				tempJetton->moveJettonBankIn(pos);
				notJetton = false;
			}
		}
	}
	if (notJetton)
	{
		bankOutJetton();
	}
}
//庄家出币
void GameControlOxHundred::bankOutJetton(){
	if (listGameRecord.empty())
	{
		return;
	}
	tagGameRecord gameRecord = listGameRecord.back();
	//////////////////////////////////////////////////////////////////////////
	if (gameRecord.bWinShunMen)
	{
		moveJettonBankOut(0);
	}
	//////////////////////////////////////////////////////////////////////////
	if (gameRecord.bWinDuiMen)
	{
		moveJettonBankOut(1);
	}
	//////////////////////////////////////////////////////////////////////////
	if (gameRecord.bWinDaoMen)
	{
		moveJettonBankOut(2);
	}
	//////////////////////////////////////////////////////////////////////////
	if (gameRecord.bWinHuang)
	{
		moveJettonBankOut(3);
	}
}
//庄家出
void GameControlOxHundred::moveJettonBankOut(int chairID){
	CCArray *arrTemp = CCArray::create();
	arrTemp->retain();
	for (int i = 0; i < DataModel::sharedDataModel()->vecJettonNode.size(); i++)
	{
		JettonNode *tempJetton = DataModel::sharedDataModel()->vecJettonNode[i];
		if (!tempJetton->isReuse)
		{
			if (tempJetton->iBetArea == chairID)
			{
				arrTemp->addObject(tempJetton);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	CCObject *object;
	CCARRAY_FOREACH(arrTemp, object){
		JettonNode *pTempJettonNode = (JettonNode*)object;
		int fWidth = pSeatData[chairID]->seatSize.width - 31;
		int fHeight = pSeatData[chairID]->seatSize.height - 70 - 31;

		int offsetX = rand() % fWidth;
		int offsetY = rand() % fHeight;
		CCPoint randPos = ccp(offsetX, offsetY);
		CCPoint pos = pSeatData[chairID]->posCenter;
		pos = ccpAdd(pos, randPos);
		pos = ccpSub(pos, ccp(fWidth / 2, fHeight / 2));

		JettonNode *pJetton = getJettonNode();
		pJetton->iBetArea = chairID;
		pJetton->bankOutJetton(pTempJettonNode->llJettonScore, posBankJetton, pos);
	}
	arrTemp->release();
}
//玩家收币
void GameControlOxHundred::playerInJetton(){
	if (listGameRecord.empty())
	{
		return;
	}
	tagGameRecord gameRecord = listGameRecord.back();
	//////////////////////////////////////////////////////////////////////////
	if (gameRecord.bWinShunMen)
	{
		moveJettonPlayerIn(0);
	}
	//////////////////////////////////////////////////////////////////////////
	if (gameRecord.bWinDuiMen)
	{
		moveJettonPlayerIn(1);
	}
	//////////////////////////////////////////////////////////////////////////
	if (gameRecord.bWinDaoMen)
	{
		moveJettonPlayerIn(2);
	}
	//////////////////////////////////////////////////////////////////////////
	if (gameRecord.bWinHuang)
	{
		moveJettonPlayerIn(3);
	}
}
//玩家收
void GameControlOxHundred::moveJettonPlayerIn(int chairID){
	for (int i = 0; i < DataModel::sharedDataModel()->vecJettonNode.size(); i++)
	{
		JettonNode *tempJetton = DataModel::sharedDataModel()->vecJettonNode[i];
		if (!tempJetton->isReuse)
		{
			if (tempJetton->iBetArea == chairID)
			{
				tempJetton->moveJettonPlayerIn(posPlayerJetton);
			}
		}
	}
	
}
//分币完成
void GameControlOxHundred::splitJettonFinish(){
	pPlayerData[0]->lGameScore = lUserScore;
	DataModel::sharedDataModel()->userInfo->lScore += lUserScore;
	pPlayerData[0]->changePlayerGold(DataModel::sharedDataModel()->userInfo->lScore);


	pPlayerData[1]->lGameScore = lBankerScore;
	tagUserInfo *uInfo = getUserInfo(m_wBankerUser);
	if (uInfo)
	{
		uInfo->lScore += lBankerScore;
		pPlayerData[1]->changePlayerGold(uInfo->lScore);
	}
	//显示结算
	showAllResult();
	//刷新在线玩家信息
	if (getParent()->getChildByTag(TAG_ONLINE))
	{
		map<long, tagUserInfo >::iterator l_it;
		l_it = DataModel::sharedDataModel()->mTagUserInfo.find(DataModel::sharedDataModel()->userInfo->dwUserID);
		if (l_it != DataModel::sharedDataModel()->mTagUserInfo.end())
		{
			l_it->second = *DataModel::sharedDataModel()->userInfo;
		}


		PopDialogBoxOnLine *pOnLine = (PopDialogBoxOnLine*)getParent()->getChildByTag(TAG_ONLINE);
		pOnLine->updateOnLineList();
	}
}