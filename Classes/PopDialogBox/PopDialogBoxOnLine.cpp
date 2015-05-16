/*
 * PopDialogBoxOnLine.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxOnLine.h"

#include "Tools/GameConfig.h"
#include "Tools/Tools.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxOnLine::PopDialogBoxOnLine()
{
	
}
PopDialogBoxOnLine::~PopDialogBoxOnLine() {
	CCLog("~ <<%s>>",__FUNCTION__);
}
void PopDialogBoxOnLine::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxOnline.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pLOnLine=static_cast<UIListView*>(pUILayer->getWidgetByName("ListViewOnLine"));
	fPosX=pLOnLine->getPositionX();
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
	}
	//动画
	CCEaseExponentialOut  *out=CCEaseExponentialOut ::create(CCMoveTo::create(0.5,ccp(0,pLOnLine->getPositionY())));
	pLOnLine->runAction(out);

	//重设置下，否则会被覆盖事件
	this->setTouchEnabled(true);
	this->setTouchPriority(0);
	this->setTouchMode(kCCTouchesOneByOne);
}
void PopDialogBoxOnLine::onExit(){
	CCLayer::onExit();
}
bool PopDialogBoxOnLine::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	CCPoint touchPoint = this->convertTouchToNodeSpace(pTouch);
	CCRect rect= CCRect(pLOnLine->getPositionX(),pLOnLine->getPositionY(),pLOnLine->getContentSize().width,pLOnLine->getContentSize().height);
	if (!rect.containsPoint(touchPoint))
	{
		CCEaseExponentialOut  *out=CCEaseExponentialOut ::create(CCMoveTo::create(0.5,ccp(fPosX,pLOnLine->getPositionY())));
		CCCallFunc *call=CCCallFunc::create(this,SEL_CallFunc(&PopDialogBoxOnLine::removSelf));
		CCSequence *seq=CCSequence::create(out,call,NULL);
		pLOnLine->runAction(seq);
	}else
	{
		return false;
	}
	return true;
}
//设置在线用户信息
void PopDialogBoxOnLine::setOnLineUserInfo(bool isInsert,tagUserInfo userInfo){
	if (isInsert)
	{
		pLOnLine->insertDefaultItem(0);
	}
	UILabel *pLGold=static_cast<UILabel*>(pLOnLine->getItem(0)->getChildByName("LabelGold"));
	pLGold->setText(CCString::createWithFormat("%lld",userInfo.lScore)->getCString());
	if (Tools::GBKToUTF8(userInfo.szNickName))
	{
		UILabel *pLName=static_cast<UILabel*>(pLOnLine->getItem(0)->getChildByName("LabelName"));
		pLName->setText(Tools::GBKToUTF8(userInfo.szNickName));
	}
}
//移除自己
void PopDialogBoxOnLine::removSelf(){
	this->removeFromParentAndCleanup(true);
}