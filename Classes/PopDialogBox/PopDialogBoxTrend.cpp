/*
 * PopDialogBoxTrend.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxTrend.h"

#include "../Tools/GameConfig.h"
#include "../Tools/Tools.h"
#include "../Play/GameControl/GameControlOxHundred.h"
#include "../Network/SEvent.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxTrend::PopDialogBoxTrend()
{
	
}
PopDialogBoxTrend::~PopDialogBoxTrend() {
	CCLog("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxTrend::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxTrend.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pPTrend=static_cast<UIPanel*>(pUILayer->getWidgetByName("Panel"));
	pLGameRecord=static_cast<UIListView*>(pPTrend->getChildByName("ListViewTrend"));
	updateTrendList(NULL);
	
	/*fPosX=pLOnLine->getPositionX();
	pLOnLine->setItemModel(pLOnLine->getItem(0));
	if (DataModel::sharedDataModel()->mTagUserInfo.size()==0)
	{
		pLOnLine->removeAllItems();
	}
	int i=0;
	std::map<long,tagUserInfo>::iterator iter;
	for (iter = DataModel::sharedDataModel()->mTagUserInfo.begin(); iter != DataModel::sharedDataModel()->mTagUserInfo.end(); iter++)
	{
		if (i==0)
		{
			setOnLineUserInfo(false,iter->second);
		}else
		{
			setOnLineUserInfo(true,iter->second);
		}
		i++;
	}*/
	//动画
	CCEaseExponentialOut  *out=CCEaseExponentialOut ::create(CCMoveTo::create(0.5,ccp(DataModel::sharedDataModel()->deviceSize.width-pPTrend->getContentSize().width,pPTrend->getPositionY())));
	pPTrend->runAction(out);
	
	//重设置下，否则会被覆盖事件
	this->setTouchEnabled(true);
	this->setTouchPriority(0);
	this->setTouchMode(kCCTouchesOneByOne);

	//添加监听事件
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(PopDialogBoxTrend::updateTrendList),UPDATE_LIST,NULL);
}
void PopDialogBoxTrend::onExit(){
	//移除监听事件 
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, UPDATE_LIST); 
	CCLayer::onExit();
}
bool PopDialogBoxTrend::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	CCPoint touchPoint = this->convertTouchToNodeSpace(pTouch);
	CCRect rect= CCRect(pPTrend->getPositionX(),pPTrend->getPositionY(),pPTrend->getContentSize().width,pPTrend->getContentSize().height);
	if (!rect.containsPoint(touchPoint))
	{
		CCEaseExponentialOut  *out=CCEaseExponentialOut ::create(CCMoveTo::create(0.5,ccp(DataModel::sharedDataModel()->deviceSize.width,pPTrend->getPositionY())));
		CCCallFunc *call=CCCallFunc::create(this,SEL_CallFunc(&PopDialogBoxTrend::removSelf));
		CCSequence *seq=CCSequence::create(out,call,NULL);
		pPTrend->runAction(seq);
	}else
	{
		return false;
	}
	return true;
}
//移除自己
void PopDialogBoxTrend::removSelf(){
	this->removeFromParentAndCleanup(true);
}
//刷新列表
void PopDialogBoxTrend::updateTrendList(CCObject *obj){
	std::list<tagGameRecord> listRecord=DataModel::sharedDataModel()->getMainSceneOxHundred()->getGameControlOxHundred()->listGameRecord;
	int i=0;
	for (std::list < tagGameRecord > ::iterator it = listRecord.begin(); it != listRecord.end(); ++ it)
	{
		UIImageView *pIResult0=static_cast<UIImageView*>(pLGameRecord->getItem(i)->getChildByName("ImageResult0"));
		if (it->bWinShunMen)
		{
			pIResult0->loadTexture("u_gih_win.png",UI_TEX_TYPE_PLIST);
		}else
		{
			pIResult0->loadTexture("u_gih_lost.png",UI_TEX_TYPE_PLIST);
		}
		//////////////////////////////////////////////////////////////////////////
		UIImageView *pIResult1=static_cast<UIImageView*>(pLGameRecord->getItem(i)->getChildByName("ImageResult1"));
		if (it->bWinDuiMen)
		{
			pIResult1->loadTexture("u_gih_win.png",UI_TEX_TYPE_PLIST);
		}else
		{
			pIResult1->loadTexture("u_gih_lost.png",UI_TEX_TYPE_PLIST);
		}
		//////////////////////////////////////////////////////////////////////////
		UIImageView *pIResult2=static_cast<UIImageView*>(pLGameRecord->getItem(i)->getChildByName("ImageResult2"));
		if (it->bWinDaoMen)
		{
			pIResult2->loadTexture("u_gih_win.png",UI_TEX_TYPE_PLIST);
		}else
		{
			pIResult2->loadTexture("u_gih_lost.png",UI_TEX_TYPE_PLIST);
		}
		//////////////////////////////////////////////////////////////////////////
		UIImageView *pIResult3=static_cast<UIImageView*>(pLGameRecord->getItem(i)->getChildByName("ImageResult3"));
		if (it->bWinHuang)
		{
			pIResult3->loadTexture("u_gih_win.png",UI_TEX_TYPE_PLIST);
		}else
		{
			pIResult3->loadTexture("u_gih_lost.png",UI_TEX_TYPE_PLIST);
		}
		i++;
	}
}