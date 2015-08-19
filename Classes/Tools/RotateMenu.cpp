#include "RotateMenu.h"
#include <math.h>
#define PI acos(-1)
USING_NS_CC;
bool RotateMenu::init(){
	if (!CCLayer::init())
		return false;
	_angle = 0.0;
	this->ignoreAnchorPointForPosition(false);
	_selectedItem = NULL;
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	this->setContentSize(s / 3 * 2);
	this->setAnchorPoint(ccp(0.5f, 0.5f));
	/*auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(RotateMenu::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(RotateMenu::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(RotateMenu::onTouchEnded, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);*/
	this->setTouchEnabled(true);
	this->setTouchPriority(0);
	this->setTouchMode(kCCTouchesOneByOne);
	return true;
};

void RotateMenu::addMenuItem(cocos2d::CCMenuItem *item){
	item->setPosition(this->getContentSize() / 2);
	this->addChild(item);
	_items.push_back(item);
	setUnitAngle(2 * PI / _items.size());
	reset();
	updatePositionWithAnimation();
	return;
}
void RotateMenu::updatePosition(){
	CCSize menuSize = getContentSize();
	float disY = menuSize.height / 30;
	float disX = menuSize.width / 3;
	for (int i = 0; i < _items.size(); i++){
		float x = menuSize.width / 2 + disX*sin(i*_unitAngle + getAngle());
		float y = menuSize.height / 2 - disY*cos(i*_unitAngle + getAngle());
		_items.at(i)->setPosition(ccp(x, y));
		_items.at(i)->setZOrder(-(int)y);
		//Opacity  129~255
		_items.at(i)->setOpacity(220 + 25 * cos(i*_unitAngle + getAngle()));
		_items.at(i)->setScale(0.75 + 0.25*cos(i*_unitAngle + getAngle()));
	}
	return;
}
void RotateMenu::updatePositionWithAnimation(){
	//先停止所有可能存在的动作
	for (int i = 0; i < _items.size(); i++)
		_items.at(i)->stopAllActions();
	CCSize menuSize = getContentSize();
	float disY = menuSize.height / 30;
	float disX = menuSize.width / 3;
	for (int i = 0; i < _items.size(); i++){
		float x = menuSize.width / 2 + disX*sin(i*_unitAngle + getAngle());
		float y = menuSize.height / 2 - disY*cos(i*_unitAngle + getAngle());
		CCMoveTo *moveTo = CCMoveTo::create(animationDuration, ccp(x, y));
		_items.at(i)->runAction(moveTo);
		//Opacity  129~255
		GLubyte opacity = (220 + 25 * cos(i*_unitAngle + getAngle()));
		CCFadeTo *fadeTo = CCFadeTo::create(animationDuration, opacity);
		_items.at(i)->runAction(fadeTo);
		//缩放比例  0.5~1
		CCScaleTo *scaleTo = CCScaleTo::create(animationDuration, 0.75 + 0.25*cos(i*_unitAngle + getAngle()));
		_items.at(i)->runAction(scaleTo);
		_items.at(i)->setZOrder(-(int)y);
	}
	scheduleOnce(schedule_selector(RotateMenu::actionEndCallBack), animationDuration);
	return;
}
void RotateMenu::reset(){
	_angle = 0;
}
void RotateMenu::setAngle(float angle){
	this->_angle = angle;
}
float RotateMenu::getAngle(){
	return _angle;
}
void RotateMenu::setUnitAngle(float angle){
	_unitAngle = angle;
}
float RotateMenu::getUnitAngle(){
	return _unitAngle;
}

float RotateMenu::disToAngle(float dis){
	float width = this->getContentSize().width / 2;
	return dis / width*getUnitAngle();
}

CCMenuItem * RotateMenu::getCurrentItem(){
	if (_items.size() == 0)
		return NULL;
	//这里实际加上了0.1getAngle(),用来防止精度丢失
	int  index = (int)((2 * PI - getAngle()) / getUnitAngle() + 0.1*getUnitAngle());
	index %= _items.size();
	return _items.at(index);
}


bool RotateMenu::ccTouchBegan(CCTouch* touch, CCEvent* event){
	//先停止所有可能存在的动作
	for (int i = 0; i < _items.size(); i++)
		_items.at(i)->stopAllActions();

	CCPoint position = this->convertToNodeSpace(touch->getLocation());
	CCSize size = this->getContentSize();
	CCRect rect = CCRect(0, 0, size.width, size.height);
	if (rect.containsPoint(position)){
		if (_selectedItem)
			_selectedItem->unselected();
		return true;
	}
	return false;
}
void RotateMenu::ccTouchEnded(CCTouch* touch,CCEvent* event){
	float xDelta = touch->getLocation().x - touch->getStartLocation().x;
	rectify(xDelta > 0);
	if (disToAngle(fabs(xDelta)) < getUnitAngle() / 100 && _selectedItem)
	{
		CCPoint position = this->convertTouchToNodeSpace(touch);
		CCPoint itemPos = _selectedItem->getPosition();
		CCRect rect = CCRect(itemPos.x - _selectedItem->getContentSize().width / 2, itemPos.y - _selectedItem->getContentSize().height / 2, _selectedItem->getContentSize().width, _selectedItem->getContentSize().height);
		if (rect.containsPoint(position))
		{
			_selectedItem->activate();
		}
	}
		
	updatePositionWithAnimation();
	return;
}
void RotateMenu::ccTouchMoved(CCTouch* touch, CCEvent* event){
	float angle = disToAngle(touch->getDelta().x);
	setAngle(getAngle() + angle);
	updatePosition();
	return;
}

void RotateMenu::rectify(bool forward){
	float angle = getAngle();
	while (angle<0)
		angle += PI * 2;
	while (angle>PI * 2)
		angle -= PI * 2;
	if (forward > 0)
		angle = ((int)((angle + getUnitAngle() / 3 * 2) / getUnitAngle()))*getUnitAngle();
	else
		angle = ((int)((angle + getUnitAngle() / 3) / getUnitAngle()))*getUnitAngle();
	setAngle(angle);
}

void RotateMenu::actionEndCallBack(float dx){
	_selectedItem = getCurrentItem();
	if (_selectedItem)
		_selectedItem->selected();
}