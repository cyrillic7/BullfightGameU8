/*
 * PopDialogBoxShop.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxShop.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../Network/CMD_Server/CMD_LogonServer.h"
#include "PopDialogBoxLoading.h"
#include "../Network/ListernerThread/LogonGameListerner.h"
#include "../GameLobby/BaseLobbyScene.h"
#define ROW_COUNT			3			//商品横排数
//////////////////////////////////////////////////////////////////////////
PopDialogBoxShop::PopDialogBoxShop()
{
	tempSize = 0;
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
	updateListCommodity();
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
		tempSize++;
		updateListCommodity();
		CCLog(" <<%s>>", __FUNCTION__);
	}
		break;
	default:
		break;
	}
}
void PopDialogBoxShop::onMenuBack(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		/*Tools::playSound(kSoundBack);
		GameHUD *gameHUD = DataModel::sharedDataModel()->getMainSceneManage()->getGameHUD();
		gameHUD->changePauseState(false);
		//this->getParent()->removeChild(this, true);
		*/
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
		PopDialogBox *box = PopDialogBoxLoading::create();
		this->addChild(box, 10, TAG_LOADING);
		TCPSocket *tcp = getSocket();
		if (tcp)
		{
			tcp->createSocket(GAME_IP, PORT_LOGON, new LogonGameListerner());
		}

		//bool isSend=getSocket()->SendData(MDM_GP_USER_SERVICE, SUB_GP_GIFT);
		//CCLog("isSEND=============:%d <<%s>>",isSend,__FUNCTION__);
		UIButton *pButton = static_cast<UIButton*>(object);
		CCLog("tag:%d <<%s>>",pButton->getTag(), __FUNCTION__);
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
		CCLog("gift <<%s>>", __FUNCTION__);
	}
		break;
	case PopDialogBoxShop::SHOP_PROP:
	{
		CCLog("prop <<%s>>", __FUNCTION__);
	}
		break;
	default:
		break;
	}
}
//更新商品列表
void PopDialogBoxShop::updateListCommodity(){
	pListViewCommodity->removeAllItems();
	if (tempSize==0)
	{
		return;
	}
	
	for (int i = 0; i < (tempSize-1) / ROW_COUNT + 1; i++)
	{
		pListViewCommodity->insertDefaultItem(pListViewCommodity->getItems()->count());
		for (int j = 0; j < ROW_COUNT; j++)
		{
			UIButton *pItem = static_cast<UIButton*>(pListViewCommodity->getItem(pListViewCommodity->getItems()->count() - 1)->getChildByName(CCString::createWithFormat("ImageCommodity%d", j)->getCString())->getChildByName("ButtonBuy"));
			pItem->setTag(i*ROW_COUNT+j);
			pItem->addTouchEventListener(this, toucheventselector(PopDialogBoxShop::onMenuButProp)); 
		}
	}

	for (int j = 0; j < ROW_COUNT; j++)
	{
		if (j > (tempSize-1) % (ROW_COUNT)){
			UIImageView *pItem = static_cast<UIImageView*>(pListViewCommodity->getItem(pListViewCommodity->getItems()->count() - 1)->getChildByName(CCString::createWithFormat("ImageCommodity%d", j)->getCString()));
			pItem->setEnabled(false);
		}

	}
	/*
	for (int j = 0; j < ROW_COUNT; j++){
			if (j >= tempSize % (ROW_COUNT + 1))
			{
				UIImageView *pItem = static_cast<UIImageView*>(pListViewCommodity->getItem(i)->getChildByName(CCString::createWithFormat("ImageCommodity%d", j)->getCString()));
				pItem->setEnabled(false);
			}
		}*/
}
void PopDialogBoxShop::setParentReadMessage(bool isRead){
	((BaseLobbyScene*)this->getParent())->isReadMessage = isRead;
}
void PopDialogBoxShop::update(float delta){
	MessageQueue::update(delta);
}
//读取网络消息回调
void PopDialogBoxShop::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_MB_SOCKET://socket连接成功
	{
		getSocket()->SendData(MDM_GP_USER_SERVICE, SUB_GP_GIFT);
	}
		break;
	case MDM_GP_USER_SERVICE:
	{

	}
		break;
	default:
		CCLog("other:%d   %d<<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
		break;
	}
}