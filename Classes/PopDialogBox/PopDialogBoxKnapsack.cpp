/*
 * PopDialogBoxKnapsack.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxKnapsack.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "PopDialogBoxLoading.h"
#include "PopDialogBoxShop.h"
#include "PopDialogBoxTipInfo.h"
#include "PopDialogBoxInputExchange.h"
//#include "../Network/ListernerThread/LogonGameListerner.h"
#include "../Network/CMD_Server/Packet.h"
#include "../Network/MD5/MD5.h"
#include "../GameLobby/BaseLobbyScene.h"
#define MAX_KNAPSACK_ROW_COUNT			4			//物品横排数

#define NOT_USE							1			//道具不能使用
#define USE_STATE						2			//使用
#define EXCHANGE_STATE					3			//兑换
//////////////////////////////////////////////////////////////////////////
PopDialogBoxKnapsack::PopDialogBoxKnapsack()
	:knapsackItem(KNAPSACK_LIST)
	, iCurSelectIndex(0)
	, eAgainGetData(AGAIN_NOTHING)
{
    
	scheduleUpdate();
}
PopDialogBoxKnapsack::~PopDialogBoxKnapsack() {
	CCLOG("~ <<%s>>",__FUNCTION__);
	unscheduleUpdate();
	//TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_KNAPSACK);
	//gameSocket.Destroy(true);
}
void PopDialogBoxKnapsack::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxKnapsack.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);

	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	//信息背景
	pIInfoBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageInfoBg")); 
	//空包背景
	pINothing = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageNothing"));
	pINothing->setVisible(false);
	//兑换
	UIButton *pBExchange = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonExchange"));
	pBExchange->addTouchEventListener(this, toucheventselector(PopDialogBoxKnapsack::onMenuExchange));
	//物品图片
	pIVGoods = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageGoodsInfoBg"));
	initListGoods();
	initGoodsInfo();

	pIInfoBg->setEnabled(false);
	//updateListGoods();

	playAnimation();
	
	connectServer();
	connectSuccess();
	/*TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_SHOP);
	PopDialogBox *box = PopDialogBoxLoading::create();
	this->addChild(box, 10, TAG_LOADING);
	box->setSocketName(SOCKET_KNAPSACK);
	TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_KNAPSACK);
	TCPSocket *tcp = getSocket();
	if (tcp)
	{
		tcp->createSocket(GAME_IP, PORT_LOGON, new LogonGameListerner());
	}*/
}
void PopDialogBoxKnapsack::onExit(){
	if (getIPopAssistKnapsack())
	{
		getIPopAssistKnapsack()->onCloseKnapsack();
	}
	CCLayer::onExit();
}
void PopDialogBoxKnapsack::update(float delta){
	//MessageQueue::update(delta);
	gameSocket.updateSocketData(delta);
}
//连接成功
void PopDialogBoxKnapsack::connectSuccess(){
	switch (knapsackItem)
	{
	case PopDialogBoxKnapsack::KNAPSACK_LIST:
	{
		CMD_GP_UserID userInfo;
		userInfo.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;

		MD5 m;
		m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(), DataModel::sharedDataModel()->sLogonPassword.length());
		std::string md5PassWord = m.GetMd5();
		strcpy(userInfo.szPassword, md5PassWord.c_str());

		gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_KNAPSACK, &userInfo, sizeof(CMD_GP_UserID));
	}
	break;
	case PopDialogBoxKnapsack::KNAPSACK_EXCHANGE:
	{
		CMD_GP_UseKnapsack useKnapsack;
		useKnapsack.dwUserID = DataModel::sharedDataModel()->userInfo->dwUserID;
		useKnapsack.dwOpTerminal = 2;

		MD5 m;
		m.ComputMd5(DataModel::sharedDataModel()->sLogonPassword.c_str(), DataModel::sharedDataModel()->sLogonPassword.length());
		std::string md5PassWord = m.GetMd5();
		strcpy(useKnapsack.szPassword, md5PassWord.c_str());

		useKnapsack.dwID = vecGoods[iCurSelectIndex].dwID;
		useKnapsack.dwNum = lExchangeNum;

		strcpy(useKnapsack.szNote, sExchangeContent.c_str());//qq号或手机号
		strcpy(useKnapsack.szMachineID, "12");

		gameSocket.SendData(MDM_GP_USER_SERVICE, SUB_GP_USE_KNAPSACKLOG, &useKnapsack, sizeof(CMD_GP_UseKnapsack));

		/*DWORD							dwUserID;							//用户 I D
		DWORD							dwOpTerminal;						//操作终端（1：pc, 2：手机）
		TCHAR							szPassword[LEN_PASSWORD];			//用户密码
		DWORD							dwID;								//背包id
		DWORD							dwNum;								//数量
		TCHAR							szNote[NOTE_LEN];					//描述消息
		TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列*/
	}
	break;
	default:
		break;
	}
}
//读取网络消息回调
void PopDialogBoxKnapsack::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_MB_SOCKET://socket连接成功
	{
		connectSuccess();
	}
	break;
	case MDM_GP_USER_SERVICE://用户服务
	{
		onEventUserService(wSubCmdID,pDataBuffer,wDataSize);

		//移除loading
		this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
		//关闭网络
		//TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_KNAPSACK);
		gameSocket.Destroy(true);
		switch (eAgainGetData)
		{
		case PopDialogBoxKnapsack::AGAIN_NOTHING:
			break;
		case PopDialogBoxKnapsack::AGAIN_UPDATE_LIST:
		{
			setKnapsackItem(KNAPSACK_LIST);
			connectServer();
			connectSuccess();
			setAgainGetData(AGAIN_DELAY_DISPLAY);
		}
			break;
		case PopDialogBoxKnapsack::AGAIN_DELAY_DISPLAY:
		{
			PopDialogBoxTipInfo *pTipInfo = PopDialogBoxTipInfo::create();
			this->addChild(pTipInfo, 100);
			pTipInfo->setTipInfoContent(sDisplayTipInfo.c_str());

			setAgainGetData(AGAIN_NOTHING);
		}
			break;
		default:
			break;
		}
	}
	break;
	default:
		CCLOG("other:%d   %d<<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
		break;
	}
}
//用户服务
void PopDialogBoxKnapsack::onEventUserService(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GP_KNAPSACK:
	{
		onSubGoodsList(pDataBuffer, wDataSize);
	}
		break;
	case SUB_GP_KNAPSACKLOG:
	{
		updateListGoods();
		//CMD_GP_KnapsackLog *pGoods = (CMD_GP_KnapsackLog*)pDataBuffer;
		//showTipInfo(GBKToUTF8(pGoods->szDescribeString));
	}
		break;
	case SUB_GP_USE_KNAPSACKLOG:
	{
		onSubUseGoods(pDataBuffer, wDataSize);
	}
		break;
	default:
		break;
	}
}
//背包列表
void PopDialogBoxKnapsack::onSubGoodsList(void * pDataBuffer, unsigned short wDataSize){
	assert(wDataSize >= sizeof(CMD_GP_Knapsack));

	vecGoods.clear();

	int gameServerSize = sizeof(CMD_GP_Knapsack);
	int serverCount = wDataSize / gameServerSize;

	BYTE cbDataBuffer[SOCKET_TCP_PACKET + sizeof(TCP_Head)];
	CopyMemory(cbDataBuffer, pDataBuffer, wDataSize);

	for (int i = 0; i < serverCount; i++)
	{
		void * pDataBuffer = cbDataBuffer + i*sizeof(CMD_GP_Knapsack);
		CMD_GP_Knapsack *pGoods = (CMD_GP_Knapsack*)pDataBuffer;
		CMD_GP_Knapsack gpGoods;
		memcpy(&gpGoods, pGoods, sizeof(CMD_GP_Knapsack));
		vecGoods.push_back(gpGoods);

		//CCLOG("%ld  %s<<%s>>", gpGoods.dwSortID,GBKToUTF8(gpGoods.szName), __FUNCTION__);
		/*DWORD				dwID;
		DWORD				dwPackType;								//类别1:礼包，2:道具
		DWORD				dwPropID;								//礼包或道具id
		DWORD				dwNum;									//数量
		DWORD				dwSortID;								//顺序
		TCHAR				szName[GIFT_NAME_LEN];					//名称
		TCHAR				szImgName[GIFT_IMGNAME];				//图片*/
	}
	updateListGoods();
}
//使用物品
void PopDialogBoxKnapsack::onSubUseGoods(void * pDataBuffer, unsigned short wDataSize){
	//效验数据
	assert (wDataSize != sizeof(CMD_GP_UseKnapsackLog));
	CMD_GP_UseKnapsackLog * pOpenCard = (CMD_GP_UseKnapsackLog *)pDataBuffer;
	if (pOpenCard->dwRet==0)
	{
		

		vecGoods[iCurSelectIndex].dwNum--;
		if (vecGoods[iCurSelectIndex].dwNum<=0)
		{
			vector< CMD_GP_Knapsack >::iterator k = vecGoods.begin() + iCurSelectIndex;
			vecGoods.erase(k);
		}
		if (iCurSelectIndex>=vecGoods.size()-1)
		{
			iCurSelectIndex = vecGoods.size()-1;
		}
		updateListGoods();
		/*if (vecGoods.size()>0)
		{
			if (vecGoods[iCurSelectIndex].dwExchangeType == EXCHANGE_STATE)
			{
				setAgainGetData(AGAIN_UPDATE_LIST);
				sDisplayTipInfo = GBKToUTF8(pOpenCard->szDescribeString);
				return;
			}
		}
		else*/
		{
			setAgainGetData(AGAIN_UPDATE_LIST);
			sDisplayTipInfo = GBKToUTF8(pOpenCard->szDescribeString);
			return;
		}
	}
	else
	{
		
	}
	PopDialogBoxTipInfo *pTipInfo = PopDialogBoxTipInfo::create();
	this->addChild(pTipInfo, 100);
	pTipInfo->setTipInfoContent(GBKToUTF8(pOpenCard->szDescribeString).c_str());

}
//初始化商品列表
void PopDialogBoxKnapsack::initListGoods(){
	//物品列表
	pListViewGoods = static_cast<UIListView*>(pUILayer->getWidgetByName("ListViewKnapsack"));
	//设置cell模式
	pListViewGoods->setItemModel(pListViewGoods->getItem(0));
	pListViewGoods->removeAllItems();
}
//更新物品列表
void PopDialogBoxKnapsack::updateListGoods(){
	pListViewGoods->removeAllItems();

	int tempSize = vecGoods.size();
	if (tempSize == 0)
	{
		pIInfoBg->setEnabled(false);
		pINothing->setVisible(true);
		return;
	}
	pIInfoBg->setEnabled(true);
	pINothing->setVisible(false);
	for (int i = 0; i < (tempSize - 1) / MAX_KNAPSACK_ROW_COUNT + 1; i++)
	{
		pListViewGoods->insertDefaultItem(pListViewGoods->getItems()->count());
		for (int j = 0; j < MAX_KNAPSACK_ROW_COUNT; j++)
		{
			int tempIndex = i*MAX_KNAPSACK_ROW_COUNT + j;
			if (tempIndex < vecGoods.size())
			{
				UIImageView *pIVItem = static_cast<UIImageView*>(pListViewGoods->getItem(pListViewGoods->getItems()->count() - 1)->getChildByName(CCString::createWithFormat("ImageItem%d", j)->getCString()));


				UICheckBox *pCheckBox = static_cast<UICheckBox*>(pListViewGoods->getItem(pListViewGoods->getItems()->count() - 1)->getChildByName(CCString::createWithFormat("ImageItem%d", j)->getCString())->getChildByName("CheckBox"));
				pCheckBox->setTag(tempIndex);
				pCheckBox->addEventListenerCheckBox(this, SEL_SelectedStateEvent(&PopDialogBoxKnapsack::onCheckBoxSelectedStateEvent));
				
				//UIImageView *pIDetermine = static_cast<UIImageView*>(pCheckBox->getChildByName("ImageDetermine"));
				//pIDetermine->setVisible(false);

				addDownloadImage(pIVItem, vecGoods[tempIndex].szImgName,CCPointZero,1,0,false);

				//设置数量
				UIImageView *pINumBg = static_cast<UIImageView*>(pListViewGoods->getItem(pListViewGoods->getItems()->count() - 1)->getChildByName(CCString::createWithFormat("ImageItem%d", j)->getCString()));
				UILabelAtlas *pPropDescription = static_cast<UILabelAtlas*>(pINumBg->getChildByName("AtlasLabelGoldNum"));
				if (vecGoods[tempIndex].dwNum>99)
				{
					pPropDescription->setStringValue("99:");
				}
				else
				{
					pPropDescription->setStringValue(CCString::createWithFormat("%ld", vecGoods[tempIndex].dwNum)->getCString());
				}
				
			}
		}
	}

	for (int j = 0; j < MAX_KNAPSACK_ROW_COUNT; j++)
	{
		if (j >(tempSize - 1) % (MAX_KNAPSACK_ROW_COUNT)){
			UIImageView *pItem = static_cast<UIImageView*>(pListViewGoods->getItem(pListViewGoods->getItems()->count() - 1)->getChildByName(CCString::createWithFormat("ImageItem%d", j)->getCString()));
			pItem->setEnabled(false);
		}

	}

	//////////////////////////////////////////////////////////////////////////
	UICheckBox *pCheckBox = static_cast<UICheckBox*>(pListViewGoods->getItem(iCurSelectIndex / MAX_KNAPSACK_ROW_COUNT)->getChildByName(CCString::createWithFormat("ImageItem%d", iCurSelectIndex%MAX_KNAPSACK_ROW_COUNT)->getCString())->getChildByName("CheckBox"));
	pCheckBox->setSelectedState(true);
	onCheckBoxSelectedStateEvent(pCheckBox, CHECKBOX_STATE_EVENT_SELECTED);
}
//初始化物品信息
void PopDialogBoxKnapsack::initGoodsInfo(){
	//物品列表
	pLGoodsName = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelGoodsName"));
	pLGoodsNum = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelPropCount"));
	pLInfoContent = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelInfoContent"));
}
//更新物品信息
void PopDialogBoxKnapsack::updateGoodInfo(int index){
	pLGoodsName->setText(GBKToUTF8(vecGoods[index].szName));
	pLInfoContent->setText(GBKToUTF8(vecGoods[index].szOpenDespict));
	pLGoodsNum->setText(CCString::createWithFormat("数量:%ld",vecGoods[index].dwNum)->getCString());

	//兑换
	UIButton *pBExchange = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonExchange"));
	switch (vecGoods[index].dwExchangeType)
	{
	case NOT_USE:
	{
		pBExchange->setEnabled(false);
	}
		break;
	case USE_STATE:
	{
		//pBExchange->setTitleText("使用");
		pBExchange->loadTextureNormal("u_k_ShiYong_But_Normal.png", UI_TEX_TYPE_PLIST);
		pBExchange->loadTexturePressed("u_k_ShiYong_But_Down.png", UI_TEX_TYPE_PLIST);
		
		pBExchange->setEnabled(true);
	}
		break;
	case EXCHANGE_STATE:
	{
		//pBExchange->setTitleText("兑换");
		pBExchange->loadTextureNormal("DuiHuan_Btn_Normal.png", UI_TEX_TYPE_PLIST);
		pBExchange->loadTexturePressed("DuiHuan_Btn_Down.png", UI_TEX_TYPE_PLIST);

		pBExchange->setEnabled(true);
	}
		break;
	default:
		break;
	}

	addDownloadImage(pIVGoods, vecGoods[index].szImgName, CCPointZero, 1,0, false);
}
//复选框回调（选择性别）
void PopDialogBoxKnapsack::onCheckBoxSelectedStateEvent(CCObject *pSender, CheckBoxEventType type){
	switch (type)
	{
	case CHECKBOX_STATE_EVENT_SELECTED:
	{
		UICheckBox *box = (UICheckBox*)pSender;
		iCurSelectIndex = box->getTag();
		
		for (int i= 0; i < vecGoods.size(); i++)
		{
			for (int j = 0; j < MAX_KNAPSACK_ROW_COUNT; j++)
			{
				int tempIndex = i*MAX_KNAPSACK_ROW_COUNT + j;
				if (tempIndex < vecGoods.size())
				{
					UICheckBox *pCheckBox = static_cast<UICheckBox*>(pListViewGoods->getItem(i)->getChildByName(CCString::createWithFormat("ImageItem%d", j)->getCString())->getChildByName("CheckBox"));
					//UIImageView *pIDetermine = static_cast<UIImageView*>(pCheckBox->getChildByName("ImageDetermine"));
				
					if (iCurSelectIndex == tempIndex)
					{
						//pIDetermine->setVisible(true);
						updateGoodInfo(iCurSelectIndex);
						pCheckBox->setTouchEnabled(false);
					}
					else
					{
						//pIDetermine->setVisible(false);
						pCheckBox->setSelectedState(false);
						pCheckBox->setTouchEnabled(true);
					}
				}
				
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
//兑换按键
void PopDialogBoxKnapsack::onMenuExchange(CCObject *object, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		if (vecGoods[iCurSelectIndex].dwExchangeType == USE_STATE
			&&vecGoods[iCurSelectIndex].dwUseType != USE_PHONE_COST
			&&vecGoods[iCurSelectIndex].dwUseType != USE_QQ_COST)
		{
			lExchangeNum = 1;
			sExchangeContent = "";
			setKnapsackItem(KNAPSACK_EXCHANGE);
			connectServer();
			connectSuccess();
		}
		else
		{
			PopDialogBoxInputExchange *box = PopDialogBoxInputExchange::create();
			this->addChild(box, 10, TAG_INPUT_BOX);
			int iUseType = 0;
			if (vecGoods[iCurSelectIndex].dwExchangeType==EXCHANGE_STATE)
			{ 
				iUseType = USE_EXCHANGE_OTHER;
			}
			else
			{
				iUseType=vecGoods[iCurSelectIndex].dwUseType;
			}
			box->setInputExchangeData((UseType)(iUseType), GBKToUTF8(vecGoods[iCurSelectIndex].szName).c_str(), vecGoods[iCurSelectIndex].szImgName, vecGoods[iCurSelectIndex].dwNum, GBKToUTF8(vecGoods[iCurSelectIndex].szRemark).c_str());
			box->setIPopDialogBoxExchange(this);
		}
		
	}
		break;
	default:
		break;
	}
}
//数量输入回调
void PopDialogBoxKnapsack::onExchangeNumWithContent(int type, std::string sContent){
	//CCLOG("%ld  %s <<%s>>",num,sContent.c_str(), __FUNCTION__);
	switch (type)
	{
	case USE_QQ_COST:
	case USE_PHONE_COST:
	{
		lExchangeNum = 1;
		sExchangeContent = sContent;
	}
		break;
	default:
	{
		lExchangeNum = strtol(sContent.c_str(),NULL,10);
		sExchangeContent = "";
	}
		break;
	}
	setKnapsackItem(KNAPSACK_EXCHANGE);
	connectServer();
	connectSuccess();
}