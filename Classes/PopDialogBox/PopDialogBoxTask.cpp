/*
 * PopDialogBoxTask.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxTask.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
//////////////////////////////////////////////////////////////////////////
PopDialogBoxTask::PopDialogBoxTask()
{
    
	scheduleUpdate();
}
PopDialogBoxTask::~PopDialogBoxTask() {
	CCLog("~ <<%s>>",__FUNCTION__);
	unscheduleUpdate();
}
void PopDialogBoxTask::onEnter(){
	CCLayer::onEnter();
	Layout* layoutPauseUI = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCS_PATH_SCENE(UIPopDialogBoxTask.ExportJson)));
	pUILayer->addWidget(layoutPauseUI);
	
	pWidgetBg = static_cast<UIImageView*>(pUILayer->getWidgetByName("bg"));
	pWidgetBg->setScale(0.8);
	//关闭
	UIButton *backButton = static_cast<UIButton*>(pUILayer->getWidgetByName("buttonClose"));
	backButton->addTouchEventListener(this, toucheventselector(PopDialogBox::menuBack));
	
	playAnimation();

	
}
void PopDialogBoxTask::onExit(){
	CCLayer::onExit();
}
//更新
void PopDialogBoxTask::update(float delta){
	MessageQueueLobby::update(delta);
}
//////////////////////////////////////////////////////////////////////////
//网络数据
void PopDialogBoxTask::onEventReadMessage(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_MB_SOCKET:
	{
		CMD_GL_GetTask getTask;
		getTask.dwOpTerminal = 1;
		bool isSend= getSocket()->SendData(MDM_GL_C_DATA, SUB_GL_C_TASK_LOAD, &getTask, sizeof(getTask));
		CCLog(" <<%s>>", __FUNCTION__);
	}
		break;
	default:
		CCLog("main:%d <<%s>>",wMainCmdID, __FUNCTION__);
		break;
	}
}