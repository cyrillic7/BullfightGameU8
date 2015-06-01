/*
 * PopDialogBoxShop.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxShop.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "PopDialogBoxLoading.h"
#include "PopDialogBoxKnapsack.h"
#include "PopDialogBoxTipInfo.h"
#include "../Network/ListernerThread/LogonGameListerner.h"
#include "../Network/MD5/MD5.h"
#include "../GameLobby/BaseLobbyScene.h"

#define ROW_COUNT			3			//商品横排数
//////////////////////////////////////////////////////////////////////////
PopDialogBoxShop::PopDialogBoxShop()
	:shopItem(SHOP_GIFT_PACKAGE)
	, iBuyPropIndex(0)
{
	scheduleUpdate();
}
PopDialogBoxShop::~PopDialogBoxShop() {
	CCLog("~ <<%s>>",__FUNCTION__);
	unscheduleUpdate();
	TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_SHOP);
}
void PopDialogBoxShop::onEnter(){
	CCLayer::onEnter();
	//设置父结点不读取网络数据
	setParentReadMessage(false);
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxShop.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//添加背景
	CCSize deviceSize=DataModel::sharedDataModel()->deviceSize;
	CCSprite *bg=CCSprite::create("res/shop_bg.jpg");
	pWidgetBg->addNode(bg,-1);
	bg->setPosition(CCPointZero);
	float scale=deviceSize.height/bg->getContentSize().height;
	bg->setScale(scale);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBoxShop::onMenuBack));
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
	/*
	else
	{
		int i = 0;
		std::list <tagApplyUser> ::iterator iter;
		for (iter = listApplyUser.begin(); iter != listApplyUser.end(); iter++)
		{
			if (i == 0)
			{
				insertBank(false, i, (tagApplyUser)*iter);
			}
			else
			{
				insertBank(true, i, (tagApplyUser)*iter);
			}
			i++;
		}
	}*/
}
//我的背包////////////////////////////////////////////////////////////////////////
void PopDialogBoxShop::onMenuMyPackaga(CCObject *object, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		PopDialogBox *box = PopDialogBoxKnapsack::create();
		this->addChild(box, 10);
		//tempSize++;
		//updateListCommodity();
		//CCLog(" <<%s>>", __FUNCTION__);
	}
		break;
	default:
		break;
	}
}
void PopDialogBoxShop::onMenuBack(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		setParentReadMessage(true);
		this->removeFromParentAndCleanup(true);
	}
}
//购买道具
void PopDialogBoxShop::onMenuButProp(CCObject *object, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		UIButton *pButton = static_cast<UIButton*>(object);
		iBuyPropIndex = pButton->getTag();
		CCLog("tag:%d <<%s>>",iBuyPropIndex, __FUNCTION__);
		switch (shopItem)
		{
		case PopDialogBoxShop::SHOP_GIFT_PACKAGE:
		{
			setShopItem(SHOP_BUY_GIFT);
		}
			break;
		case PopDialogBoxShop::SHOP_PROP:
		{
			setShopItem(SHOP_BUY_PROP);
		}
			break;
		default:
			break;
		}
		connectServer();
	}
	break;
	default:
		break;
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
			UILabel *pItemName = static_cast<UILabel*>(pCBShopItems[i]->getChildByName("LabelShopItemName"));
			if (index != i)
			{
				pItemName->setColor(ccc3(253, 255, 0));
				pItemName->setScale(1.0);
				pCBShopItems[i]->setSelectedState(false);
				pCBShopItems[i]->setTouchEnabled(true);
			}
			else
			{
				pItemName->setColor(ccc3(255,255,255));
				pItemName->setScale(1.2);
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
		//CCLog("gift <<%s>>", __FUNCTION__);
		if (vecGift.size()<=0)
		{
			//连接服务器
			connectServer();
		}
		else
		{
			updateListCommodity(&vecGift);
		}
		
	}
		break;
	case PopDialogBoxShop::SHOP_PROP:
	{
		//CCLog("prop <<%s>>", __FUNCTION__);
		if (vecProp.size()<=0)
		{
			//连接服务器
			connectServer();
		}
		else
		{
			updateListCommodity(&vecProp);
		}
		
	}
		break;
	default:
		break;
	}
}
//连接服务器
void PopDialogBoxShop::connectServer(){
	PopDialogBox *box = PopDialogBoxLoading::create();
	this->addChild(box, 10, TAG_LOADING);
	box->setSocketName(SOCKET_SHOP);

	TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_SHOP);
	
	TCPSocket *tcp = getSocket();
	if (tcp)
	{
		tcp->createSocket(GAME_IP, PORT_LOGON, new LogonGameListerner());
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
				UIButton *pButton = static_cast<UIButton*>(pListViewCommodity->getItem(pListViewCommodity->getItems()->count() - 1)->getChildByName(CCString::createWithFormat("ImageCommodity%d", j)->getCString())->getChildByName("ButtonBuy"));
				pButton->setTag(tempIndex);
				pButton->addTouchEventListener(this, toucheventselector(PopDialogBoxShop::onMenuButProp));
				std::string buttonText = CCString::createWithFormat("%ld", vec->at(tempIndex).price[0].dwCount)->getCString();
				pButton->setTitleText(GBKToUTF8(buttonText.c_str()));

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
void PopDialogBoxShop::setParentReadMessage(bool isRead){
	((BaseLobbyScene*)this->getParent())->isReadMessage = isRead;
}
void PopDialogBoxShop::update(float delta){
	MessageQueue::update(delta);
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
	default:
		break;
	}
	
	buyGift.dwNum = 1;
	buyGift.dwBuyMethod = 4;
	
	strcpy(buyGift.szNote, "");
	strcpy(buyGift.szMachineID, "12");

	getSocket()->SendData(MDM_GP_USER_SERVICE, SUB_GP_BUYGIFT, &buyGift, sizeof(CMD_GP_BuyGift));
}
//读取网络消息回调
void PopDialogBoxShop::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_MB_SOCKET://socket连接成功
	{
		switch (shopItem)
		{
		case PopDialogBoxShop::SHOP_GIFT_PACKAGE:
		{
			getSocket()->SendData(MDM_GP_USER_SERVICE, SUB_GP_GIFT);
		}
			break;
		case PopDialogBoxShop::SHOP_PROP:
		{
			getSocket()->SendData(MDM_GP_USER_SERVICE, SUB_GP_PROPERTY);
		}
			break;
		case PopDialogBoxShop::SHOP_BUY_GIFT:
		case PopDialogBoxShop::SHOP_BUY_PROP:
		{
			buyPropForType();
		}
			break;
		default:
			break;
		}
		
	}
		break;
	case MDM_GP_USER_SERVICE://用户服务
	{
		onEventUserService(wSubCmdID, pDataBuffer, wDataSize);
	}
		break;
	default:
		CCLog("other:%d   %d<<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
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
	case SUB_GP_BUYGIFT://购买礼品包,道具
		onSubButGift(pDataBuffer, wDataSize);
		break;
	default:
		CCLog("sub:%d <<%s>>",wSubCmdID, __FUNCTION__);
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
		//CCLog("propName:%s <<%s>>",GBKToUTF8(gpGift.szName), __FUNCTION__);
		vec.push_back(gpGift);
	}
	updateListCommodity(&vec);

	//移除loading
	this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
	//关闭网络
	TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_SHOP);
}

//购买礼品
void PopDialogBoxShop::onSubButGift(void * pDataBuffer, unsigned short wDataSize){
	//效验数据
	 if (wDataSize != sizeof(CMD_GP_BuyGiftLog)) return;
	 CMD_GP_BuyGiftLog * pBuyGiftLog = (CMD_GP_BuyGiftLog *)pDataBuffer;
	 if (pBuyGiftLog->dwRet==1)
	 {
		 PopDialogBoxTipInfo *pTipInfo = PopDialogBoxTipInfo::create();
		 this->getParent()->addChild(pTipInfo, 100);
		 pTipInfo->setTipInfoContent(GBKToUTF8(pBuyGiftLog->szDescribeString));
	 }
	 else
	 {
		 PopDialogBoxTipInfo *pTipInfo = PopDialogBoxTipInfo::create();
		 this->getParent()->addChild(pTipInfo, 100);
		 pTipInfo->setTipInfoContent(GBKToUTF8(pBuyGiftLog->szDescribeString));
	 }

	 //移除loading
	 this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
	 //关闭网络
	 TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_SHOP);
}