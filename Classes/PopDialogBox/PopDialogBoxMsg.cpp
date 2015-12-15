/*
 * PopDialogBoxMsg.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxMsg.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "../Tools/Tools.h"
#include "../MTNotificationQueue/MTNotificationQueue.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxMsg::PopDialogBoxMsg()
{
	
    
}
PopDialogBoxMsg::~PopDialogBoxMsg() {
	CCLOG("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxMsg::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxMsg.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBoxMsg::onMenuClose));
	//系统消息按键
	pBSystemMsg = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonSystem"));
	pBSystemMsg->addTouchEventListener(this, toucheventselector(PopDialogBoxMsg::onMenuMsg));
	//pBSystemMsg->setVisible(false);
	pBSystemMsg->setBright(false);
	pBSystemMsg->setTouchEnabled(false);
	//我的消息
	pBSystemMy = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonMy"));
	pBSystemMy->addTouchEventListener(this, toucheventselector(PopDialogBoxMsg::onMenuMsg));
	//pBSystemMy->setVisible(false);
	//标题
	//pIVTitle = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageTitle"));
	//内容标题
	pIVTitleContent = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageTitleContent"));
	pIVTitleContent->setEnabled(false);
	//详细消息返回键
	pBBackMsg = static_cast<UIButton*>(pUILayer->getWidgetByName("ButtonBack"));
	pBBackMsg->addTouchEventListener(this, toucheventselector(PopDialogBoxMsg::onMenuBackMsg));
	pBBackMsg->setEnabled(false);
	//详细内容
	pSVMsgContent=static_cast<UIScrollView*>(pUILayer->getWidgetByName("ScrollViewContent"));
	pSVMsgContent->setEnabled(false);
	//游标
	//pIVCursor = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageCursor"));
	//消息名称
	//pLMsgName = static_cast<UILabel*>(pUILayer->getWidgetByName("LabelMsgName"));
	
	//消息列表
	pLVMsgList = static_cast<UIListView*>(pUILayer->getWidgetByName("ListViewMsg"));
	//设置cell模式
	pLVMsgList->setItemModel(pLVMsgList->getItem(0));
	pLVMsgList->removeAllItems();

	//没有消息
	pIVNothing = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageNothing"));
	pIVNothing->setVisible(false);

	//DataModel::sharedDataModel()->vecSystemMsg.push_back("1233");
	//更新列表
	updateListMsg(DataModel::sharedDataModel()->vecSystemMsg);

	playAnimation();
}
void PopDialogBoxMsg::onExit(){
	CCLayer::onExit();
}
//关闭
void PopDialogBoxMsg::onMenuClose(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		saveMsgID();
		PopDialogBox::menuBack(object, type);
	}
}
//消息按键回调
void PopDialogBoxMsg::onMenuMsg(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		UIButton *pBTemp = (UIButton *)object;
		switch (pBTemp->getTag())
		{
		case PopDialogBoxMsg::MSG_SYSTEM:
		{
			//pIVCursor->stopAllActions();
			//pIVCursor->runAction(CCMoveTo::create(0.1,ccp(-88,0)));
			//pLMsgName->setText("系统消息");
			pBSystemMsg->setBright(false);
			pBSystemMsg->setTouchEnabled(false);

			pBSystemMy->setBright(true);
			pBSystemMy->setTouchEnabled(true);
			//更新列表
			updateListMsg(DataModel::sharedDataModel()->vecSystemMsg);
		}
			break;
		case PopDialogBoxMsg::MSG_MY:
		{
			pBSystemMsg->setBright(true);
			pBSystemMsg->setTouchEnabled(true);

			pBSystemMy->setBright(false);
			pBSystemMy->setTouchEnabled(false);
			//pIVCursor->stopAllActions();
			//pIVCursor->runAction(CCMoveTo::create(0.1, ccp(88, 0)));
			//pLMsgName->setText("我的消息");
			//更新列表
			updateListMsg(DataModel::sharedDataModel()->vecMyMsg);
		}
			break;
		default:
			break;
		}
	}
}
//消息返回按键回调
void PopDialogBoxMsg::onMenuBackMsg(CCObject *object, TouchEventType type){
	if (type==TOUCH_EVENT_ENDED)
	{
		pLVMsgList->setEnabled(true);
		pSVMsgContent->setEnabled(false);
		pBBackMsg->setEnabled(false);

		pBSystemMsg->setEnabled(true);
		pBSystemMy->setEnabled(true);
		//pIVTitle->setEnabled(true);
		pIVTitleContent->setEnabled(false);
	}
}
//选择消息项
void PopDialogBoxMsg::onMenuSelectMsgItem(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		UIImageView *pIVTemp = (UIImageView*)object;
		UILabel *pLContent = static_cast<UILabel*>(pIVTemp->getChildByName("LabelContent"));
		//CCLOG("-:%s <<%s>>", pLContent->getStringValue(), __FUNCTION__);
		pLVMsgList->setEnabled(false);
		pSVMsgContent->setEnabled(true);
		pBBackMsg->setEnabled(true);

		//pIVTitle->setEnabled(false);
		pBSystemMsg->setEnabled(false);
		pBSystemMy->setEnabled(false);

		pIVTitleContent->setEnabled(true);
		//设置内容
		UILabel *pLMsgContent = static_cast<UILabel*>(pSVMsgContent->getChildByName("LabelMsgContent"));
		pLMsgContent->ignoreContentAdaptWithSize(true);
		pLMsgContent->setTextAreaSize(CCSize(608, 0));
		pLMsgContent->setColor(ccc3(89, 40, 0));
		pLMsgContent->setText(pLContent->getStringValue());
		
		pSVMsgContent->setInnerContainerSize(pLMsgContent->getContentSize());
	}
}
//更新消息列表
void PopDialogBoxMsg::updateListMsg(std::vector<std::string> qMsg){
	pLVMsgList->removeAllItems();
	
	int tempSize = qMsg.size();
	if (tempSize == 0)
	{
		pIVNothing->setVisible(true);
		return;
	}
	pIVNothing->setVisible(false);

	for (int i = 0; i < tempSize; i++)
	{
		pLVMsgList->insertDefaultItem(pLVMsgList->getItems()->count());
		
		//
		UIImageView *pIVItem = static_cast<UIImageView*>(pLVMsgList->getItem(pLVMsgList->getItems()->count() - 1)->getChildByName("ImageBg"));
		pIVItem->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxMsg::onMenuSelectMsgItem));

		//[0] = "喜讯来啦：达人捕鱼话费天天送，每天乐开花了吧！更大重磅的消息来了，我们推出了电脑版【七彩牛游戏中心】有多种不同游戏，有比赛：二人斗地主比赛，千变双扣比赛，捕鱼比赛；有牛牛：百人牛牛，二人牛牛，通比牛牛
				
		UILabel *pLContent = static_cast<UILabel*>(pIVItem->getChildByName("LabelContent"));
		CCLOG("-:%s <<%s>>", GBKToUTF8(qMsg[i].c_str()).c_str(), __FUNCTION__);
		pLContent->setText(GBKToUTF8(qMsg[i].c_str()));
		pLContent->setColor(ccc3(89, 40, 0));
		
	}
	/*
	for (int j = 0; j < ROW_COUNT; j++)
	{
		if (j >(tempSize - 1) % (ROW_COUNT)){
			UIImageView *pItem = static_cast<UIImageView*>(pListViewCommodity->getItem(pListViewCommodity->getItems()->count() - 1)->getChildByName(CCString::createWithFormat("ImageCommodity%d", j)->getCString()));
			pItem->setEnabled(false);
		}

	}*/
}
//保存消息ID
void PopDialogBoxMsg::saveMsgID(){
	//保存
	std::string saveStr = "{";

	std::map<long, NewMsg>::iterator iter;
	for (iter = DataModel::sharedDataModel()->mNewMsg.begin(); iter != DataModel::sharedDataModel()->mNewMsg.end(); iter++)
	{
		saveStr += CCString::createWithFormat("\"newMsgData\":{\"userId\":\"%s\",\"msgId\":\"%s\"},", CCString::createWithFormat("%ld", iter->first)->getCString(), iter->second.sMsgId.c_str())->getCString();
	}
	saveStr.erase(saveStr.end() - 1);

	saveStr += "}";
	Tools::saveStringByRMS(RMS_NEW_MSG_TIP, saveStr.c_str());

	DataModel::sharedDataModel()->isShowNewMsg = false;
	MTNotificationQueue::sharedNotificationQueue()->postNotification(UPDATE_NEW_MSG_STATE, NULL);
}