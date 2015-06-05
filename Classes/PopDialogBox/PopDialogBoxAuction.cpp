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
//////////////////////////////////////////////////////////////////////////
PopDialogBoxAuction::PopDialogBoxAuction()
	:auctionItem(AUCTION_INFO)
{
	scheduleUpdate();
}
PopDialogBoxAuction::~PopDialogBoxAuction() {
	CCLog("~ <<%s>>",__FUNCTION__);
	unscheduleUpdate();
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
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	//元宝
	pIBigGold = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageScoreIcon0"));
	//保险箱
	pIInsure= static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageScoreIcon1"));
	//拍卖
	pIAuction = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageScoreIcon2"));
	
	
	for (int i = 0; i < MAX_AUCTION_ITEM_COUNT; i++)
	{
		//列表
		pLVAuction[i] = static_cast<UIListView*>(pUILayer->getWidgetByName(CCString::createWithFormat("ListViewAuction%d",i)->getCString()));
		//设置cell模式
		pLVAuction[i]->setItemModel(pLVAuction[i]->getItem(0));

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
	onCheckBoxSelectedStateEvent(pCBAuctionItems[0], CHECKBOX_STATE_EVENT_SELECTED);

	setBigGold(112);
	setInsure(123456);
	setAuction(0);
	

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
		auctionBuyIndex = pTempButton->getTag();
		showInputNumBox(BUY_AUCTION, GBKToUTF8(vecAuctionInfo[auctionBuyIndex].szAuctionName), "", vecAuctionInfo[auctionBuyIndex].dwPropNum, vecAuctionInfo[auctionBuyIndex].lGold,this);
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
		auctionBuyIndex = pTempButton->getTag();
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
//购买数量回调
void PopDialogBoxAuction::onBuyNum(long lNum){
	lCurBuyNum = lNum;
	setAuctionItem(AUCTION_BUY);
	connectServer(SOCKET_AUCTION_INFO);
}
//复选框回调
void PopDialogBoxAuction::onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type){
	switch (type)
	{
	case CHECKBOX_STATE_EVENT_SELECTED:
	{
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
				pCBAuctionItems[i]->setTouchEnabled(false);
				changeSelectItem((AuctionItem)index);
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
		if (vecAuctionInfo.size()<=0)
		{
			connectServer(SOCKET_AUCTION_INFO);
		}
		else
		{
		}
	}
		break;
	case PopDialogBoxAuction::AUCTION_MY:
	{
		if (vecMyAuction.size() <= 0)
		{
			connectServer(SOCKET_AUCTION_INFO);
		}
		else
		{
		}
	}
		break;
	case PopDialogBoxAuction::AUCTION_RECORD:
	{
		if (vecHistoryAuction.size() <= 0)
		{
			connectServer(SOCKET_AUCTION_INFO);
		}
		else
		{
		}
	}
		break;
	default:
		break;
	}

	
	
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
		//购买按键
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

//////////////////////////////////////////////////////////////////////////
void PopDialogBoxAuction::update(float delta){
	MessageQueue::update(delta);
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
 		qAuction.dwPage = 1;
 		qAuction.dwPageSize = 20;
		qAuction.dwUserID = 0;
 		qAuction.dwLastDay=12;
		getSocket()->SendData(MDM_GP_USER_SERVICE, SUB_GP_AUCTION_RECORD, &qAuction, sizeof(qAuction));
	}
		break;
	case PopDialogBoxAuction::AUCTION_MY:
	{
		CMD_GP_GetAuctionRecord qAuction;
		qAuction.dwPage = 1;
		qAuction.dwPageSize = 20;
		qAuction.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;
		qAuction.dwLastDay = 12;
		getSocket()->SendData(MDM_GP_USER_SERVICE, SUB_GP_MYAUCTION_RECORD, &qAuction, sizeof(qAuction));
	}
		break;
	case PopDialogBoxAuction::AUCTION_RECORD:
	{
		CMD_GP_GetAuctionRecord qAuction;
		qAuction.dwPage = 1;
		qAuction.dwPageSize = 20;
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

		buyAuction.dwID = vecAuctionInfo[auctionBuyIndex].dwIndex;
		buyAuction.dwNum = lCurBuyNum;

		strcpy(buyAuction.szMachineID, "12");
		getSocket()->SendData(MDM_GP_USER_SERVICE, SUB_GP_BUY_AUCTION, &buyAuction, sizeof(buyAuction));
	}
		break;
	case PopDialogBoxAuction::AUCTION_CANCEL://取消拍卖
	{
		/*
		struct 
		{
			DWORD			    dwUserID;								//id
			DWORD				dwOpTerminal;							//操作终端（1：pc, 2：手机）
			TCHAR				szLogonPass[LEN_MD5];					//登录密码
			DWORD				dwID;									//礼包或道具id
			TCHAR				szMachineID[LEN_MACHINE_ID];			//机器序列*/
		CMD_GP_Cancel_Auction cancel;
		cancel.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;
		cancel.dwOpTerminal = 2;

		MD5 m;
		m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(), DataModel::sharedDataModel()->sLogonPassword.length());
		std::string md5PassWord = m.GetMd5();
		strcpy(cancel.szLogonPass, md5PassWord.c_str());

		cancel.dwID = vecMyAuction[auctionBuyIndex].dwIndex;

		strcpy(cancel.szMachineID, "12");
		getSocket()->SendData(MDM_GP_USER_SERVICE, SUB_GP_CANCEL_AUCTION, &cancel, sizeof(cancel));
	}
		break;
	default:
		break;
	}
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
		int size = sizeof(CMD_GP_Cancel_AuctionLog);
		CMD_GP_Cancel_AuctionLog * pCencelAuction = (CMD_GP_Cancel_AuctionLog *)pDataBuffer;
		assert(wDataSize >= (sizeof(CMD_GP_Cancel_AuctionLog) - sizeof(pCencelAuction->szDescribeString)));
		if (wDataSize < (sizeof(CMD_GP_Cancel_AuctionLog) - sizeof(pCencelAuction->szDescribeString))) return;
		showTipInfo(GBKToUTF8(pCencelAuction->szDescribeString));
	}
		break;
	default:
		CCLog("sub:%d <<%s>>", wSubCmdID, __FUNCTION__);
		break;
	}
	//移除loading
	this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
	//关闭网络
	TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_AUCTION_INFO);
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
		CCLog("--:::::%ld <<%s>>", gpAuctionRecord->RecordItem[i].dwType, __FUNCTION__);
	}
	updateListHistoryAuctionRecord();
}

//购买拍卖品
void PopDialogBoxAuction::onSubBuyAuction(void * pDataBuffer, unsigned short wDataSize){
	int size = sizeof(CMD_GP_Buy_AuctionLog);
	assert(wDataSize <= sizeof(CMD_GP_Buy_AuctionLog));
	CMD_GP_Buy_AuctionLog *pBuyAuction = (CMD_GP_Buy_AuctionLog *)pDataBuffer;
	if (pBuyAuction->dwRet!=0)
	{
		showTipInfo(GBKToUTF8(pBuyAuction->szDescribeString));
	}
	else
	{

	}
	CCLog("buy:: %d <<%s>>",pBuyAuction->dwRet, __FUNCTION__);
}
