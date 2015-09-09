//
//  BaseLobbyScene.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#include "BaseLobbyScene.h"
#include "../Tools/Tools.h"
#include "../Tools/BaseAttributes.h"
#include "../PopDialogBox/PopDialogBoxUserInfo.h"
#include "../PopDialogBox/PopDialogBoxSetUp.h"
#include "../PopDialogBox/PopDialogBoxMore.h"
#include "../PopDialogBox/PopDialogBoxActivity.h"
#include "../PopDialogBox/PopDialogBoxTask.h"
#include "../PopDialogBox/PopDialogBoxBank.h"
#include "../PopDialogBox/PopDialogBoxShop.h"
#include "../PopDialogBox/PopDialogBoxAuction.h"
#include "../PopDialogBox/PopDialogBoxRanking.h"
#include "../PopDialogBox/PopDialogBoxTipInfo.h"
#include "../PopDialogBox/PopDialogBoxMsg.h"
#include "../PopDialogBox/PopDialogBoxVip.h"
#include "../PopDialogBox/PopDialogBoxRecharge.h"
#include "../PopDialogBox/PopDialogBoxFirstRecharge.h"
#include "../Platform/coPlatform.h"
#include "../Tools/DataModel.h"
#include "network/HttpRequest.h"
BaseLobbyScene * BaseLobbyScene::lobbyScene = NULL;
BaseLobbyScene::BaseLobbyScene()
	:isReadMessage(true){
	setKeypadEnabled(true);//设置相应按键消息 
}

BaseLobbyScene::~BaseLobbyScene(){
	CCLOG("~ <<%s>>", __FUNCTION__);
}
//进入场景
void BaseLobbyScene::onEnter(){
	CCLayer::onEnter();
	
	//添加背景
	CCSize deviceSize=DataModel::sharedDataModel()->deviceSize;
	CCSprite *spriteBg=CCSprite::create("res/main_bg.jpg");
	this->addChild(spriteBg,-1);
	spriteBg->setPosition(ccp(deviceSize.width/2,deviceSize.height/2));
	float scale=deviceSize.height/spriteBg->getContentSize().height;
	spriteBg->setScale(scale);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//粒子
	CCParticleSystemQuad *emitter1 = CCParticleSystemQuad::create("particle/sakura.plist");
	emitter1->setPosition(ccp(SCENE_SIZE.width+20, SCENE_SIZE.height+20));    // 设置发射粒子的位置
	//emitter1->setAutoRemoveOnFinish(true);                          // 完成后制动移除
	//emitter1->setDuration(10);                                      // 设置粒子系统的持续时间秒
	spriteBg->addChild(emitter1, 1);
	//////////////////////////////////////////////////////////////////////////
	//创建UI层
	m_pWidgetBase = UILayer::create();
	this->addChild(m_pWidgetBase);
	//加载UI层
	UILayout *pWidget = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIBaseLobby.ExportJson)));
	m_pWidgetBase->addWidget(pWidget);
	//绑定设置按键
	UIButton* button  = static_cast<UIButton*>(m_pWidgetBase->getWidgetByName("ButtonSetUp"));
	button->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuCallback));
	//绑定用户信息按键
	pBUserInfo= static_cast<UIButton*>(m_pWidgetBase->getWidgetByName("ButtonIcon"));
	pBUserInfo->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::menuResetUser));
	pBUserInfo->loadTextureNormal(CCString::createWithFormat("u_p_icon_%d.png", DataModel::sharedDataModel()->userInfo->wFaceID)->getCString(), UI_TEX_TYPE_PLIST);
	pBUserInfo->loadTexturePressed(CCString::createWithFormat("u_p_icon_%d.png", DataModel::sharedDataModel()->userInfo->wFaceID)->getCString(), UI_TEX_TYPE_PLIST);

	//绑定更多按键
	button = static_cast<UIButton*>(m_pWidgetBase->getWidgetByName("ButtonMore"));
	button->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuCallback));
	//绑定活动按键
	button = static_cast<UIButton*>(m_pWidgetBase->getWidgetByName("ButtonActivity"));
	button->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuCallback));
	//绑定任务按键
	button = static_cast<UIButton*>(m_pWidgetBase->getWidgetByName("ButtonTask"));
	button->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuCallback));
	//绑定银行按键
	button = static_cast<UIButton*>(m_pWidgetBase->getWidgetByName("ButtonBank"));
	button->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuCallback));
	//绑定商店按键
	button = static_cast<UIButton*>(m_pWidgetBase->getWidgetByName("ButtonShop"));
	button->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuCallback));
	//绑定拍卖按键
	button = static_cast<UIButton*>(m_pWidgetBase->getWidgetByName("ButtonAuction"));
	button->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuCallback));
	//绑定排行按键
	button = static_cast<UIButton*>(m_pWidgetBase->getWidgetByName("ButtonRanking"));
	button->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuCallback));
	//绑定消息
	button = static_cast<UIButton*>(m_pWidgetBase->getWidgetByName("ButtonMsg"));
	button->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuCallback));
	//绑定背包
	button = static_cast<UIButton*>(m_pWidgetBase->getWidgetByName("ButtonKnapsack"));
	button->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuCallback));
	//绑定充值
	button = static_cast<UIButton*>(m_pWidgetBase->getWidgetByName("ButtonRecharge"));
	button->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuCallback));
	//立即游戏
	//UIButton *pBStartGame = static_cast<UIButton*>(m_pWidgetBase->getWidgetByName("ButtonStartGame"));
	//pBStartGame->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuQuickGame));
	//绑定VIP
	UIImageView *pIVip = static_cast<UIImageView*>(m_pWidgetBase->getWidgetByName("ImageVip"));
	pIVip->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuVip));
	pIVip->loadTexture(CCString::createWithFormat("icon_vip%d.png",DataModel::sharedDataModel()->userInfo->dwVipLevel)->getCString(), UI_TEX_TYPE_PLIST);
	//绑定首充
	UIImageView *pFirstRecharge = static_cast<UIImageView*>(m_pWidgetBase->getWidgetByName("ButtonFirstRecharge"));
	pFirstRecharge->addTouchEventListener(this, SEL_TouchEvent(&BaseLobbyScene::onMenuFirstRecharge));
	
	//用户名
	userName=static_cast<UILabel*>(m_pWidgetBase->getWidgetByName("labelUserName"));
	//金币
	pLabelGoldCount=static_cast<UILabel*>(m_pWidgetBase->getWidgetByName("LabelGoldCount"));
	pLabelGoldCount->setText(CCString::createWithFormat("%lld",DataModel::sharedDataModel()->userInfo->lScore)->getCString());

	//金币闪光动画
	UIImageView *pIVGoldIcon = static_cast<UIImageView*>(m_pWidgetBase->getWidgetByName("ImageGoldIcon")); 
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(CCS_PATH_SCENE(AnimationGameIng.ExportJson));
	pAnimate = CCArmature::create("AnimationGameIng");
	pIVGoldIcon->addNode(pAnimate, 100);
	pAnimate->setPosition(3, 1);
	pAnimate->setVisible(false);
	schedule(SEL_SCHEDULE(&BaseLobbyScene::updateGoldLight), 2);
}
//退出场景
void BaseLobbyScene::onExit(){
	CCLayer::onExit();
}
//post回调
void BaseLobbyScene::onHttpRequestCompleted(CCHttpClient* client, CCHttpResponse* response){
	
	if (!response->isSucceed())
	{
		CCLOG("-------------return <<%s>>", __FUNCTION__);
		//return;
	}
	CCLOG("--------------*************************** <<%s>>", __FUNCTION__);
	platformAction("{\"act\":201 ,\"url\":\"http://121.40.31.203:8010\"}").c_str();
}
//弹出框
void BaseLobbyScene::popDialogBox(PopType type){
	PopDialogBox *pdb = NULL;
	switch (type)
	{
	case BaseLobbyScene::POP_USER_INFO:
	{
		pdb = PopDialogBoxUserInfo::create();
		((PopDialogBoxUserInfo *)pdb)->setIPopDialogBoxAssist(this);
	}
		break;
	case BaseLobbyScene::POP_SETUP:
		pdb=PopDialogBoxSetUp::create();
		break;
	case BaseLobbyScene::POP_MORE:
		pdb=PopDialogBoxMore::create();
		break;
	case BaseLobbyScene::POP_ACTIVITY:
	{
		std::string actStr = "{\"act\":201 ,\"url\":\"http://121.40.31.203:9021/Home/Index/?account=";
		actStr += DataModel::sharedDataModel()->sLogonAccount;
		actStr += "&pwd=";
		actStr += DataModel::sharedDataModel()->sLogonPassword;
		actStr += "&plat=1\"}";
		platformAction(actStr.c_str());

		//platformAction("{\"act\":201 ,\"url\":\"http://121.40.31.203:9021/Home/Index/?account=test0001&pwd=123456&plat=1\"}").c_str();

		/*CCHttpRequest *request=new CCHttpRequest();
		request->setUrl("http://121.40.31.203:8010/api/Login/?account=z40144322&pwd=z12345678&plat=1");
		request->setRequestType(CCHttpRequest::kHttpGet);*/
		
		/*std::vector <std::string>headers;
		headers.push_back("Content-Type: application/json; charset=utf-8");
		request->setHeaders(headers);
		
		
		const  char* postData = "account=z40144322&pwd=z12345678&plat=1";
		request->setRequestData(postData, strlen(postData));
		*/
		/*request->setResponseCallback(this, httpresponse_selector(BaseLobbyScene::onHttpRequestCompleted));

		request->setTag("GET test1");
		CCHttpClient::getInstance()->send(request);
		request->release();
		*/
		

#ifdef WIN32

#else
		/*HttpRequest* request = new HttpRequest();
		request->setUrl("http://121.40.31.203:8010/api/Login/");
		request->setRequestType(HttpRequest::Type::POST);
		request->setResponseCallback(this, httpresponse_selector(HelloWorld::onHttpRequestCompleted));


		const  char* postData = "account=z40144322";
		request->setRequestData(postData,strlen(postData) );
		const  char* postData1 = "pwd=z12345678";
		request->setRequestData(postData1,strlen(postData1) );
		const  char* postData2 = "pwd=1";
		request->setRequestData(postData2,strlen(postData2) );


		//request->setTag("POST test1");
		HttpClient::getInstance()->send(request);
		request->release();*/


		m_pWidget->setTouchEnabled(false);
		m_pWidgetBase->setTouchEnabled(false);
		/*std::string actStr="{\"act\":201 ,\"url\":\"http://121.40.31.203:8010/api/Login/?account=";
		actStr+=DataModel::sharedDataModel()->sLogonAccount;
		actStr += "&pwd=";
		actStr += DataModel::sharedDataModel()->sLogonPassword;
		actStr += "&plat=1\"}";
		platformAction(actStr.c_str());*/
		//platformAction("{\"act\":201 ,\"url\":\"http://121.40.31.203:8010\"}").c_str();
#endif
		//pdb = PopDialogBoxActivity::create();
	}
		break;
	case BaseLobbyScene::POP_TASK:
		pdb=PopDialogBoxTask::create();
		break;
	case BaseLobbyScene::POP_BANK:
		pdb=PopDialogBoxBank::create();
		break;
	case BaseLobbyScene::POP_SHOP:
	{
		pdb = PopDialogBoxShop::create();
		((PopDialogBoxShop *)pdb)->setIPopDialogBoxAssist(this);
	}
		break;
	case BaseLobbyScene::POP_AUCTION:
		pdb=PopDialogBoxAuction::create();
		break;
	case BaseLobbyScene::POP_RANKING:
		pdb=PopDialogBoxRanking::create();
		break;
	case BaseLobbyScene::POP_MSG:
		pdb = PopDialogBoxMsg::create();
		break;
	case BaseLobbyScene::POP_VIP:
	{
		pdb = PopDialogBoxVip::create();
		((PopDialogBoxVip *)pdb)->setIPopDialogBoxAssist(this);
	}
		
		break;
	case BaseLobbyScene::POP_KNAPSACK:
	{
		isReadMessage = false;
		pdb = PopDialogBoxKnapsack::create();
		((PopDialogBoxKnapsack *)pdb)->setIPopAssistKnapsack(this);
	}
		break;
	case BaseLobbyScene::POP_RECHARGE://充值、兑换
	{
		isReadMessage = false;
		pdb = PopDialogBoxRecharge::create();
		((PopDialogBoxRecharge *)pdb)->setIPopDialogBoxAssistCloseView(this);
	}

	break;
	default:
		break;
	}
	if (pdb)
	{
		this->addChild(pdb);
	}
}
//立即游戏
void BaseLobbyScene::onMenuQuickGame(CCObject* pSender, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		quickGame();
	}
}
//首充值
void BaseLobbyScene::onMenuFirstRecharge(CCObject* pSender, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		PopDialogBoxFirstRecharge *pPDBFirstRecharge = PopDialogBoxFirstRecharge::create();
		this->addChild(pPDBFirstRecharge);
	}
}
//菜单回调
void BaseLobbyScene::onMenuCallback(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			UIButton *button=(UIButton*)pSender;
			if(strcmp(button->getName(),"ButtonSetUp")==0)
			{
				popDialogBox(POP_SETUP);
			}
			else if(strcmp(button->getName(),"ButtonMore")==0)
			{
				popDialogBox(POP_MORE);
			}
			else if(strcmp(button->getName(),"ButtonActivity")==0)
			{
				popDialogBox(POP_ACTIVITY);
			}
			else if(strcmp(button->getName(),"ButtonTask")==0)
			{
				popDialogBox(POP_TASK);
			}
			else if(strcmp(button->getName(),"ButtonTask")==0)
			{
				popDialogBox(POP_TASK);
			}
			else if(strcmp(button->getName(),"ButtonBank")==0)
			{
				popDialogBox(POP_BANK);
			}
			else if(strcmp(button->getName(),"ButtonShop")==0)
			{
				popDialogBox(POP_SHOP);
			}
			else if(strcmp(button->getName(),"ButtonAuction")==0)
			{
				popDialogBox(POP_AUCTION);
			}
			else if(strcmp(button->getName(),"ButtonRanking")==0)
			{
				popDialogBox(POP_RANKING);
			}
			else if (strcmp(button->getName(), "ButtonMsg") == 0)
			{
				popDialogBox(POP_MSG);
			}
			else if (strcmp(button->getName(), "ButtonKnapsack") == 0)
			{
				popDialogBox(POP_KNAPSACK);
			}
			else if (strcmp(button->getName(), "ButtonRecharge") == 0)
			{
				popDialogBox(POP_RECHARGE);
			}
			else
			{
				PopDialogBoxTipInfo *tipInfo = PopDialogBoxTipInfo::create();
				this->addChild(tipInfo);
				tipInfo->setTipInfoContent(BaseAttributes::sharedAttributes()->sWaitCodeing.c_str());
			}
		}
		break;
	default:
		break;
	}
}
//用户信息回调
void BaseLobbyScene::menuResetUser(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		popDialogBox(POP_USER_INFO);
		break;
	default:
		break;
	}
}
//VIP
void BaseLobbyScene::onMenuVip(CCObject* pSender, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		popDialogBox(POP_VIP);
		break;
	default:
		break;
	}
}
//关闭背包回调
void BaseLobbyScene::onCloseKnapsack(){
	isReadMessage = true;
}
//关闭VIP回调
void BaseLobbyScene::onCloseViewToShop(){
	popDialogBox(POP_SHOP);
}
//关闭跳转到VIP
void BaseLobbyScene::onCloseViewToVip(){
	popDialogBox(POP_VIP);
}
//关闭wap网站
void BaseLobbyScene::closeWebView(){
	m_pWidget->setTouchEnabled(true);
	m_pWidgetBase->setTouchEnabled(true);
}
//android返回键 
void BaseLobbyScene::keyBackClicked(){
	platformAction("{\"act\":400}");
}
//金币闪光动画
void BaseLobbyScene::updateGoldLight(float dt){
	if (!pAnimate->isVisible())
	{
		pAnimate->setVisible(true);
	}
	pAnimate->getAnimation()->play("cUIGold");
}
