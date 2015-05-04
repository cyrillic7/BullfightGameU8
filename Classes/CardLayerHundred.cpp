

#include "CardLayerHundred.h"
CardLayerHundred::CardLayerHundred()
{
}
CardLayerHundred::~CardLayerHundred() {
}
void CardLayerHundred::onEnter(){
	CardLayer::onEnter();
}
void CardLayerHundred::onExit(){
	CardLayer::onExit();
}
void CardLayerHundred::updateGameState(){
	
	sendCard();
}
void CardLayerHundred::updateServerState(){
	
}