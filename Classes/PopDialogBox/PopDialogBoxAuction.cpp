/*
 * PopDialogBoxAuction.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxAuction.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../Tools/Tools.h"
#include "../Tools/BaseAttributes.h"
#include "PopDialogBoxLoading.h"

#include "../Network/ListernerThread/LogonGameListerner.h"
#include "../Network/MD5/MD5.h"
#define MAX_PAGE_SIZE		20			//每页最大数
//////////////////////////////////////////////////////////////////////////
PopDialogBoxAuction::PopDialogBoxAuction()
	:auctionItem(AUCTION_INFO)
	, eAgainGetData(AGAIN_MY_AUCTION_GOODS)
	, iAuctionSellIndex(-1)
	, isCurSearchInfo(false)
{
	scheduleUpdate();
}
PopDialogBoxAuction::~PopDialogBoxAuction() {
	CCLog("~ <<%s>>",__FUNCTION__);
	unscheduleUpdate();
	TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_AUCTION_INFO);
}
void PopDialogBoxAuction::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxAuction.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);

	//添加背景
	CCSize deviceSize = DataModel::sharedDataModel()->deviceSize;
	CCSprite *bg = CCSprite::create("res/shop_bg.jpg");
	pWidgetBg->addNode(bg, -1);
	bg->setPosition(CCPointZero);
	float scale = deviceSize.height / bg->getContentSize().height;
	bg->setScale(scale);

	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::onMenuBackWithReadMsg));
	//我的背包
	UIButton *pBPackage = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonPackage"));
	pBPackage->addTouchEventListener(this, toucheventselector(PopDialogBoxAuction::onMenupMyPackage));
	//开始拍卖
	UIButton *pBSell = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonSellAuction"));
	pBSell->addTouchEventListener(this, toucheventselector(PopDialogBoxAuction::onMenuSellAuctionGoods));

	//元宝
	pIBigGold = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageScoreIcon0"));
	//保险箱
	pIInsure= static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageScoreIcon1"));
	//拍卖
	pIAuction = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageScoreIcon2"));
	//上架物品名称
	pLAuctionGoodsName = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelGoodsName"));
	//上架物品数量
	pTFAuctionGoodsNum = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldAuctionGoodsNum"));
	//上架物品单价
	pTFAuctionGoodsPice = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldAuctionGoodsPice"));

	//搜索物品输入框
	pTFSearchByID = static_cast<UITextField*>(pUILayer->getWidgetByName("TextFieldSearchByID"));
	//搜索按键
	pBSearchByID = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonSearchByID"));
	pBSearchByID->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxAuction::onMenuSearchByID));

	//我的上架物品列表
	pLVMyAuction = static_cast<UIListView*>(pUILayer->getWidgetByName("ListViewMyAuction"));
	//设置cell样式
	pLVMyAuction->setItemModel(pLVMyAuction->getItem(0));
	pLVMyAuction->removeAllItems();
	
	for (int i = 0; i < MAX_AUCTION_ITEM_COUNT; i++)
	{
		//列表
		pLVAuction[i] = static_cast<UIListView*>(pUILayer->getWidgetByName(CCString::createWithFormat("ListViewAuction%d",i)->getCString()));
		//设置cell模式
		pLVAuction[i]->setItemModel(pLVAuction[i]->getItem(0));
		pLVAuction[i]->setTag(i);
		pLVAuction[i]->addEventListenerScrollView(this, SEL_ScrollViewEvent(&PopDialogBoxAuction::onScrollViewEvent));

		//单项内容
		pICellItemContent[i] = static_cast<UIImageView*>(pUILayer->getWidgetByName(CCString::createWithFormat("ImageCellItemContent%d", i)->getCString()));

		//线条
		pPLine[i] = static_cast<UIPanel*>(pUILayer->getWidgetByName(CCString::createWithFormat("PanelLine%d", i)->getCString()));

		//选项
		pCBAuctionItems[i] = static_cast<UICheckBox *>(pUILayer->getWidgetByName(CCString::createWithFormat("CheckBoxItem%d", i)->getCString()));
		pCBAuctionItems[i]->addEventListenerCheckBox(this, SEL_SelectedStateEvent(&PopDialogBoxAuction::onCheckBoxSelectedStateEvent));
	}

	//更新拍卖信息（赋0）
	updateListAuctionInfo();
	//更新我的拍卖()
	updateListMyAuctionRecord();
	//更新拍卖历史记录
	updateListHistoryAuctionRecord();

	pCBAuctionItems[0]->setSelectedState(true);
	onCheckBoxSelectedStateEvent(pCBAuctionItems[AUCTION_INFO], CHECKBOX_STATE_EVENT_SELECTED);
	

	setBigGold(DataModel::sharedDataModel()->userInfo->lIngotScore);
	setInsure(DataModel::sharedDataModel()->userInfo->lInsure);
	setAuction(DataModel::sharedDataModel()->userInfo->lIngot);
	

	playAnimation();

	
	setLobbyReadMessage(false);
}
void PopDialogBoxAuction::onExit(){
	CCLayer::onExit();
}
//设置元宝
void PopDialogBoxAuction::setBigGold(long long llGold){
	UILabel *pLNum = static_cast<UILabel*>(pIBigGold->getChildByName("LabelNum"));
	pLNum->setText(CCString::createWithFormat("%lld",llGold)->getCString());
	CCSize bigGoldSize = CCSize(pLNum->getContentSize().width + 60, pIBigGold->getContentSize().height);
	pIBigGold->setSize(bigGoldSize);
}
//设置保险箱
void PopDialogBoxAuction::setInsure(long long llInsure){
	UILabel *pLNum = static_cast<UILabel*>(pIInsure->getChildByName("LabelNum"));
	pLNum->setText(CCString::createWithFormat("%lld", llInsure)->getCString());
	CCSize bigGoldSize = CCSize(pLNum->getContentSize().width + 60, pIInsure->getContentSize().height);
	pIInsure->setSize(bigGoldSize);
}
//设置拍卖
void PopDialogBoxAuction::setAuction(long long llAuction){
	UILabel *pLNum = static_cast<UILabel*>(pIAuction->getChildByName("LabelNum"));
	pLNum->setText(CCString::createWithFormat("%lld", llAuction)->getCString());
	CCSize bigGoldSize = CCSize(pLNum->getContentSize().width + 60, pIAuction->getContentSize().height);
	pIAuction->setSize(bigGoldSize);

	UIButton *pBTakeOut = static_cast<UIButton*>(pIAuction->getChildByName("ButtonTakeOut"));
	CCPoint posTakeOut = ccpAdd(pLNum->getPosition(), ccp(pLNum->getContentSize().width + 60, 0));
	pBTakeOut->setPosition(posTakeOut);
}
//购买按键
void PopDialogBoxAuction::onMenuBuyProp(CCObject *object, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		UIButton *pTempButton = (UIButton*)object;
		iAuctionBuyIndex = pTempButton->getTag();
		showInputNumBox(BUY_AUCTION, GBKToUTF8(vecAuctionInfo[iAuctionBuyIndex].szAuctionName), "", vecAuctionInfo[iAuctionBuyIndex].dwPropNum, vecAuctionInfo[iAuctionBuyIndex].lGold,this);
	}
		break;
	default:
		break;
	}
}
//取消拍卖按键
void PopDialogBoxAuction::onMenuCancelAuction(CCObject *object, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		UIButton *pTempButton = (UIButton*)object;
		iAuctionBuyIndex = pTempButton->getTag();
		setAuctionItem(AUCTION_CANCEL);
		connectServer(SOCKET_AUCTION_INFO);
		//CCLog("auction:%d <<%s>>",auctionBuyIndex, __FUNCTION__);
		//showInputNumBox(BUY_AUCTION, GBKToUTF8(vecAuctionInfo[auctionBuyIndex].szAuctionName), "", vecAuctionInfo[auctionBuyIndex].dwPropNum, vecAuctionInfo[auctionBuyIndex].lGold, this);
	}
	break;
	default:
		break;
	}
}
//我的背包
void PopDialogBoxAuction::onMenupMyPackage(CCObject *object, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		isReadMessage = false;
		PopDialogBoxKnapsack *box = PopDialogBoxKnapsack::create();
		this->addChild(box, 10);
		box->setIPopAssistKnapsack(this);
	}
	break;
	default:
		break;
	}
}
//开始拍卖按键
void PopDialogBoxAuction::onMenuSellAuctionGoods(CCObject *object, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		if (strcmp(pLAuctionGoodsName->getStringValue(),"")==0)
		{
			showTipInfo(BaseAttributes::sharedAttributes()->sSelectAuctionGoods.c_str());
		}
		else if (strcmp(pTFAuctionGoodsNum->getStringValue(), "") == 0)
		{
			showTipInfo(BaseAttributes::sharedAttributes()->sInputAuctionGoodsNum.c_str());
		}
		else if (strcmp(pTFAuctionGoodsPice->getStringValue(), "") == 0)
		{
			showTipInfo(BaseAttributes::sharedAttributes()->sInputAuctionGoodsPice.c_str());
		}
		else
		{
			setAuctionItem(AUCTION_SELL_AUCTION);
			connectServer(SOCKET_AUCTION_INFO);
		}
	}
	break;
	default:
		break;
	}
}
//选择我的上架商品
void PopDialogBoxAuction::onMenuSelectMyAuctionCell(CCObject *object, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		UIImageView *pIVCell = (UIImageView*)object;
		for (int i = 0; i < vecMyAuctionGoods.size(); i++)
		{
			UIImageView *pIVTempCell = static_cast<UIImageView*>(pLVMyAuction->getItem(i));
			if (pIVCell->getTag()==i)
			{
				iAuctionSellIndex = i;
				pIVTempCell->setOpacity(255);
				pLAuctionGoodsName->setText(GBKToUTF8(vecMyAuctionGoods[i].szName));
				if (strcmp(pTFAuctionGoodsNum->getStringValue(),"")==0)
				{
					pTFAuctionGoodsNum->setText("1");
				}
				if (strcmp(pTFAuctionGoodsPice->getStringValue(), "") == 0)
				{
					pTFAuctionGoodsPice->setText("1000");
				}
			}
			else
			{
				pIVTempCell->setOpacity(30);
			}
		}
		CCLog("%d <<%s>>",pIVCell->getTag(), __FUNCTION__);
	}
	break;
	default:
		break;
	}
}
//搜索按键
void PopDialogBoxAuction::onMenuSearchByID(CCObject *object, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		if (strcmp(pTFSearchByID->getStringValue(),"")!=0)
		{
			vecAuctionInfo.clear();
			iCurPage[AUCTION_INFO] = 1;
			isCurSearchInfo = true;
			setAuctionItem(AUCTION_SEARCH);
			connectServer(SOCKET_AUCTION_INFO);
		}
		else
		{
			//pTFSearchByID->setPlaceHolder("输入ID或昵称 ");
		}
	}
	break;
	default:
		break;
	}
}
//购买数量回调
void PopDialogBoxAuction::onBuyNum(long lNum){
	lCurBuyNum = lNum;
	setAuctionItem(AUCTION_BUY);
	connectServer(SOCKET_AUCTION_INFO);
}
//关闭背包回调
void PopDialogBoxAuction::onCloseKnapsack(){
	isReadMessage = true;
}
//滚动回调
void PopDialogBoxAuction::onScrollViewEvent(CCObject*obj, ScrollviewEventType type){
	switch (type)
	{
	case SCROLLVIEW_EVENT_BOUNCE_BOTTOM:
	{
		UIListView *pLVTemp = (UIListView*)obj;
		int index = pLVTemp->getTag();
		if (iCurPage[index]<iPageCount[index])
		{
			iCurPage[index]++;
			if (isCurSearchInfo)
			{
				setAuctionItem(AUCTION_SEARCH);
			}
			else
			{
				setAuctionItem((AuctionItem)index);
			}
			
			connectServer(SOCKET_AUCTION_INFO);
		}
	}
		
		break;
	default:
		break;
	}
}
//复选框回调
void PopDialogBoxAuction::onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type){
	switch (type)
	{
	case CHECKBOX_STATE_EVENT_SELECTED:
	{
		pTFSearchByID->setText("");
		UICheckBox *box = (UICheckBox*)pSender;
		int index = box->getTag() - 1;
		
		for (int i = 0; i < MAX_AUCTION_ITEM_COUNT; i++)
		{
			UILabel *pItemName = static_cast<UILabel*>(pCBAuctionItems[i]->getChildByName("LabelItemName"));
			if (index != i)
			{
				pItemName->setColor(ccc3(253, 205, 0));
				pItemName->setScale(1.0);
				pCBAuctionItems[i]->setSelectedState(false);
				pCBAuctionItems[i]->setTouchEnabled(true);
			}
			else
			{
				pItemName->setColor(ccc3(255, 255, 255));
				pItemName->setScale(1.2);
				//pCBAuctionItems[i]->setTouchEnabled(false);
				changeSelectItem((AuctionItem)index);
			}
		}
	}
	break;
	case CHECKBOX_STATE_EVENT_UNSELECTED:
	{
		pTFSearchByID->setText("");
		UICheckBox *box = (UICheckBox*)pSender;
		int index = box->getTag() - 1;
		if (!pCBAuctionItems[index]->getSelectedState())
		{
			pCBAuctionItems[index]->setSelectedState(true);
			onCheckBoxSelectedStateEvent(box, CHECKBOX_STATE_EVENT_SELECTED);
		}
	}
	break;
	default:
		break;
	}
}
//切换拍卖项
void PopDialogBoxAuction::changeSelectItem(AuctionItem eItem){
	setAuctionItem(eItem);
	for (int i = 0; i < MAX_AUCTION_ITEM_COUNT; i++)
	{
		if (i == auctionItem)
		{
			pICellItemContent[i]->setVisible(true);
			//pPLine[i]->setEnabled(true);
			pPLine[i]->setVisible(true);

			pLVAuction[i]->setEnabled(true);
			pLVAuction[i]->setVisible(true);
		}
		else
		{
			

			pICellItemContent[i]->setVisible(false);
			//pPLine[i]->setEnabled(false);
			pPLine[i]->setVisible(false);

			pLVAuction[i]->setEnabled(false);
			pLVAuction[i]->setVisible(false);
		}
	}


	switch (auctionItem)
	{
	case PopDialogBoxAuction::AUCTION_INFO:
	{
		showSearch(true);
		iCurPage[AUCTION_INFO] = 1;
		vecAuctionInfo.clear();
		isCurSearchInfo = false;
		connectServer(SOCKET_AUCTION_INFO);
	}
		break;
	case PopDialogBoxAuction::AUCTION_MY:
	{
		showSearch(false);
		iCurPage[AUCTION_MY] = 1;
		vecMyAuction.clear();
		connectServer(SOCKET_AUCTION_INFO);
	}
		break;
	case PopDialogBoxAuction::AUCTION_RECORD:
	{
		showSearch(false);
		iCurPage[AUCTION_RECORD] = 1;
		vecHistoryAuction.clear();
		connectServer(SOCKET_AUCTION_INFO);
	}
		break;
	default:
		break;
	}

	
	
}
//显示搜索
void PopDialogBoxAuction::showSearch(bool isShow){
	pTFSearchByID->getParent()->setVisible(isShow);
	pTFSearchByID->setEnabled(isShow);
	pBSearchByID->setEnabled(isShow);
}
//更新拍卖信息列表
void PopDialogBoxAuction::updateListAuctionInfo(){
	UIListView *pLVTemp = pLVAuction[AUCTION_INFO];

	pLVTemp->removeAllItems();
	int tempSize = vecAuctionInfo.size();
	if (tempSize == 0)
	{
		return;
	}

	for (int i = 0; i < tempSize ; i++)
	{
		int inserterPos = pLVTemp->getItems()->count();
		pLVTemp->insertDefaultItem(inserterPos);
		//拍卖物品名称
		UILabel *pGoodsName = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("Label0"));
		pGoodsName->setText(GBKToUTF8(vecAuctionInfo[i].szAuctionName));
		//addDownloadImage(pGoodsName,vecAuctionInfo[i].)
		//拍卖数量
		UILabel *pGoodsCount = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("Label1"));
		pGoodsCount->setText(CCString::createWithFormat("%ld",vecAuctionInfo[i].dwPropNum)->getCString());
		//卖家
		UILabel *pSeller = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("Label2"));
		std::string nickName = GBKToUTF8(vecAuctionInfo[i].szNickName);
		CCString *pSSellerNick = CCString::createWithFormat("%s\nID:%ld", Tools::subUTF8(nickName, 0, 4).c_str(), vecAuctionInfo[i].dwGameID);
		pSeller->setText(pSSellerNick->getCString());
		//价格
		UILabel *pPice = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("Label3"));
		pPice->setText(CCString::createWithFormat("%lld", vecAuctionInfo[i].lGold)->getCString());
		//购买按键
		UIButton *pButton = static_cast<UIButton*>(pLVTemp->getItem(inserterPos)->getChildByName("ButtonBuy"));
		pButton->setTag(inserterPos);
		pButton->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxAuction::onMenuBuyProp));
	}
}
// 更新我的列表
void PopDialogBoxAuction::updateListMyAuctionRecord(){
	UIListView *pLVTemp = pLVAuction[AUCTION_MY];
	pLVTemp->removeAllItems();

	int tempSize = vecMyAuction.size();
	if (tempSize == 0)
	{
		return;
	}

	for (int i = 0; i < tempSize; i++)
	{
		int inserterPos = pLVTemp->getItems()->count();
		pLVTemp->insertDefaultItem(inserterPos);
		//拍卖物品名称
		UILabel *pGoodsName = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("Label0"));
		pGoodsName->setText(GBKToUTF8(vecMyAuction[i].szAuctionName));
		//拍卖数量
		UILabel *pGoodsCount = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("Label1"));
		pGoodsCount->setText(CCString::createWithFormat("%ld", vecMyAuction[i].dwPropNum)->getCString());
		//价格
		UILabel *pPice = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("Label2"));
		pPice->setText(CCString::createWithFormat("%lld", vecMyAuction[i].lGold)->getCString());
		//按键
		UIButton *pButton = static_cast<UIButton*>(pLVTemp->getItem(inserterPos)->getChildByName("ButtonSell"));
		pButton->setTag(inserterPos);
		pButton->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxAuction::onMenuCancelAuction));
	}
}
//更新拍卖历史记录列表
void PopDialogBoxAuction::updateListHistoryAuctionRecord(){
	UIListView *pLVTemp = pLVAuction[AUCTION_RECORD];
	pLVTemp->removeAllItems();

	int tempSize = vecHistoryAuction.size();
	if (tempSize == 0)
	{
		return;
	}

	for (int i = 0; i < tempSize; i++)
	{
		int inserterPos = pLVTemp->getItems()->count();
		pLVTemp->insertDefaultItem(inserterPos);
		//拍卖物品名称
		UILabel *pGoodsName = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("Label0"));
		pGoodsName->setText(GBKToUTF8(vecHistoryAuction[i].szAuctionName));
		//状态
		UILabel *pAuctionState = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("Label1"));
		pAuctionState->setText(vecHistoryAuction[i].dwType == 1 ? BaseAttributes::sharedAttributes()->sAuctionBuy.c_str() : BaseAttributes::sharedAttributes()->sAuctionSell.c_str());
		CCLog("%ld <<%s>>",vecHistoryAuction[i].dwType, __FUNCTION__);
		CCLog(" <<%s>>", __FUNCTION__);
		/*//拍卖数量
		UILabel *pGoodsCount = static_cast<UILabel*>(pLVAuction[0]->getItem(inserterPos)->getChildByName("Label1"));
		pGoodsCount->setText(CCString::createWithFormat("%ld", vecAuctionInfo[i].dwPropNum)->getCString());*/
		//卖家
		UILabel *pSeller = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("Label2"));
		std::string nickName = GBKToUTF8(vecHistoryAuction[i].szNickName);
		CCString *pSSellerNick = CCString::createWithFormat("%s%s\nID:%ld",
			vecHistoryAuction[i].dwType == 1 ? BaseAttributes::sharedAttributes()->sAuctionSeller.c_str() : BaseAttributes::sharedAttributes()->sAuctionBuyers.c_str(),
			Tools::subUTF8(nickName, 0, 4).c_str(), vecHistoryAuction[i].dwGameID);
		pSeller->setText(pSSellerNick->getCString());
		//时间
		UILabel *pTime = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("Label3"));
		CCString *pStrTime = CCString::createWithFormat("%d-%02d-%02d\n%02d::%02d::%02d",
			vecHistoryAuction[i].tOpDate.wYear,
			vecHistoryAuction[i].tOpDate.wMonth,
			vecHistoryAuction[i].tOpDate.wDay,
			vecHistoryAuction[i].tOpDate.wHour,
			vecHistoryAuction[i].tOpDate.wMinute,
			vecHistoryAuction[i].tOpDate.wSecond
			);
		pTime->setText(pStrTime->getCString());
		//数量
		UILabel *pLNum = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("Label4"));
		pLNum->setText(CCString::createWithFormat("%ld", vecHistoryAuction[i].dwPropNum)->getCString());
		//价格
		UILabel *pPice = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("Label5"));
		pPice->setText(CCString::createWithFormat("%lld", vecHistoryAuction[i].lGold)->getCString());
	}
}
//更新玩家上架商品列表
void PopDialogBoxAuction::updateListMyAuctionGoods(){
	UIListView *pLVTemp = pLVMyAuction;
	pLVTemp->removeAllItems();

	int tempSize = vecMyAuctionGoods.size();
	if (tempSize == 0)
	{
		return;
	}



	pLAuctionGoodsName->setText("");
	for (int i = 0; i < tempSize; i++)
	{
		int inserterPos = pLVTemp->getItems()->count();
		pLVTemp->insertDefaultItem(inserterPos);
		//拍卖物品名称
		UILabel *pGoodsName = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("LabelGoodsContent"));
		CCString *strGoodInfo = CCString::createWithFormat("X%ld %s", vecMyAuctionGoods[i].dwNum, GBKToUTF8(vecMyAuctionGoods[i].szName));
		pGoodsName->setText(strGoodInfo->getCString());

		UIImageView *pIVCell=static_cast<UIImageView*>(pLVTemp->getItem(inserterPos));
		if (i==iAuctionSellIndex)
		{
			pIVCell->setOpacity(255);
			pLAuctionGoodsName->setText(GBKToUTF8(vecMyAuctionGoods[i].szName));
		}
		else
		{
			pIVCell->setOpacity(30);
		}
		UIImageView *pIVGoodsIcon = static_cast<UIImageView*>(pLVTemp->getItem(inserterPos)->getChildByName("ImageGoodsIcon"));
		addDownloadImage(pIVGoodsIcon, vecMyAuctionGoods[i].szImgName, CCPointZero, 0.3, -100, false);

		pIVCell->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxAuction::onMenuSelectMyAuctionCell));
		pIVCell->setTag(i);
	}
}
//////////////////////////////////////////////////////////////////////////
void PopDialogBoxAuction::update(float delta){
	if (isReadMessage)
	{
		MessageQueue::update(delta);
	}
}
//读取网络消息回调
void PopDialogBoxAuction::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_MB_SOCKET://socket连接成功
	{
		connectSuccess();
	}
		break;
	case MDM_GP_USER_SERVICE:
	{
		onEventUserService(wSubCmdID, pDataBuffer, wDataSize);
	}
		break;
	default:
		CCLog("sub:%d <<%s>>", wSubCmdID, __FUNCTION__);
		break;
	}
}
//连接成功
void PopDialogBoxAuction::connectSuccess(){
	switch (auctionItem)
	{
	case PopDialogBoxAuction::AUCTION_INFO:
	{
		CMD_GP_GetAuctionRecord qAuction;
 		qAuction.dwPage = iCurPage[AUCTION_INFO];
 		qAuction.dwPageSize = MAX_PAGE_SIZE;
		qAuction.dwUserID = 0;
 		qAuction.dwLastDay=12;
		getSocket()->SendData(MDM_GP_USER_SERVICE, SUB_GP_AUCTION_RECORD, &qAuction, sizeof(qAuction));
	}
		break;
	case PopDialogBoxAuction::AUCTION_MY:
	{
		CMD_GP_GetAuctionRecord qAuction;
		qAuction.dwPage = iCurPage[AUCTION_MY];
		qAuction.dwPageSize = MAX_PAGE_SIZE;
		qAuction.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;
		qAuction.dwLastDay = 12;
		getSocket()->SendData(MDM_GP_USER_SERVICE, SUB_GP_MYAUCTION_RECORD, &qAuction, sizeof(qAuction));
	}
		break;
	case PopDialogBoxAuction::AUCTION_RECORD:
	{
		CMD_GP_GetAuctionRecord qAuction;
		qAuction.dwPage = iCurPage[AUCTION_RECORD];
		qAuction.dwPageSize = MAX_PAGE_SIZE;
		qAuction.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;
		qAuction.dwLastDay = 12;
		getSocket()->SendData(MDM_GP_USER_SERVICE, SUB_GP_AUCTION_HISTORY_RECORD, &qAuction, sizeof(qAuction));
	}
		break;
	case PopDialogBoxAuction::AUCTION_BUY:
	{
		CMD_GP_Buy_Auction buyAuction;
		buyAuction.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;
		buyAuction.dwOpTerminal = 2;

		MD5 m;
		m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(), DataModel::sharedDataModel()->sLogonPassword.length());
		std::string md5PassWord = m.GetMd5();
		strcpy(buyAuction.szLogonPass, md5PassWord.c_str());

		buyAuction.dwID = vecAuctionInfo[iAuctionBuyIndex].dwIndex;
		buyAuction.dwNum = lCurBuyNum;

		strcpy(buyAuction.szMachineID, "12");
		getSocket()->SendData(MDM_GP_USER_SERVICE, SUB_GP_BUY_AUCTION, &buyAuction, sizeof(buyAuction));
	}
		break;
	case PopDialogBoxAuction::AUCTION_CANCEL://取消拍卖
	{
		CMD_GP_Cancel_Auction cancel;
		cancel.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;
		cancel.dwOpTerminal = 2;

		MD5 m;
		m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(), DataModel::sharedDataModel()->sLogonPassword.length());
		std::string md5PassWord = m.GetMd5();
		strcpy(cancel.szLogonPass, md5PassWord.c_str());

		cancel.dwID = vecMyAuction[iAuctionBuyIndex].dwIndex;

		strcpy(cancel.szMachineID, "12");
		getSocket()->SendData(MDM_GP_USER_SERVICE, SUB_GP_CANCEL_AUCTION, &cancel, sizeof(cancel));
	}
		break;
	case AUCTION_MY_GOODS://我的拍卖物品列表
	{
		CMD_GP_UserID gpUserID;
		gpUserID.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;

		MD5 m;
		m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(), DataModel::sharedDataModel()->sLogonPassword.length());
		std::string md5PassWord = m.GetMd5();
		strcpy(gpUserID.szPassword, md5PassWord.c_str());
		
		bool isSend=getSocket()->SendData(MDM_GP_USER_SERVICE, SUB_GP_AUCTION, &gpUserID, sizeof(gpUserID));
		CCLog("send <<%s>>", __FUNCTION__);
	}
		break;
	case AUCTION_SELL_AUCTION://卖拍卖品
	{
		CMD_GP_Sell_Auction sellAuction;
		sellAuction.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;
		sellAuction.dwOpTerminal = 2;

		MD5 m;
		m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(), DataModel::sharedDataModel()->sLogonPassword.length());
		std::string md5PassWord = m.GetMd5();
		strcpy(sellAuction.szLogonPass, md5PassWord.c_str());

		sellAuction.dwID = vecMyAuctionGoods[iAuctionSellIndex].dwID;
		sellAuction.lGold = strtoll(pTFAuctionGoodsPice->getStringValue(),NULL,10);
		sellAuction.dwNum = strtol(pTFAuctionGoodsNum->getStringValue(), NULL, 10);

		strcpy(sellAuction.szMachineID, "12");
		getSocket()->SendData(MDM_GP_USER_SERVICE, SUB_GP_SELL_AUCTION, &sellAuction, sizeof(sellAuction));
	}
		break;
	case AUCTION_SEARCH://搜索
	{
		//查找拍卖记录
		CMD_GP_Query_Auction queryAuction;
		strcpy(queryAuction.szID, UTF8ToGBK(pTFSearchByID->getStringValue()));
		queryAuction.dwPage = iCurPage[AUCTION_INFO];
		queryAuction.dwPageSize = MAX_PAGE_SIZE;
		queryAuction.dwLastDay = 1;
		getSocket()->SendData(MDM_GP_USER_SERVICE, SUB_GP_QUERY_AUCTION, &queryAuction, sizeof(queryAuction));
	}
		break;
	default:
		break;
	}
}
//消息处理完成
void PopDialogBoxAuction::messageFinish(){
	//移除loading
	this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
	//关闭网络
	TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_AUCTION_INFO);
}

//用户服务
void PopDialogBoxAuction::onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_AUCTION_RECORD://拍卖记录
		onSubAuctionInfo(pDataBuffer, wDataSize);
		break;
	case SUB_GP_MYAUCTION_RECORD://我的拍卖
		onSubMyAuction(pDataBuffer, wDataSize);
		break;
	case SUB_GP_AUCTION_HISTORY_RECORD://拍卖历史记录
		onSubHistoryAuctionRecord(pDataBuffer,wDataSize);
		break;
	case SUB_GP_BUY_AUCTION://购买拍卖品
		onSubBuyAuction(pDataBuffer, wDataSize);
		break;
	case SUB_GP_OPERATE_FAILURE://操作失败
	{
		CMD_GP_UserInsureFailure * pUserInsureFailure = (CMD_GP_UserInsureFailure *)pDataBuffer;
		assert(wDataSize >= (sizeof(CMD_GP_UserInsureFailure) - sizeof(pUserInsureFailure->szDescribeString)));
		if (wDataSize < (sizeof(CMD_GP_UserInsureFailure) - sizeof(pUserInsureFailure->szDescribeString))) return;

		showTipInfo(GBKToUTF8(pUserInsureFailure->szDescribeString));
	}
		break;
	case SUB_GP_CANCEL_AUCTION://取消拍卖
	{
		onSubCancelAuction(pDataBuffer, wDataSize);
	}
		break;
	case SUB_GP_AUCTION://我的拍卖物品
	{
		int size = sizeof(CMD_GP_Auction);

		BYTE cbDataBuffer[SOCKET_TCP_PACKET + sizeof(TCP_Head)];
		CopyMemory(cbDataBuffer, pDataBuffer, wDataSize);
		vecMyAuctionGoods.clear();
		for (int i = 0; i < wDataSize / size; i++)
		{
			CMD_GP_Auction *gpAuction = (CMD_GP_Auction*)(cbDataBuffer + i*sizeof(CMD_GP_Auction));
			vecMyAuctionGoods.push_back(*gpAuction);
		}
		updateListMyAuctionGoods();
	}
		break;
	case SUB_GP_AUCTIONLOG:
	{
		
		CMD_GP_AuctionLog *Log = (CMD_GP_AuctionLog *)pDataBuffer;
		CCLog(" <<%s>>", __FUNCTION__);
		//showTipInfo(GBKToUTF8(Log->szDescribeString));
	}
		break;
	case SUB_GP_SELL_AUCTION://出售拍卖品
	{
		onSubSellAuction(pDataBuffer,wDataSize);
	}
		break;
	case SUB_GP_QUERY_AUCTION://搜索
	{
		onSubAuctionInfo(pDataBuffer, wDataSize);
	}
		break;
	default:
		CCLog("sub:%d <<%s>>", wSubCmdID, __FUNCTION__);
		break;
	}
	//关掉scoket
	messageFinish();

	switch (eAgainGetData)
	{
	case PopDialogBoxAuction::AGAIN_NOTHING:
		break;
	case PopDialogBoxAuction::AGAIN_MY_AUCTION_GOODS:
	{
		setAuctionItem(AUCTION_MY_GOODS);
		connectServer(SOCKET_AUCTION_INFO);
	}
		break;
	case PopDialogBoxAuction::AGAIN_AUCTION_INFO:
	{
		onCheckBoxSelectedStateEvent(pCBAuctionItems[AUCTION_INFO], CHECKBOX_STATE_EVENT_SELECTED);
	}
		break;
	case PopDialogBoxAuction::AGAIN_MY_AUCTION:
	{
		onCheckBoxSelectedStateEvent(pCBAuctionItems[AUCTION_MY], CHECKBOX_STATE_EVENT_SELECTED);
	}
		break;
	default:
		break;
	}
	setAgainGetData(AGAIN_NOTHING);
}
//拍卖记录
void PopDialogBoxAuction::onSubAuctionInfo(void * pDataBuffer, unsigned short wDataSize){
	CMD_GP_AuctionRecord <CMD_GP_AuctionRecordItem> *gpAuctionRecord;
	assert(wDataSize>=sizeof(gpAuctionRecord));
	gpAuctionRecord = (CMD_GP_AuctionRecord<CMD_GP_AuctionRecordItem> *)pDataBuffer;
	for (int i = 0; i < gpAuctionRecord->dwIndex; i++)
	{
		vecAuctionInfo.push_back(gpAuctionRecord->RecordItem[i]);
	}
	iPageCount[AUCTION_INFO] = gpAuctionRecord->dwPageCount;
	updateListAuctionInfo();
}
//我的拍卖
void PopDialogBoxAuction::onSubMyAuction(void * pDataBuffer, unsigned short wDataSize){
	CMD_GP_AuctionRecord <CMD_GP_MyAuctionRecordItem> *gpAuctionRecord;
	assert(wDataSize >= sizeof(gpAuctionRecord));
	gpAuctionRecord = (CMD_GP_AuctionRecord<CMD_GP_MyAuctionRecordItem> *)pDataBuffer;
	for (int i = 0; i < gpAuctionRecord->dwIndex; i++)
	{
		vecMyAuction.push_back(gpAuctionRecord->RecordItem[i]);
	}
	iPageCount[AUCTION_MY] = gpAuctionRecord->dwPageCount;
	updateListMyAuctionRecord();
}
//拍卖历史记录
void  PopDialogBoxAuction::onSubHistoryAuctionRecord(void * pDataBuffer, unsigned short wDataSize){
	CMD_GP_AuctionRecord <CMD_GP_HistoryAuctionRecordItem> *gpAuctionRecord;
	assert(wDataSize >= sizeof(gpAuctionRecord));
	gpAuctionRecord = (CMD_GP_AuctionRecord<CMD_GP_HistoryAuctionRecordItem> *)pDataBuffer;
	for (int i = 0; i < gpAuctionRecord->dwIndex; i++)
	{
		vecHistoryAuction.push_back(gpAuctionRecord->RecordItem[i]);
	}
	iPageCount[AUCTION_RECORD] = gpAuctionRecord->dwPageCount;
	updateListHistoryAuctionRecord();
}

//购买拍卖品
void PopDialogBoxAuction::onSubBuyAuction(void * pDataBuffer, unsigned short wDataSize){
	int size = sizeof(CMD_GP_Buy_AuctionLog);
	assert(wDataSize <= sizeof(CMD_GP_Buy_AuctionLog));
	CMD_GP_Buy_AuctionLog *pBuyAuction = (CMD_GP_Buy_AuctionLog *)pDataBuffer;
	if (pBuyAuction->dwRet==0)
	{
		vecAuctionInfo[iAuctionBuyIndex].dwPropNum -= lCurBuyNum;
		if (vecAuctionInfo[iAuctionBuyIndex].dwPropNum <= 0)
		{
			vector< CMD_GP_AuctionRecordItem >::iterator k = vecAuctionInfo.begin() + iAuctionBuyIndex;
			vecAuctionInfo.erase(k);
		}
		updateListAuctionInfo();

		setAgainGetData(AGAIN_MY_AUCTION_GOODS);
	}
	else
	{
		setAgainGetData(AGAIN_AUCTION_INFO);
		//onCheckBoxSelectedStateEvent(pCBAuctionItems[AUCTION_INFO], CHECKBOX_STATE_EVENT_SELECTED);
	}
	showTipInfo(GBKToUTF8(pBuyAuction->szDescribeString));
}
//出售拍卖品
void PopDialogBoxAuction::onSubSellAuction(void * pDataBuffer, unsigned short wDataSize){
	
	int size = sizeof(CMD_GP_Sell_AuctionLog);
	assert(wDataSize <= sizeof(CMD_GP_Sell_AuctionLog));
	CMD_GP_Sell_AuctionLog *pSellAuction = (CMD_GP_Sell_AuctionLog *)pDataBuffer;
	if (pSellAuction->dwRet==0)
	{
		vecMyAuctionGoods[iAuctionSellIndex].dwNum -= pSellAuction->dwNum;
		if (vecMyAuctionGoods[iAuctionSellIndex].dwNum<=0)
		{
			vector< CMD_GP_Auction >::iterator k = vecMyAuctionGoods.begin() + iAuctionSellIndex;
			vecMyAuctionGoods.erase(k);
			//pLAuctionGoodsName->setText("");
			iAuctionSellIndex = -1;
		}
		else
		{
			//pLAuctionGoodsName->setText("");
			//iAuctionSellIndex = -1;
		}
		updateListMyAuctionGoods();
		if (pCBAuctionItems[AUCTION_INFO]->getSelectedState())
		{
			//更新信息列表
			setAgainGetData(AGAIN_AUCTION_INFO);
		}
		else if (pCBAuctionItems[AUCTION_MY]->getSelectedState())
		{
			//更新撤销列表
			setAgainGetData(AGAIN_MY_AUCTION);
		}
		
		
	}
	else
	{

	}
	showTipInfo(GBKToUTF8(pSellAuction->szDescribeString));
	CCLog(" <<%s>>", __FUNCTION__);
}
//取消拍卖
void PopDialogBoxAuction::onSubCancelAuction(void * pDataBuffer, unsigned short wDataSize){
	int size = sizeof(CMD_GP_Cancel_AuctionLog);
	CMD_GP_Cancel_AuctionLog * pCencelAuction = (CMD_GP_Cancel_AuctionLog *)pDataBuffer;
	//assert(wDataSize >= (sizeof(CMD_GP_Cancel_AuctionLog) - sizeof(pCencelAuction->szDescribeString)));
	//if (wDataSize < (sizeof(CMD_GP_Cancel_AuctionLog) - sizeof(pCencelAuction->szDescribeString))) return;
	assert(wDataSize <= sizeof(CMD_GP_Cancel_AuctionLog));
	if (pCencelAuction->dwRet==0)
	{
		for (int i = 0; i < vecMyAuction.size(); i++)
		{
			if (vecMyAuction[i].dwIndex==pCencelAuction->dwID)
			{
				vector< CMD_GP_MyAuctionRecordItem >::iterator k = vecMyAuction.begin() + i;
				vecMyAuction.erase(k);
				break;
			}
		}
		updateListMyAuctionRecord();
		//更新背包上架物品////////////////////////////////////////////////////////////////////////
		setAgainGetData(AGAIN_MY_AUCTION_GOODS);
	}
	else
	{

	}
	showTipInfo(GBKToUTF8(pCencelAuction->szDescribeString));
}