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
#include "../Network/ListernerThread/LogonGameListerner.h"
#include "../Network/MD5/MD5.h"
#include "../GameLobby/BaseLobbyScene.h"

#define MAX_KNAPSACK_ROW_COUNT			4			//物品横排数
//////////////////////////////////////////////////////////////////////////
PopDialogBoxKnapsack::PopDialogBoxKnapsack()
	:knapsackItem(KNAPSACK_LIST)
	, iCurSelectIndex(0)
{
    
	scheduleUpdate();
}
PopDialogBoxKnapsack::~PopDialogBoxKnapsack() {
	CCLog("~ <<%s>>",__FUNCTION__);
	unscheduleUpdate();
	TCPSocketControl::sharedTCPSocketControl()->removeTCPSocket(SOCKET_KNAPSACK);
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
	//兑换
	UIButton *pBExchange = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonExchange"));
	pBExchange->addTouchEventListener(this, toucheventselector(PopDialogBoxKnapsack::onMenuExchange));

	initListGoods();
	initGoodsInfo();

	updateListGoods();

	playAnimation();
	
	connectServer(SOCKET_KNAPSACK);
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
	((PopDialogBoxShop*)this->getParent())->isReadMessage = true;
	CCLayer::onExit();
}
void PopDialogBoxKnapsack::update(float delta){
	MessageQueue::update(delta);
}
//读取网络消息回调
void PopDialogBoxKnapsack::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_MB_SOCKET://socket连接成功
	{
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
			
			getSocket()->SendData(MDM_GP_USER_SERVICE, SUB_GP_KNAPSACK, &userInfo, sizeof(CMD_GP_UserID));
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
			useKnapsack.dwNum = 1;

			strcpy(useKnapsack.szNote, "");
			strcpy(useKnapsack.szMachineID, "12");

			getSocket()->SendData(MDM_GP_USER_SERVICE, SUB_GP_USE_KNAPSACKLOG, &useKnapsack, sizeof(CMD_GP_UseKnapsack));
			
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
	break;
	case MDM_GP_USER_SERVICE://用户服务
	{
		onEventUserService(wSubCmdID,pDataBuffer,wDataSize);

		//移除loading
		this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
		//关闭网络
		TCPSocketControl::sharedTCPSocketControl()->stopSocket(SOCKET_KNAPSACK);
	}
	break;
	default:
		CCLog("other:%d   %d<<%s>>", wMainCmdID, wSubCmdID, __FUNCTION__);
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

		//CCLog("%ld  %s<<%s>>", gpGoods.dwSortID,GBKToUTF8(gpGoods.szName), __FUNCTION__);
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
	if (pOpenCard->dwRet==1)
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

	}
	else
	{
		
	}
	PopDialogBoxTipInfo *pTipInfo = PopDialogBoxTipInfo::create();
	this->addChild(pTipInfo, 100);
	pTipInfo->setTipInfoContent(GBKToUTF8(pOpenCard->szDescribeString));

}
//初始化商品列表
void PopDialogBoxKnapsack::initListGoods(){
	//物品列表
	pListViewGoods = static_cast<UIListView*>(pUILayer->getWidgetByName("ListViewKnapsack"));
	//设置cell模式
	pListViewGoods->setItemModel(pListViewGoods->getItem(0));
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
				UICheckBox *pCheckBox = static_cast<UICheckBox*>(pListViewGoods->getItem(pListViewGoods->getItems()->count() - 1)->getChildByName(CCString::createWithFormat("ImageItem%d", j)->getCString())->getChildByName("CheckBox"));
				pCheckBox->setTag(tempIndex);
				pCheckBox->addEventListenerCheckBox(this, SEL_SelectedStateEvent(&PopDialogBoxKnapsack::onCheckBoxSelectedStateEvent));
				
				UIImageView *pIDetermine = static_cast<UIImageView*>(pCheckBox->getChildByName("ImageDetermine"));
				pIDetermine->setVisible(false);

				
				
				//设置数量
				UIImageView *pINumBg = static_cast<UIImageView*>(pListViewGoods->getItem(pListViewGoods->getItems()->count() - 1)->getChildByName(CCString::createWithFormat("ImageItem%d", j)->getCString())->getChildByName("ImageGoodsNumBg"));
				UILabel *pPropDescription = static_cast<UILabel*>(pINumBg->getChildByName("LabelGoldNum"));
				if (vecGoods[tempIndex].dwNum>99)
				{
					pPropDescription->setText("99+");
				}
				else
				{
					pPropDescription->setText(CCString::createWithFormat("%ld", vecGoods[tempIndex].dwNum)->getCString());
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
}
//更新物品信息
void PopDialogBoxKnapsack::updateGoodInfo(int index){
	pLGoodsName->setText(GBKToUTF8(vecGoods[index].szName));

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
					UIImageView *pIDetermine = static_cast<UIImageView*>(pCheckBox->getChildByName("ImageDetermine"));
				
					if (iCurSelectIndex == tempIndex)
					{
						pIDetermine->setVisible(true);
						updateGoodInfo(iCurSelectIndex);
						pCheckBox->setTouchEnabled(false);
					}
					else
					{
						pIDetermine->setVisible(false);
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
		setKnapsackItem(KNAPSACK_EXCHANGE);
		connectServer(SOCKET_KNAPSACK);
	}
		break;
	default:
		break;
	}
}