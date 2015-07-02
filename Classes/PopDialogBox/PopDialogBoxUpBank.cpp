/*
 * PopDialogBoxUpBank.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxUpBank.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../Network/SEvent.h"
#include "../Tools/Tools.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxUpBank::PopDialogBoxUpBank()
{
    
	
}
PopDialogBoxUpBank::~PopDialogBoxUpBank() {
	CCLog("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxUpBank::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxUpBank.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	//上庄
	UIButton *pBUpBank=static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonBankAction"));
	pBUpBank->addTouchEventListener(this, toucheventselector(PopDialogBoxUpBank::onMenuApplyBanker));
	/*if (DataModel::sharedDataModel()->userInfo->lScore<1000000
		//||DataModel::sharedDataModel()->getMainSceneOxHundred()->getGameControlOxHundred()->m_bMeApplyBanker
		)
	{
		pBUpBank->setTouchEnabled(false);
		pBUpBank->setColor(ccc3(100,100,100));
	}*/
	updateUpBankState();

	//初始化列表数据
	initListBank();
	//播放显示动画
	playAnimation();

	//添加监听事件
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(PopDialogBoxUpBank::updateBankList),UPDATE_BANK_LIST,NULL);
}
void PopDialogBoxUpBank::onExit(){
	//移除监听事件 
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, UPDATE_BANK_LIST); 

	CCLayer::onExit();
}
void PopDialogBoxUpBank::initListBank(){
	//庄家列表
	pLBank = static_cast<UIListView*>(pUILayer->getWidgetByName("ListViewBank"));
	//设置cell模式
	pLBank->setItemModel(pLBank->getItem(0));
	//庄家列表
	std::list <tagApplyUser>     listApplyUser=DataModel::sharedDataModel()->getMainSceneOxHundred()->getGameControlOxHundred()->listApplyUser;
	if (listApplyUser.size()==0)
	{
		pLBank->removeAllItems();
	}else
	{
		int i=0;
		std::list <tagApplyUser> ::iterator iter;
		for (iter = listApplyUser.begin(); iter != listApplyUser.end(); iter++)
		{
			if (i==0)
			{
				insertBank(false,i,(tagApplyUser)*iter);
			}else
			{
				insertBank(true,i,(tagApplyUser)*iter);
			}
			i++;
		}
	}
}
//插入庄家
void PopDialogBoxUpBank::insertBank(bool isInsert,int index,tagApplyUser applyUser){
	if (isInsert)
	{
		pLBank->insertDefaultItem(index);
	}
	UILabel *pLGold=static_cast<UILabel*>(pLBank->getItem(index)->getChildByName("LabelGold"));
	pLGold->setText(CCString::createWithFormat("%lld",applyUser.lUserScore)->getCString());
	if (Tools::GBKToUTF8(applyUser.strUserName.c_str()))
	{
		UILabel *pLName=static_cast<UILabel*>(pLBank->getItem(index)->getChildByName("LabelName"));
		pLName->setText(Tools::GBKToUTF8(applyUser.strUserName.c_str()));
	}
	//if (index!=0)
	{
		pLBank->getItem(index)->getChildByName("ImageBankIcon")->setVisible(false);
	}


}
//更新上庄状态
void PopDialogBoxUpBank::updateUpBankState(){
	UIButton *pBUpBank = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonBankAction"));

	if (DataModel::sharedDataModel()->getMainSceneOxHundred()->getGameControlOxHundred()->m_bMeApplyBanker)
	{
		pBUpBank->setTitleText("我要下庄");
		if (!DataModel::sharedDataModel()->getMainSceneOxHundred()->getGameControlOxHundred()->isChangeUpBank)
		{
			pBUpBank->setTouchEnabled(false);
			pBUpBank->setColor(ccc3(100, 100, 100));
			return;
		}
		WORD wBankerUser = DataModel::sharedDataModel()->getMainSceneOxHundred()->getGameControlOxHundred()->m_wBankerUser;
		
		if (wBankerUser == DataModel::sharedDataModel()->userInfo->wChairID)
		{
			switch (DataModel::sharedDataModel()->getMainSceneOxHundred()->getGameState())
			{
			case MainSceneBase::STATE_GAME_FREE:
			{
				pBUpBank->setTouchEnabled(true);
				pBUpBank->setColor(ccc3(255, 255, 255));
			}
				break;
			default:
			{
				pBUpBank->setTouchEnabled(false);
				pBUpBank->setColor(ccc3(100, 100, 100));
			}
				break;
			}
		}
		else
		{
			pBUpBank->setTouchEnabled(true);
			pBUpBank->setColor(ccc3(255, 255, 255));
		}
	}
	else
	{
		
		pBUpBank->setTitleText("我要上庄");
		if (DataModel::sharedDataModel()->userInfo->lScore < 1000000){
			pBUpBank->setTouchEnabled(false);
			pBUpBank->setColor(ccc3(100, 100, 100));
		}
		else
		{
			pBUpBank->setTouchEnabled(true);
			pBUpBank->setColor(ccc3(255, 255, 255));
		}
	}
	
}
//申请庄家
void PopDialogBoxUpBank::onMenuApplyBanker(CCObject *object, TouchEventType type){
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			UIButton *pBUpBank=(UIButton *)object;
			pBUpBank->setTouchEnabled(false);
			pBUpBank->setColor(ccc3(100,100,100));

			if (DataModel::sharedDataModel()->getMainSceneOxHundred()->getGameControlOxHundred()->m_bMeApplyBanker)
			{
				DataModel::sharedDataModel()->getMainSceneOxHundred()->getGameControlOxHundred()->onApplyBanker(false);
			}
			else
			{
				DataModel::sharedDataModel()->getMainSceneOxHundred()->getGameControlOxHundred()->onApplyBanker(true);
			}
			
		}
		break;
	default:
		break;
	}
}
//更新列表
void PopDialogBoxUpBank::updateBankList(CCObject *obj){
	//initListBank();
	
	//庄家列表 
	std::list <tagApplyUser>     listApplyUser=DataModel::sharedDataModel()->getMainSceneOxHundred()->getGameControlOxHundred()->listApplyUser;
	//增加庄家
	if (listApplyUser.size()>pLBank->getItems()->count())
	{
		int i=0;
		std::list <tagApplyUser> ::iterator iter;
		for (iter = listApplyUser.begin(); iter != listApplyUser.end(); iter++)
		{
			if (i<pLBank->getItems()->count())
			{
				i++;
				continue;
			}
			insertBank(true,i,(tagApplyUser)*iter);
		}
	}else if(listApplyUser.size()<pLBank->getItems()->count())//取消庄家
	{
		for (int i = 0; i < pLBank->getItems()->count()-listApplyUser.size(); i++)
		{
			pLBank->removeItem(pLBank->getItems()->count()-1);
		}
		updateListContent();
	}else//抢庄更新列表
	{
		updateListContent();
	}

	updateUpBankState();
}
//更新列表内容
void PopDialogBoxUpBank::updateListContent(){
	//庄家列表
	std::list <tagApplyUser>     listApplyUser=DataModel::sharedDataModel()->getMainSceneOxHundred()->getGameControlOxHundred()->listApplyUser;
	if (listApplyUser.size()==0)
	{
		pLBank->removeAllItems();
	}else
	{
		std::list <tagApplyUser> ::iterator iter;
		int i=0;
		for (iter = listApplyUser.begin(); iter != listApplyUser.end(); iter++)
		{
			UILabel *pLGold=static_cast<UILabel*>(pLBank->getItem(i)->getChildByName("LabelGold"));
			pLGold->setText(CCString::createWithFormat("%lld",iter->lUserScore)->getCString());
			if (Tools::GBKToUTF8(iter->strUserName.c_str()))
			{
				UILabel *pLName=static_cast<UILabel*>(pLBank->getItem(i)->getChildByName("LabelName"));
				pLName->setText(Tools::GBKToUTF8(iter->strUserName.c_str()));
			}
			i++;
		}
	}
}