//
//
//
//

#include "GameControlOxHundred.h"
#include "GameConfig.h"
#include "Tools.h"
#include "GameLobbyScene.h"
#include "DataModel.h"
#include "cmd_game.h"
#include "MainSceneBase.h"
#include "PacketAide.h"
#include "PopDialogBoxUpBank.h"
GameControlOxHundred::GameControlOxHundred()
:iCurSelectJettonIndex(0)
,m_lMeMaxScore(0)
{
	nJetton[0]=1000;
	nJetton[1]=5000;
	nJetton[2]=10000;
	nJetton[3]=100000;
	nJetton[4]=500000;
	resetData();
}
GameControlOxHundred::~GameControlOxHundred(){
	TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_LOGON_ROOM);
	DataModel::sharedDataModel()->	vecJettonNode.clear();
}
void GameControlOxHundred::onEnter(){
	CCLayer::onEnter();
	UILayer *pWidget = UILayer::create();
	this->addChild(pWidget);

	UILayout *pLayout = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIGameIngHundred.ExportJson)));
	pWidget->addWidget(pLayout);

	UIButton* button = static_cast<UIButton*>(pWidget->getWidgetByName("ButtonBack"));
	button->addTouchEventListener(this, SEL_TouchEvent(&GameControlOxHundred::onMenuBack));

	pBOnline= static_cast<UIButton*>(pWidget->getWidgetByName("ButtonOnline"));
	//筹码按钮
	for (int i = 0; i < MAX_JETTON_BUTTON_COUNT; i++)
	{
		pIJettonButton[i]=static_cast<UIImageView*>(pWidget->getWidgetByName(CCString::createWithFormat("ImageJetton%d",i)->getCString()));
		pIJettonButton[i]->addTouchEventListener(this, SEL_TouchEvent(&GameControlOxHundred::onMenuSelectJetton));
		//隐藏
		pIJettonButton[i]->setColor(ccc3(100,100,100));
		pIJettonButton[i]->setTouchEnabled(false);
	}
	//筹码选择光标
	pIJettonSelect=static_cast<UIImageView*>(pWidget->getWidgetByName("ImageSelectJetton"));
	pIJettonSelect->getChildByName("ImageSelectJettonBg")->runAction(CCRepeatForever::create(CCRotateBy::create(0.8,360)));
	pIJettonSelect->setVisible(false);

	//庄家牌背景
	UIImageView *pIBankCardBg=static_cast<UIImageView*>(pWidget->getWidgetByName("ImageBankCardBg"));
	getMainScene()->posChair[0]=ccpAdd(pIBankCardBg->getPosition(),ccp(0,-pIBankCardBg->getContentSize().height/2));
	//用户数据
	for (int i = 0; i < MAX_PLAYER_HUNDRED_COUNT; i++)
	{
		pPlayerData[i]=PlayerDataHundred::create();
		this->addChild(pPlayerData[i]);
		pPlayerData[i]->pIPlayerBg=static_cast<UIImageView*>(pWidget->getWidgetByName(CCString::createWithFormat("ImageIcon%d",i)->getCString()));
		//结算数字
		pPlayerData[i]->pLResult=static_cast<UILabelAtlas*>(pPlayerData[i]->pIPlayerBg->getChildByName("AtlasLabelResult"));
		pPlayerData[i]->pLResult->setVisible(false);
	}
	//上庄按键
	pIUpBank=static_cast<UIImageView*>(pWidget->getWidgetByName("ImageUpBank"));
	pIUpBank->addTouchEventListener(this, SEL_TouchEvent(&GameControlOxHundred::onMenuUpBank));
	CCSprite *pSUpBank=CCSprite::create();
	CCArray *animFrames=CCArray::create();

	CCSpriteFrame *pSFrame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("u_gih_add_score.png");
	animFrames->addObject(pSFrame);
	CCSpriteFrame *pSFrame1=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("u_gih_add_score_pre.png");
	animFrames->addObject(pSFrame1);

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames,0.1f); 
	animation->setLoops(-1); 
	CCAnimate *action=CCAnimate::create(animation); 
	pSUpBank->runAction(action);

	pIUpBank->addNode(pSUpBank);

	initTimer(pWidget);
	initSeatData(pWidget);
}
void GameControlOxHundred::onExit(){
	CCLayer::onExit();
}
void GameControlOxHundred::resetData(){
	//个人下注
	memset(m_lUserJettonScore,0,sizeof(m_lUserJettonScore));
	//全体下注
	memset(m_lAllJettonScore,0,sizeof(m_lAllJettonScore));
	//庄家信息
	m_wBankerUser=INVALID_CHAIR;		
	//m_wBankerTime=0;
	m_lBankerScore=0L;	
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
MainSceneBase*GameControlOxHundred::getMainScene(){
	return (MainSceneBase*)this->getParent();
}
//初始化计时器
void GameControlOxHundred::initTimer(UILayer *pWidget){
	pITimer = static_cast<UIImageView*>(pWidget->getWidgetByName("ImageTimer"));
	pITimer->setVisible(false);

	pLTimerNum=static_cast<UILabelAtlas*>(pWidget->getWidgetByName("AtlasLabelTimer"));
	iTimerCount=-1;

	pLTimerPromptContent=static_cast<UILabel*>(pWidget->getWidgetByName("LabelPromptContent"));
}
//初始化座位
void GameControlOxHundred::initSeatData(UILayer *pWidget){
	for (int i = 0; i < MAX_SEAT_COUNT; i++)
	{
		pSeatData[i]=SeatData::create();
		this->addChild(pSeatData[i]);
		UIImageView *bg=static_cast<UIImageView*>(pWidget->getWidgetByName(CCString::createWithFormat("ImageSeatBg%d",i)->getCString()));
		bg->addTouchEventListener(this, SEL_TouchEvent(&GameControlOxHundred::onMenuPlaceJetton));
		//设置中心点
		pSeatData[i]->posCenter=bg->getPosition();
		getMainScene()->posChair[i+1]=ccpAdd(bg->getPosition(),ccp(0,-bg->getContentSize().height/2-50));
		//设置座位大小
		pSeatData[i]->seatSize=bg->getContentSize();
		//所有筹码
		pSeatData[i]->pLAllJetton=static_cast<UILabelAtlas*>(bg->getChildByName("AtlasLabelAllScore"));
		pSeatData[i]->pIFontWan=static_cast<UIImageView*>(bg->getChildByName("AtlasLabelAllScore")->getChildByName("ImageFontWan"));
		//用户筹码
		pSeatData[i]->pLUserJetton=static_cast<UILabelAtlas*>(bg->getChildByName("ImageB")->getChildByName("AtlasLabelAddScore"));
		pSeatData[i]->pIUserFontWan=static_cast<UIImageView*>(bg->getChildByName("ImageB")->getChildByName("AtlasLabelAddScore")->getChildByName("ImageFontWan1"));
		//未下注
		pSeatData[i]->pINotAddJetton=static_cast<UIImageView*>(bg->getChildByName("ImageNotAdd"));
		//结算数字
		pSeatData[i]->pLResult=static_cast<UILabelAtlas*>(bg->getChildByName("AtlasLabelResultNum"));

		pSeatData[i]->resetData();
	}
}
//设置座位结算
void GameControlOxHundred::setSeatResult(int iSeatIndex,int iOXType){
	int oxType=iOXType;
	if (oxType==0)
	{
		oxType==1;
	}
	long long lMeJetton=m_lUserJettonScore[iSeatIndex]*oxType;
	pSeatData[iSeatIndex-1]->setResult(lMeJetton);
}
//显示总结果
void GameControlOxHundred::showAllResult(){
	for (int i = 0; i < MAX_PLAYER_HUNDRED_COUNT; i++)
	{
		pPlayerData[i]->showResultAnimation();
	}
}
//获得筹码对象
JettonNode *GameControlOxHundred::getJettonNode(){
	for (int i = 0; i < DataModel::sharedDataModel()->vecJettonNode.size(); i++)
	{
		JettonNode *tempJetton=DataModel::sharedDataModel()->vecJettonNode[i];
		if (tempJetton->isReuse)
		{
			tempJetton->resetData();
			return tempJetton;
		}
	}
	JettonNode *pJetton=JettonNode::create();
	this->addChild(pJetton,1000);
	DataModel::sharedDataModel()->vecJettonNode.push_back(pJetton);
	return pJetton;
}
int GameControlOxHundred::getChairIndex(int meChairID,int chairID){
	if (meChairID==0)
	{
		if (chairID==0)
		{
			return 3;
		}
	}else
	{
		if (chairID==1)
		{
			return 3;
		}
	}
	return 0;
}
//菜单回调（返回）
void GameControlOxHundred::onMenuBack(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_LOGON_ROOM);
			Tools::setTransitionAnimation(0, 0, GameLobbyScene::scene());
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
			if (iCurSelectJettonIndex==-1||
				DataModel::sharedDataModel()->getMainSceneOxHundred()->getGameState()!=MainSceneOxHundred::STATE_GAME_PLACE_JETTON)
			{
				CCLog("nonononon<<%s>>",__FUNCTION__);
				return;
			}
			UIImageView *pIButton=(UIImageView*)pSender;
			//变量定义
			CMD_C_PlaceJetton PlaceJetton;
			memset(&PlaceJetton,0,sizeof(PlaceJetton));
			//构造变量
			PlaceJetton.cbJettonArea=pIButton->getTag();
			PlaceJetton.lJettonScore=nJetton[iCurSelectJettonIndex];
			//个人下注
			m_lUserJettonScore[PlaceJetton.cbJettonArea]+=PlaceJetton.lJettonScore;
			m_lAllJettonScore[PlaceJetton.cbJettonArea] += PlaceJetton.lJettonScore;

			pSeatData[PlaceJetton.cbJettonArea-1]->setUserJetton(m_lUserJettonScore[PlaceJetton.cbJettonArea]);


			//发送消息
			TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_LOGON_ROOM)->SendData(MDM_GF_GAME,SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
					
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
			UIImageView *pIButton=(UIImageView*)pSender;
			pIJettonSelect->setPosition(pIButton->getPosition());
			iCurSelectJettonIndex=pIButton->getTag()-1;
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
			PopDialogBoxUpBank *pDBUpBank=PopDialogBoxUpBank::create();
			getParent()->addChild(pDBUpBank,K_Z_ORDER_POP);
			CCLog("upbank---<<%s>>",__FUNCTION__);
		}
		break;
	default:
		break;
	}
}
void GameControlOxHundred::update(float delta){
	MessageQueue::update(delta);
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
	assert(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)lNowJetton += m_lUserJettonScore[nAreaIndex]*iTimer;

	//庄家金币
	long long lBankerScore=2147483647;
	//if (m_wBankerUser!=INVALID_CHAIR) 
	lBankerScore=m_lBankerScore;
	CCLog("=lBankerScore:%lld    %lld<<%s>>",lBankerScore,m_lBankerScore,__FUNCTION__);
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
	{
		lBankerScore-=m_lAllJettonScore[nAreaIndex]*iTimer;
		CCLog("%d ---------%lld          <<%s>>",nAreaIndex,m_lAllJettonScore[nAreaIndex],__FUNCTION__);
	}
	CCLog("===lBankerScore:%lld<<%s>>",lBankerScore,__FUNCTION__);
	//区域限制
	long long lMeMaxScore=0;
	if((m_lMeMaxScore-lNowJetton)/iTimer>m_lAreaLimitScore)
	{
		lMeMaxScore= m_lAreaLimitScore*iTimer;

	}else
	{
		lMeMaxScore = m_lMeMaxScore-lNowJetton;
		lMeMaxScore = lMeMaxScore;
	}

	//庄家限制
	lMeMaxScore=MIN(lMeMaxScore,lBankerScore);

	lMeMaxScore /= iTimer; 

	//非零限制
	//ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = MAX(lMeMaxScore, 0);
	
	return lMeMaxScore;
}
//更新控制
void GameControlOxHundred::updateButtonContron(){
	bool bEnablePlaceJetton=true; 
	switch (DataModel::sharedDataModel()->getMainSceneOxHundred()->getGameState())
	{
	case MainSceneOxHundred::STATE_GAME_FREE:
		{
				bEnablePlaceJetton=false;
		}
		break;
	case MainSceneOxHundred::STATE_GAME_PLACE_JETTON:
		break;
	case MainSceneOxHundred::STATE_GAME_SEND_CARD:
		{
			bEnablePlaceJetton=false;
		}
		break;
	default:
		break;
	}
	//////////////////////////////////////////////////////////////////////////
	if (bEnablePlaceJetton)
	{
		for (int i = 0; i < MAX_JETTON_BUTTON_COUNT; i++)
		{
			pIJettonButton[i]->setColor(ccc3(255,255,255));
			pIJettonButton[i]->setTouchEnabled(true);
			pIJettonButton[0]->setVisible(true);
		}
		if (iCurSelectJettonIndex<0)
		{
			iCurSelectJettonIndex=0;
		}
		pIJettonSelect->setVisible(true);
		//////////////////////////////////////////////////////////////////////////
		//计算积分
		long long lCurrentJetton=nJetton[iCurSelectJettonIndex];//当前筹码
		long long lLeaveScore=m_lMeMaxScore;//我的金币
		for (int nAreaIndex=1; nAreaIndex<=MAX_AREA_COUNT; ++nAreaIndex) lLeaveScore -= m_lUserJettonScore[nAreaIndex];
		
		//最大下注
		long long lUserMaxJetton=getUserMaxJetton();
		CCLog("---最大下注:%lld<<%s>>",lUserMaxJetton,__FUNCTION__);
		//设置光标
		lLeaveScore = MIN((lLeaveScore/10),lUserMaxJetton); //用户可下分 和最大分比较 由于是五倍 
		//CCLog("---lLeaveScore:%lld<<%s>>",lLeaveScore,__FUNCTION__);
		if (lCurrentJetton>lLeaveScore)
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
			if (lLeaveScore<nJetton[i]*iTimer || lUserMaxJetton<nJetton[i]*iTimer)
			{
				pIJettonButton[i]->setColor(ccc3(100,100,100));
				pIJettonButton[i]->setTouchEnabled(false);
				if (iCurSelectJettonIndex==i)
				{
					iCurSelectJettonIndex--;
					if (iCurSelectJettonIndex>=0)
					{
						pIJettonSelect->setPosition(pIJettonButton[iCurSelectJettonIndex]->getPosition());
					}else
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
		CCLog("%lld    %lld<<%s>>",lLeaveScore,lUserMaxJetton,__FUNCTION__);*/
		/*m_GameClientView.m_btJetton100.EnableWindow((lLeaveScore>=100*iTimer && lUserMaxJetton>=100*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000.EnableWindow((lLeaveScore>=1000*iTimer && lUserMaxJetton>=1000*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow((lLeaveScore>=10000*iTimer && lUserMaxJetton>=10000*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton50000.EnableWindow((lLeaveScore>=50000*iTimer && lUserMaxJetton>=50000*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton100000.EnableWindow((lLeaveScore>=100000*iTimer && lUserMaxJetton>=100000*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton500000.EnableWindow((lLeaveScore>=500000*iTimer && lUserMaxJetton>=500000*iTimer)?TRUE:FALSE);		
		m_GameClientView.m_btJetton1000000.EnableWindow((lLeaveScore>=1000000*iTimer && lUserMaxJetton>=1000000*iTimer)?TRUE:FALSE);		
		m_GameClientView.m_btJetton5000000.EnableWindow((lLeaveScore>=5000000*iTimer && lUserMaxJetton>=5000000*iTimer)?TRUE:FALSE);
		*/


	}else
	{
		for (int i = 0; i < MAX_JETTON_BUTTON_COUNT; i++)
		{
			pIJettonButton[i]->setColor(ccc3(100,100,100));
			pIJettonButton[i]->setTouchEnabled(false);
		}
		pIJettonSelect->setVisible(false);
	}
}
//更新状态
void GameControlOxHundred::updateState(){
	switch (DataModel::sharedDataModel()->getMainSceneOxHundred()->getGameState())
	{
			case MainSceneOxHundred::STATE_GAME_SHOW_CARE_FINISH:
			{
				showAllResult();
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
void GameControlOxHundred::setBankerInfo(unsigned short  wBanker,long long lScore){
	//庄家椅子号
	WORD wBankerUser=INVALID_CHAIR;
	//切换判断
	if (m_wBankerUser!=wBankerUser)
	{
		m_wBankerUser=wBankerUser;
		//m_wBankerTime=0L;
		//m_lBankerWinScore=0L;	
		//m_lTmpBankerWinScore=0L;
	}
	m_lBankerScore=lScore;
}
void GameControlOxHundred::onEventReadMessage(WORD wMainCmdID,WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_GR_USER://用户信息
		onSubUserState(wSubCmdID,pDataBuffer,wDataSize);
		break;
	case MDM_GF_GAME://游戏命令
		onEventGameIng(wSubCmdID,pDataBuffer,wDataSize);
		break;
	case MDM_GF_FRAME://框架命令
		onSubGameFrame(wSubCmdID,pDataBuffer,wDataSize);
		break;
	default:
		CCLog("----------------main:%d sub:%d<<%s>>",wMainCmdID,wSubCmdID,__FUNCTION__);
		break;
	}
}
//框架命令
void GameControlOxHundred::onSubGameFrame(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GF_GAME_STATUS://游戏状态
		{
			//效验参数
			assert(wDataSize==sizeof(CMD_GF_GameStatus));
			if (wDataSize!=sizeof(CMD_GF_GameStatus)) return ;
			//消息处理
			CMD_GF_GameStatus * pGameStatus=(CMD_GF_GameStatus *)pDataBuffer;
			//设置变量
			m_cbGameStatus=pGameStatus->cbGameStatus;
			m_bAllowLookon=pGameStatus->cbAllowLookon?true:false;
		}
		break;
	case SUB_GF_GAME_SCENE://游戏场景
		{
			switch (m_cbGameStatus)
			{
			case GAME_SCENE_FREE:
				{
					int ss=sizeof(CMD_S_StatusFree);
					//效验数据
					assert(wDataSize==sizeof(CMD_S_StatusFree));
					if (wDataSize!=sizeof(CMD_S_StatusFree)) return ;

					//消息处理
					CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pDataBuffer;
					CCLog("GAME_SCENE_FREE<<%s>>",__FUNCTION__);
					m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
				}
				break;
			case GAME_SCENE_PLACE_JETTON:		//游戏状态
			case GAME_SCENE_GAME_END:		//结束状态
				{
					//效验数据
					assert(wDataSize==sizeof(CMD_S_StatusPlay));
					if (wDataSize!=sizeof(CMD_S_StatusPlay)) return ;

					//消息处理
					CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pDataBuffer;
					m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
					//下注信息
					for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
					{
						m_lAllJettonScore[nAreaIndex]+=pStatusPlay->lAllJettonScore[nAreaIndex];
						pSeatData[nAreaIndex-1]->setAllJettonByAdd(pStatusPlay->lAllJettonScore[nAreaIndex]);
					}
					if (pStatusPlay->cbGameStatus==GAME_SCENE_GAME_END)
					{

					}
					CCLog("GAME_SCENE_PLACE_JETTON|GAME_SCENE_GAME_END<<%s>>",__FUNCTION__);
				}
				break;
			default:
				break;
			}
			
		}
		break;
	case SUB_GF_SYSTEM_MESSAGE://系统消息
		CCLog("SUB_GF_SYSTEM_MESSAGE<<%s>>",__FUNCTION__);
		break;
	default:
		CCLog("-:%d<<%s>>",wSubCmdID,__FUNCTION__);
		break;
	}
}
void GameControlOxHundred::onEventGameIng(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE://游戏空闲
		onSubGameFree(pDataBuffer,wDataSize);
		break;
	case SUB_S_GAME_START://游戏开始
		onSubGameStart(pDataBuffer,wDataSize);
		break;
	case SUB_S_PLACE_JETTON://用户下注
		onSubPlaceJetton(pDataBuffer,wDataSize,true);
		break;
	case SUB_S_GAME_END://游戏结束
		onSubGameEnd(pDataBuffer,wDataSize);
		break;
	case SUB_S_APPLY_BANKER://申请庄家
		onSubUserApplyBanker(pDataBuffer,wDataSize);
		break;
	case SUB_S_CHANGE_BANKER://切换庄家
		break;
	case SUB_S_CHANGE_USER_SCORE://更新积分
		break;
	case SUB_S_SEND_RECORD://游戏记录
		break;
	case SUB_S_PLACE_JETTON_FAIL://下注失败
		onSubPlaceJettonFail(pDataBuffer,wDataSize);
		break;
	case SUB_S_CANCEL_BANKER://取消申请
		break;
	case SUB_S_SEND_ACCOUNT://发送账号
		break;
	case SUB_S_ADMIN_CHEAK://查询账号
		break;
	case SUB_S_QIANG_ZHUAN://抢庄
		break;
	case SUB_S_AMDIN_COMMAND://管理员命令
		break;
	default:
		CCLog("===========sub:%d<<%s>>",wSubCmdID,__FUNCTION__);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
//游戏空闲
void GameControlOxHundred::onSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	assert(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return ;
	//消息处理
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;
	//设置时间
	resetTimer(pGameFree->cbTimeLeave,Tools::GBKToUTF8("本轮即将开始"));
	CCLog("time:%d   count:%lld<<%s>>",pGameFree->cbTimeLeave,pGameFree->nListUserCount,__FUNCTION__);
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
	assert(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return;

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	CCLog("gameStart=time--:%d<<%s>>",pGameStart->cbTimeLeave,__FUNCTION__);
	//设置时间
	resetTimer(pGameStart->cbTimeLeave,Tools::GBKToUTF8("请下注"));
	//设置下注状态
	DataModel::sharedDataModel()->getMainSceneOxHundred()->setGameStateWithUpdate(MainSceneOxHundred::STATE_GAME_PLACE_JETTON);
	setBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);
	//操控更新
	updateButtonContron();
}
//用户下注
void GameControlOxHundred::onSubPlaceJetton(const void * pBuffer, WORD wDataSize,bool bGameMes){
	//效验数据
	assert(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return ;

	//消息处理
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;

	pSeatData[pPlaceJetton->cbJettonArea-1]->setAllJettonByAdd(pPlaceJetton->lJettonScore);
	
	CCPoint posBegin=pBOnline->getPosition();
	if (DataModel::sharedDataModel()->userInfo->wChairID!=pPlaceJetton->wChairID)
	{
		m_lAllJettonScore[pPlaceJetton->cbJettonArea] += pPlaceJetton->lJettonScore;
	}else
	{
		posBegin=pPlayerData[0]->pIPlayerBg->getPosition();
	}
	//////////////////////////////////////////////////////////////////////////
	

	//int fWidth=pSeatData[pPlaceJetton->cbJettonArea-1]->seatSize.width-jetton->getContentSize().width/2;
	//int fHeight=pSeatData[pPlaceJetton->cbJettonArea-1]->seatSize.height-jetton->getContentSize().height/2-70;
	int fWidth=pSeatData[pPlaceJetton->cbJettonArea-1]->seatSize.width-31;
	int fHeight=pSeatData[pPlaceJetton->cbJettonArea-1]->seatSize.height-70-31;

	int offsetX=rand()%fWidth;
	int offsetY=rand()%fHeight;
	CCPoint randPos=ccp(offsetX,offsetY);
	CCPoint pos=pSeatData[pPlaceJetton->cbJettonArea-1]->posCenter;
	pos=ccpAdd(pos,randPos);
	pos=ccpSub(pos,ccp(fWidth/2,fHeight/2));

	JettonNode *pJetton=getJettonNode();
	
	pJetton->setJettonTypeWithMove(pPlaceJetton->lJettonScore,posBegin,pos);
	CCLog("chair:%d jettonScore: %lld<<%s>>",pPlaceJetton->wChairID,pPlaceJetton->lJettonScore,__FUNCTION__);
	updateButtonContron();
}
//申请庄家
void GameControlOxHundred::onSubUserApplyBanker(const void * pBuffer, WORD wDataSize){
	//效验数据
	assert(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return ;

	//消息处理
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;
	CCLog("============================  %d<<%s>>",pApplyBanker->wApplyUser,__FUNCTION__);
}
//下注失败
void GameControlOxHundred::onSubPlaceJettonFail(const void * pBuffer, WORD wDataSize){
	CCLog("======================jettonFail------------<<%s>>",__FUNCTION__);
	//效验数据
	assert(wDataSize==sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize!=sizeof(CMD_S_PlaceJettonFail)) return;

	//消息处理
	CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;
	CCLog("%d<<%s>>",pPlaceJettonFail->wPlaceUser,__FUNCTION__);
	m_lAllJettonScore[pPlaceJettonFail->lJettonArea] -= pPlaceJettonFail->lPlaceScore;
	//自己判断
	if (DataModel::sharedDataModel()->userInfo->wChairID==pPlaceJettonFail->wPlaceUser 
		//&& false==IsLookonMode()
		)
	{
		//效验参数
		BYTE cbViewIndex=pPlaceJettonFail->lJettonArea;
		long long lJettonCount=pPlaceJettonFail->lPlaceScore;
		//合法校验
		assert(m_lUserJettonScore[cbViewIndex]>=lJettonCount);
		if (lJettonCount>m_lUserJettonScore[cbViewIndex]) return ;

		//设置下注
		m_lUserJettonScore[cbViewIndex]-=lJettonCount;
		pSeatData[cbViewIndex-1]->setUserJetton(m_lUserJettonScore[cbViewIndex]);
	}
}
//游戏结束
void GameControlOxHundred::onSubGameEnd(const void * pBuffer, WORD wDataSize){
	//效验数据
	assert(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return;
	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	CCLog("end:%lld<<%s>>",pGameEnd->lUserScore,__FUNCTION__);
	//设置时间
	resetTimer(pGameEnd->cbTimeLeave,Tools::GBKToUTF8("休息一下..."));
	hideTimer(false);
	//设置总结算
	pPlayerData[0]->lGameScore=pGameEnd->lUserScore;
	pPlayerData[1]->lGameScore=pGameEnd->lBankerScore;

	//设置牌数据
	for (int i = 0; i < sizeof(pGameEnd->cbTableCardArray)/sizeof(pGameEnd->cbTableCardArray[0]); i++)
	{
		for (int j = 0; j < sizeof(pGameEnd->cbTableCardArray[0]); j++)
		{
			getMainScene()->cardLayer->card[i][j]=pGameEnd->cbTableCardArray[i][j];
		}
	} 
	//设置发牌状态
	DataModel::sharedDataModel()->getMainSceneOxHundred()->setGameStateWithUpdate(MainSceneOxHundred::STATE_GAME_SEND_CARD);
	//操控更新
	updateButtonContron();
}
//////////////////////////////////////////////////////////////////////////
//用户状态
void GameControlOxHundred::onSubUserState(WORD wSubCmdID,void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GR_USER_STATUS://用户状态
		{
			CMD_GR_UserStatus *info= (CMD_GR_UserStatus*)pDataBuffer;
			switch (info->UserStatus.cbUserStatus)
			{
			case US_SIT://坐下
				{
					if (info->dwUserID==DataModel::sharedDataModel()->userInfo->dwUserID){
						DataModel::sharedDataModel()->userInfo->wTableID=info->UserStatus.wTableID;
						DataModel::sharedDataModel()->userInfo->wChairID=info->UserStatus.wChairID;
					}
				}
				break;
			case US_FREE://站立
				{
					if (info->dwUserID==DataModel::sharedDataModel()->userInfo->dwUserID)
					{
						//MTNotificationQueue::sharedNotificationQueue()->postNotification(S_L_US_FREE,NULL);
					}else
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
				CCLog("state==Other userID:%ld 状态：%d<<%s>>",info->dwUserID,info->UserStatus.cbUserStatus,__FUNCTION__);
				break;
			}
		}
		break;
	default:
		break;
	}
	
}
