/*
 * PopDialogBoxAuction.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxAuction.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "PopDialogBoxLoading.h"
#include "../Network/ListernerThread/LogonGameListerner.h"
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
	switch (auctionItem)
	{
	case PopDialogBoxAuction::AUCTION_INFO:
	{
		if (vecAuctionInfo.size()<=0)
		{
			connectServer(SOCKET_AUCTION_INFO);
		}
	}
		break;
	case PopDialogBoxAuction::AUCTION_MY:
		break;
	case PopDialogBoxAuction::AUCTION_RECORD:
		break;
	default:
		break;
	}

	for (int i = 0; i < MAX_AUCTION_ITEM_COUNT; i++)
	{
		if (i==auctionItem)
		{
			pLVAuction[i]->setEnabled(true);
			pLVAuction[i]->removeAllItems();
			pLVAuction[i]->insertDefaultItem(0);

			pICellItemContent[i]->setVisible(true);
			pPLine[i]->setEnabled(true);
			pPLine[i]->setVisible(true);
		}
		else
		{
			pLVAuction[i]->setEnabled(false);
			pICellItemContent[i]->setVisible(false);
			pPLine[i]->setEnabled(false);
			pPLine[i]->setVisible(false);
		}
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
		break;
	case PopDialogBoxAuction::AUCTION_RECORD:
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
		onSubAuctionRecord(pDataBuffer, wDataSize);
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
void PopDialogBoxAuction::onSubAuctionRecord(void * pDataBuffer, unsigned short wDataSize){
	CMD_GP_AuctionRecord <CMD_GP_AuctionRecordItem> *gpAuctionRecord;
	assert(wDataSize>=sizeof(gpAuctionRecord));
	gpAuctionRecord = (CMD_GP_AuctionRecord<CMD_GP_AuctionRecordItem> *)pDataBuffer;
	for (int i = 0; i < gpAuctionRecord->dwIndex; i++)
	{
		vecAuctionInfo.push_back(gpAuctionRecord->RecordItem[i]);
	}

}
