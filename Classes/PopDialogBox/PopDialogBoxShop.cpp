/*
 * PopDialogBoxShop.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxShop.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../Tools/Tools.h"
#include "../Shader/BlurSprite.h"
#include "PopDialogBoxLoading.h"

#include "PopDialogBoxTipInfo.h"
//#include "../Network/ListernerThread/LogonGameListerner.h"
#include "../Network/CMD_Server/Packet.h"
#include "../Network/MD5/MD5.h"
#include "../GameLobby/BaseLobbyScene.h"

#define ROW_COUNT			3			//商品横排数
//////////////////////////////////////////////////////////////////////////
PopDialogBoxShop::PopDialogBoxShop()
	:shopItem(SHOP_GIFT_PACKAGE)
	, iBuyPropIndex(0)
	, lCurBuyNum(0)
	, iIPopAssistShop(NULL)
	
{
	scheduleUpdate();
}
PopDialogBoxShop::~PopDialogBoxShop() {
	CCLOG("~ <<%s>>",__FUNCTION__);
	unscheduleUpdate();
	//TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_SHOP);
	//gameSocket.Destroy(true);
}
void PopDialogBoxShop::onEnter(){
	CCLayer::onEnter();
	//设置父结点不读取网络数据
	setLobbyReadMessage(false);
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxShop.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//添加背景
	CCSize deviceSize=DataModel::sharedDataModel()->deviceSize;
    CCSprite *blurSprite=BlurSprite::create("res/main_bg.jpg");
    CCSprite *bg = CCSprite::createWithTexture(blurSprite->getTexture()) ;
	pWidgetBg->addNode(bg,-1);
	bg->setPosition(CCPointZero);
	float scale=deviceSize.height/bg->getContentSize().height;
	bg->setScaleX(scale);
	bg->setScaleY(-scale);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::onMenuBackWithReadMsg));
	//我的背包
	backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonPackage"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBoxShop::onMenuMyPackaga));
	//初始化商品列表
	initListCommodity();
	//商店选项
	for (int i = 0; i < MAX_SHOP_ITEM_COUNT; i++)
	{
		pCBShopItems[i] = static_cast<UICheckBox *>(pUILayer->getWidgetByName(CCString::createWithFormat("CheckBoxShopItem%d",i)->getCString()));
		pCBShopItems[i]->addEventListenerCheckBox(this, SEL_SelectedStateEvent(&PopDialogBoxShop::onCheckBoxSelectedStateEvent));
	}
	pCBShopItems[0]->setSelectedState(true);
	onCheckBoxSelectedStateEvent(pCBShopItems[0], CHECKBOX_STATE_EVENT_SELECTED);

	//奖券
	pIVoucher = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageScoreIcon0"));
	//元宝
	pIBigGold = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageScoreIcon1"));
	//保险箱
	pIInsure = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageScoreIcon2"));
	
// 	setVoucher(0);
// 	setBigGold(DataModel::sharedDataModel()->userInfo->lIngotScore);
// 	setInsure(DataModel::sharedDataModel()->userInfo->lInsure);
	setVoucher(0);
	setBigGold(0);
	setInsure(0);
	
	

	playAnimation();
	
}
void PopDialogBoxShop::onExit(){
	CCLayer::onExit();
}
//初始化商品列表
void PopDialogBoxShop::initListCommodity(){
	//商品列表
	pListViewCommodity = static_cast<UIListView*>(pUILayer->getWidgetByName("ListViewCommodity"));
	//设置cell模式
	pListViewCommodity->setItemModel(pListViewCommodity->getItem(0));
	//更新商品列表
	updateListCommodity(NULL);
}
//设置奖券
void PopDialogBoxShop::setVoucher(long long llVoucher){
	UILabel *pLNum = static_cast<UILabel*>(pIVoucher->getChildByName("LabelNum"));
	pLNum->setText(CCString::createWithFormat("%lld", llVoucher)->getCString());
	CCSize bigGoldSize = CCSize(pLNum->getContentSize().width + 60, pIVoucher->getContentSize().height);
	pIVoucher->setSize(bigGoldSize);
}
//设置元宝
void PopDialogBoxShop::setBigGold(long long llBigGold){
	UILabel *pLNum = static_cast<UILabel*>(pIBigGold->getChildByName("LabelNum"));
	pLNum->setText(CCString::createWithFormat("%lld", llBigGold)->getCString());
	CCSize bigGoldSize = CCSize(pLNum->getContentSize().width + 60, pIBigGold->getContentSize().height);
	pIBigGold->setSize(bigGoldSize);
}
//设置保险箱
void PopDialogBoxShop::setInsure(long long llInsure){
	UILabel *pLNum = static_cast<UILabel*>(pIInsure->getChildByName("LabelNum"));
	pLNum->setText(CCString::createWithFormat("%lld", llInsure)->getCString());
	CCSize bigGoldSize = CCSize(pLNum->getContentSize().width + 60, pIInsure->getContentSize().height);
	pIInsure->setSize(bigGoldSize);
}
//购买数量回调
void PopDialogBoxShop::onBuyNum(long lNum){
	lCurBuyNum = lNum;
	connectServer();
	connectSuccess();
}
//跳转到vip
void PopDialogBoxShop::onToVip(){
	getIPopDialogBoxAssist()->onCloseViewToVip();
	this->removeFromParentAndCleanup(true);
}
//我的背包////////////////////////////////////////////////////////////////////////
void PopDialogBoxShop::onMenuMyPackaga(CCObject *object, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		isReadMessage = false;
		PopDialogBoxKnapsack *box = PopDialogBoxKnapsack::create();
		this->addChild(box, 10);
		box->setIPopAssistKnapsack(this);
		//tempSize++;
		//updateListCommodity();
		//CCLOG(" <<%s>>", __FUNCTION__);
	}
		break;
	default:
		break;
	}
}
//关闭背包回调////////////////////////////////////////////////////////////////////////
void PopDialogBoxShop::onCloseKnapsack(){
	isReadMessage = true;
}
//购买道具
void PopDialogBoxShop::onMenuBuyProp(CCObject *object, TouchEventType type){
	if(type == TOUCH_EVENT_ENDED)
	{
		UIButton *pButton = static_cast<UIButton*>(object);
		iBuyPropIndex = pButton->getTag();
		switch (shopItem)
		{
		case PopDialogBoxShop::SHOP_GIFT_PACKAGE:
		case PopDialogBoxShop::SHOP_BUY_GIFT:
		{
			setShopItem(SHOP_BUY_GIFT);

			//会员价
			float vipDiscount = vecGift.at(iBuyPropIndex).dwDiscount / 100.0;
			if (vecGift.at(iBuyPropIndex).dwDiscount == 0)
			{
				vipDiscount = 1;
			}
			long vipPice = vecGift.at(iBuyPropIndex).price[0].dwCount*vipDiscount;

			showInputNumBox(BUY_SHOP, GBKToUTF8(vecGift[iBuyPropIndex].szName).c_str(), vecGift[iBuyPropIndex].szImgName, 1, vipPice, vecGift[iBuyPropIndex].dwDiscount, 0, this);
		}
			break;
		case PopDialogBoxShop::SHOP_PROP:
		case PopDialogBoxShop::SHOP_BUY_PROP:
		{
			setShopItem(SHOP_BUY_PROP);

			//会员价
			float vipDiscount = vecProp.at(iBuyPropIndex).dwDiscount / 100.0;
			if (vecProp.at(iBuyPropIndex).dwDiscount == 0)
			{
				vipDiscount = 1;
			}
			long vipPice = vecProp.at(iBuyPropIndex).price[0].dwCount*vipDiscount;

			showInputNumBox(BUY_SHOP, GBKToUTF8(vecProp[iBuyPropIndex].szName).c_str(), vecProp[iBuyPropIndex].szImgName, 1, vipPice, vecProp[iBuyPropIndex].dwDiscount, 0, this);
		}
			break;
		case PopDialogBoxShop::SHOP_VIP:
		case PopDialogBoxShop::SHOP_BUY_VIP:
		{
			setShopItem(SHOP_BUY_VIP);

			//会员价
			float vipDiscount = vecVip.at(iBuyPropIndex).dwDiscount / 100.0;
			if (vecVip.at(iBuyPropIndex).dwDiscount == 0)
			{
				vipDiscount = 1;
			}
			vipDiscount = 1;
			long vipPice = vecVip.at(iBuyPropIndex).price[0].dwCount*vipDiscount;

			showInputNumBox(BUY_SHOP_VIP, GBKToUTF8(vecVip[iBuyPropIndex].szName).c_str(), vecVip[iBuyPropIndex].szImgName, 1, vipPice, vecVip[iBuyPropIndex].dwDiscount, 0, this);
		}
		break;
		default:
			break;
		}
		
		//connectServer(SOCKET_SHOP);
	}
}
//复选框回调
void PopDialogBoxShop::onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type){
	switch (type)
	{
	case CHECKBOX_STATE_EVENT_SELECTED:
	{
		UICheckBox *box = (UICheckBox*)pSender;
		int index = box->getTag() - 1;
		
		for (int i = 0; i < MAX_SHOP_ITEM_COUNT; i++)
		{
			//UILabel *pItemName = static_cast<UILabel*>(pCBShopItems[i]->getChildByName("LabelShopItemName"));
			if (index != i)
			{
				//pItemName->setColor(ccc3(253, 255, 0));
				//pItemName->setScale(1.0);
				pCBShopItems[i]->setSelectedState(false);
				pCBShopItems[i]->setTouchEnabled(true);
			}
			else
			{
				//pItemName->setColor(ccc3(255,255,255));
				//pItemName->setScale(1.2);
				pCBShopItems[i]->setTouchEnabled(false);
				changeSelectItem((ShopItem)index);
			}
		}
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
//切换商店项
void PopDialogBoxShop::changeSelectItem(ShopItem eItem){
	setShopItem(eItem);
	switch (eItem)
	{
	case PopDialogBoxShop::SHOP_GIFT_PACKAGE:
	{
		//CCLOG("gift <<%s>>", __FUNCTION__);
		if (vecGift.size()<=0)
		{
			//连接服务器
			connectServer();
			connectSuccess();
		}
		else
		{
			updateListCommodity(&vecGift);
		}
		
	}
		break;
	case PopDialogBoxShop::SHOP_PROP:
	{
		//CCLOG("prop <<%s>>", __FUNCTION__);
		if (vecProp.size()<=0)
		{
			//连接服务器
			connectServer();
			connectSuccess();
		}
		else
		{
			updateListCommodity(&vecProp);
		}
		
	}
		break;
	case PopDialogBoxShop::SHOP_VIP:
	{
		if (vecVip.size() <= 0)
		{
			//连接服务器
			connectServer();
			connectSuccess();
		}
		else
		{
			updateListCommodity(&vecVip);
		}
	}
		break;
	default:
		break;
	}
}
//更新商品列表
void PopDialogBoxShop::updateListCommodity(std::vector<CMD_GP_Gift> *vec){
	pListViewCommodity->removeAllItems();
	if (vec==NULL)
	{
		return;
	}
	int tempSize = vec->size() ;
	if (tempSize == 0)
	{
		return;
	}
	
	for (int i = 0; i < (tempSize - 1) / ROW_COUNT + 1; i++)
	{
		pListViewCommodity->insertDefaultItem(pListViewCommodity->getItems()->count());
		for (int j = 0; j < ROW_COUNT; j++)
		{
			int tempIndex = i*ROW_COUNT + j;
			if (tempIndex<vec->size())
			{
				//加载商品图片
				UIImageView *pIVItem = static_cast<UIImageView*>(pListViewCommodity->getItem(pListViewCommodity->getItems()->count() - 1)->getChildByName(CCString::createWithFormat("ImageCommodity%d", j)->getCString()));
				float fScale = CCDirector::sharedDirector()->getContentScaleFactor();
				addDownloadImage(pIVItem, vec->at(tempIndex).szImgName, ccp(0, 0 * fScale), 1, 0, false);
				
				UIButton *pButton = static_cast<UIButton*>(pListViewCommodity->getItem(pListViewCommodity->getItems()->count() - 1)->getChildByName(CCString::createWithFormat("ImageCommodity%d", j)->getCString())->getChildByName("ButtonBuy"));
				pButton->setTag(tempIndex);
				pButton->addTouchEventListener(this, toucheventselector(PopDialogBoxShop::onMenuBuyProp));
				std::string buttonText = CCString::createWithFormat("券%ld", vec->at(tempIndex).price[0].dwCount)->getCString();
				pButton->setTitleText(buttonText.c_str());
				//会员价
				float vipDiscount =  vec->at(tempIndex).dwDiscount/100.0;
				if (vec->at(tempIndex).dwDiscount==0)
				{
					//vipDiscount = 1;
					vipDiscount = 0.98;
				}
				long vipPice = vec->at(tempIndex).price[0].dwCount*vipDiscount;
				UILabel *pLVipPice0 = static_cast<UILabel*>(pIVItem->getChildByName("ImageVipPice")->getChildByName("LabelVipPice0"));
				pLVipPice0->setText(CCString::createWithFormat("%ld", vipPice)->getCString());
				UILabel *pLVipPice1 = static_cast<UILabel*>(pIVItem->getChildByName("ImageVipPice")->getChildByName("LabelVipPice1"));
				pLVipPice1->setText(CCString::createWithFormat("%ld", vipPice)->getCString());
				if (getShopItem()==SHOP_VIP)
				{
					pLVipPice0->setVisible(false);
					pLVipPice1->setVisible(false);
					pLVipPice0->getParent()->setVisible(false);
					vipDiscount = 1;
				}

				UILabel *pPropName = static_cast<UILabel*>(pListViewCommodity->getItem(pListViewCommodity->getItems()->count() - 1)->getChildByName(CCString::createWithFormat("ImageCommodity%d", j)->getCString())->getChildByName("LabelPropName"));
				pPropName->setText(GBKToUTF8(vec->at(tempIndex).szName));

				UILabel *pPropDescription = static_cast<UILabel*>(pListViewCommodity->getItem(pListViewCommodity->getItems()->count() - 1)->getChildByName(CCString::createWithFormat("ImageCommodity%d", j)->getCString())->getChildByName("LabelPropDescription"));
				pPropDescription->setText(GBKToUTF8(vec->at(tempIndex).szDespict));
				
	
			}
		}
	}

	for (int j = 0; j < ROW_COUNT; j++)
	{
		if (j > (tempSize-1) % (ROW_COUNT)){
			UIImageView *pItem = static_cast<UIImageView*>(pListViewCommodity->getItem(pListViewCommodity->getItems()->count() - 1)->getChildByName(CCString::createWithFormat("ImageCommodity%d", j)->getCString()));
			pItem->setEnabled(false);
		}

	}
}
void PopDialogBoxShop::update(float delta){
	if (isReadMessage)
	{
		//MessageQueue::update(delta);
	}
	gameSocket.updateSocketData(delta);
}
//购买道具
void PopDialogBoxShop::buyPropForType(){
	CMD_GP_BuyGift buyGift;
	buyGift.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;
	buyGift.dwOpTerminal = 2;

	MD5 m;
	m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(), DataModel::sharedDataModel()->sLogonPassword.length());
	std::string md5PassWord = m.GetMd5();
	strcpy(buyGift.szLogonPass, md5PassWord.c_str());
	
	switch (shopItem)
	{
	case PopDialogBoxShop::SHOP_BUY_GIFT:
	{
		buyGift.dwID = vecGift[iBuyPropIndex].dwID;
		buyGift.dwType = 1;
	}
		break;
	case PopDialogBoxShop::SHOP_BUY_PROP:
	{
		buyGift.dwID = vecProp[iBuyPropIndex].dwID;
		buyGift.dwType = 2;
	}
		break;
	case PopDialogBoxShop::SHOP_BUY_VIP:
	{
		buyGift.dwID = vecVip[iBuyPropIndex].dwID;
		buyGift.dwType = 3;
	}
	break;
	default:
		break;
	}
	
	buyGift.dwNum = lCurBuyNum;
	buyGift.dwBuyMethod = 4;
	
//	strcpy(buyGift.szNote, "");
	strcpy(buyGift.szMachineID, Tools::getMachineID().c_str());

	bool isSend=gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_BUYGIFT, &buyGift, sizeof(CMD_GP_BuyGift));
	CCLOG("send: %d  <<%s>>",isSend, __FUNCTION__);
}
//连接成功
void PopDialogBoxShop::connectSuccess(){
	switch (shopItem)
	{
	case PopDialogBoxShop::SHOP_GIFT_PACKAGE:
	{
		CMD_GP_GetGift getGift;
		getGift.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;
		getGift.dwOpTerminal = 2;
		gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_GIFT, &getGift, sizeof(getGift));
	}
	break;
	case PopDialogBoxShop::SHOP_PROP:
	{
		CMD_GP_GetGift getGift;
		getGift.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;
		getGift.dwOpTerminal = 2;
		gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_PROPERTY, &getGift, sizeof(getGift));
	}
	break;
	case PopDialogBoxShop::SHOP_VIP:
	{
		CMD_GP_GetGift getGift;
		getGift.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;
		getGift.dwOpTerminal = 2;
		gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_VIPSHOP, &getGift, sizeof(getGift));
	}
	break;
	case PopDialogBoxShop::SHOP_BUY_GIFT:
	case PopDialogBoxShop::SHOP_BUY_PROP:
	case PopDialogBoxShop::SHOP_BUY_VIP:
	{
		buyPropForType();
	}
	break;
	case PopDialogBoxShop::SHOP_GET_PROPERTY://获取财产
	{
		CMD_GP_UserID userID;
		userID.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;

		MD5 m;
		m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(), DataModel::sharedDataModel()->sLogonPassword.length());
		std::string md5PassWord = m.GetMd5();
		strcpy(userID.szPassword, md5PassWord.c_str());

		gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_TREASURE, &userID, sizeof(CMD_GP_UserID));
	}
	break;
	default:
		break;
	}
}
//读取网络消息回调
void PopDialogBoxShop::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_MB_SOCKET://socket连接成功
	{
		connectSuccess();
	}
		break;
	case MDM_GP_USER_SERVICE://用户服务
	{
		onEventUserService(wSubCmdID, pDataBuffer, wDataSize);
	}
		break;
	default:
		CCLOG("other:%d   %d<<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
		break;
	}
}
//用户服务
void PopDialogBoxShop::onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_GIFT://礼品包列表
		onSubGiftList(pDataBuffer, wDataSize,vecGift);
		break;							
	case SUB_GP_PROPERTY://道具列表
		onSubGiftList(pDataBuffer, wDataSize,vecProp);
		break;
	case SUB_GP_VIPSHOP://VIP馆
		onSubGiftList(pDataBuffer, wDataSize, vecVip);
		break;
	case SUB_GP_BUYGIFT://购买礼品包,道具
		onSubBuyGift(pDataBuffer, wDataSize);
		break;
	case SUB_GP_TREASURE://财富详细 
		onSubTreasure(pDataBuffer, wDataSize);
		break;
	default:
		CCLOG("sub:%d <<%s>>",wSubCmdID, __FUNCTION__);
		break;
	}
}
//礼品列表
void PopDialogBoxShop::onSubGiftList(void * pDataBuffer, unsigned short wDataSize, std::vector<CMD_GP_Gift> &vec){
	// 
	assert(wDataSize >= sizeof(CMD_GP_Gift));

	int gameServerSize = sizeof(CMD_GP_Gift);
	int serverCount = wDataSize / gameServerSize;

	BYTE cbDataBuffer[SOCKET_TCP_PACKET + sizeof(TCP_Head)];
	CopyMemory(cbDataBuffer, pDataBuffer, wDataSize);

	for (int i = 0; i < serverCount; i++)
	{
		void * pDataBuffer = cbDataBuffer + i*sizeof(CMD_GP_Gift);
		CMD_GP_Gift *pGPGift = (CMD_GP_Gift*)pDataBuffer;
		CMD_GP_Gift gpGift;
		memcpy(&gpGift, pGPGift, sizeof(CMD_GP_Gift));
		//CCLOG("propName:%s <<%s>>",GBKToUTF8(gpGift.szName), __FUNCTION__);
		vec.push_back(gpGift);
	}
	updateListCommodity(&vec);

	//移除loading
	this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
	//关闭网络
	//TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_SHOP);
	gameSocket.Destroy(true);

	//刷新财富
	setShopItem(SHOP_GET_PROPERTY);
	connectServer();
	connectSuccess();
}

//购买礼品
void PopDialogBoxShop::onSubBuyGift(void * pDataBuffer, unsigned short wDataSize){
	//效验数据
	 if (wDataSize != sizeof(CMD_GP_BuyGiftLog)) return;

	 //移除loading
	 this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);

	 CMD_GP_BuyGiftLog * pBuyGiftLog = (CMD_GP_BuyGiftLog *)pDataBuffer;
	 if (pBuyGiftLog->dwRet==1)
	 {
		 PopDialogBoxTipInfo *pTipInfo = PopDialogBoxTipInfo::create();
		 this->getParent()->addChild(pTipInfo, 100);
		 pTipInfo->setTipInfoContent(GBKToUTF8(pBuyGiftLog->szDescribeString).c_str());
	 }
	 else
	 {
		 PopDialogBoxTipInfo *pTipInfo = PopDialogBoxTipInfo::create();
		 this->getParent()->addChild(pTipInfo, 100);
		 pTipInfo->setTipInfoContent(GBKToUTF8(pBuyGiftLog->szDescribeString).c_str());
	 }

	 
	 //关闭网络
	 //TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_SHOP);
	 gameSocket.Destroy(true);
	 //购买成功刷新财富
	 if (pBuyGiftLog->dwRet==0)
	 {
		 //刷新财富
		 setShopItem(SHOP_GET_PROPERTY);
		 connectServer();
		 connectSuccess();
	 }
}
//财富
void PopDialogBoxShop::onSubTreasure(void * pDataBuffer, unsigned short wDataSize){
	if (wDataSize != sizeof(CMD_GP_UserTreasure)) return;
	CMD_GP_UserTreasure * pUserTreasure = (CMD_GP_UserTreasure *)pDataBuffer;

	setVoucher(pUserTreasure->lLottery);
	setBigGold(pUserTreasure->lIngotScore);
	setInsure(pUserTreasure->lInsureScore);

	//移除loading
	this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
	//关闭网络
	//TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_SHOP);
	gameSocket.Destroy(true);

	if (pCBShopItems[0]->getSelectedState())
	{
		setShopItem(SHOP_GIFT_PACKAGE);
	}
	else if(pCBShopItems[1]->getSelectedState())
	{
		setShopItem(SHOP_PROP);
	}
	else if (pCBShopItems[2]->getSelectedState())
	{
		setShopItem(SHOP_VIP);
	}
	
}