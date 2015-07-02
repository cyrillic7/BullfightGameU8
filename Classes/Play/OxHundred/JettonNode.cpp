/*
 * JettonNode.cpp
 *
 *  Created on: 2015年4月3日
 *      Author: xw007
 */

#include "JettonNode.h"

JettonNode::JettonNode()
:isReuse(false)
,fMoveSpeed(600)
{
	pIJetton=UIImageView::create();
	this->addChild(pIJetton);
    
}
JettonNode::~JettonNode() {
	CCLog("~<<%s>>",__FUNCTION__);
	this->removeAllChildrenWithCleanup(true);
}
void JettonNode::resetData(){
	isReuse=false;
}
//设置筹码类型并移动
void JettonNode::setJettonTypeWithMove(long long lJettonValue,CCPoint posBegin,CCPoint posEnd){
	pIJetton->setVisible(true);
	switch (lJettonValue)
	{
	case 1000:
		pIJetton->loadTexture("u_gih_jetton0.png",UI_TEX_TYPE_PLIST);
		break;
	case 5000:
		pIJetton->loadTexture("u_gih_jetton1.png",UI_TEX_TYPE_PLIST);
		break;
	case 10000:
		pIJetton->loadTexture("u_gih_jetton2.png",UI_TEX_TYPE_PLIST);
		break;
	case 100000:
		pIJetton->loadTexture("u_gih_jetton3.png",UI_TEX_TYPE_PLIST);
		break;
	case 500000:
		pIJetton->loadTexture("u_gih_jetton4.png",UI_TEX_TYPE_PLIST);
		break;
	default:
		pIJetton->loadTexture("u_gih_jetton0.png",UI_TEX_TYPE_PLIST);
		break;
	}
	pIJetton->setScale(0.5);
	this->setPosition(posBegin);

	float fDistance=ccpDistance(posBegin,posEnd);
	float fTime=fDistance/fMoveSpeed;
	//CCEaseQuarticActionOut *out=CCEaseQuarticActionOut::create(CCMoveTo::create(fTime,posEnd),0.2);
	//CCEaseOut *out=CCEaseOut::create(CCMoveTo::create(fTime,posEnd),0.8);
	int angle = abs(rand() % 2160);
	CCSpawn *pSpawn = CCSpawn::create(CCMoveTo::create(fTime, posEnd),CCRotateBy::create(fTime,angle),NULL);
	CCEaseExponentialOut  *out=CCEaseExponentialOut ::create(pSpawn);
	this->runAction(out);
	
	/*UUIImageView *jetton=IImageView::create();
	jetton->loadTexture("u_gih_jetton0.png",UI_TEX_TYPE_PLIST);
	this->addChild(jetton,1000);
	
	;
	jetton->setPosition(pos);*/
}
//设置筹码数、位置
void JettonNode::setJettonTypeWithPos(long long lJettonValue, CCPoint posEnd){
	pIJetton->setVisible(true);
	switch (lJettonValue)
	{
	case 1000:
		pIJetton->loadTexture("u_gih_jetton0.png", UI_TEX_TYPE_PLIST);
		break;
	case 5000:
		pIJetton->loadTexture("u_gih_jetton1.png", UI_TEX_TYPE_PLIST);
		break;
	case 10000:
		pIJetton->loadTexture("u_gih_jetton2.png", UI_TEX_TYPE_PLIST);
		break;
	case 100000:
		pIJetton->loadTexture("u_gih_jetton3.png", UI_TEX_TYPE_PLIST);
		break;
	case 500000:
		pIJetton->loadTexture("u_gih_jetton4.png", UI_TEX_TYPE_PLIST);
		break;
	default:
		pIJetton->loadTexture("u_gih_jetton0.png", UI_TEX_TYPE_PLIST);
		break;
	}
	pIJetton->setScale(0.5);
	this->setPosition(posEnd);
}
void JettonNode::hideJetton(){
	pIJetton->setVisible(false);
	isReuse=true;
}