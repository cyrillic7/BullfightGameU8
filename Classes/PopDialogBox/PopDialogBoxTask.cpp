/*
 * PopDialogBoxTask.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#include "PopDialogBoxTask.h"
#include "../Tools/DataModel.h"
#include "../Tools/GameConfig.h"
#include "PopDialogBoxLoading.h"
#include "../Network/ListernerThread/LobbyGameListerner.h"
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
	//任务列表
	pLVTask = static_cast<UIListView*>(pUILayer->getWidgetByName("ListViewTask"));
	//设置cell样式
	pLVTask->setItemModel(pLVTask->getItem(0));
	pLVTask->removeAllItems();
	//连接失败容器
	pPFailure = static_cast<UIPanel*>(pUILayer->getWidgetByName("PanelNothing"));
	pPFailure->setVisible(false);
	//获取列表
	getTashInfoList();
	//
	playAnimation();

	
}
void PopDialogBoxTask::onExit(){
	CCLayer::onExit();
}
//获取任务列表
void PopDialogBoxTask::getTashInfoList(){
	switch (getSocket()->eSocketState)
	{
	case TCPSocket::SOCKET_STATE_CONNECT_SUCCESS:
	{
		PopDialogBox *box = PopDialogBoxLoading::create();
		this->addChild(box, 10, TAG_LOADING);
		box->setSocketName(SOCKET_LOBBY);

		CMD_GL_GetTask getTask;
		getTask.dwOpTerminal = 1;
		bool isSend = getSocket()->SendData(MDM_GL_C_DATA, SUB_GL_C_TASK_LOAD, &getTask, sizeof(getTask));
	}
	break;
	default:
	{
		//显示连接失败
		pPFailure->setVisible(true);
		UIImageView *pIVFailure = static_cast<UIImageView*>(pUILayer->getWidgetByName("ImageFailure"));
		CCSequence *pSeq = CCSequence::create(CCRotateTo::create(1, 3), CCRotateTo::create(1, -3), NULL);
		CCRepeatForever *pRForever = CCRepeatForever::create(pSeq);
		pIVFailure->runAction(pRForever);
		//断开连接
		TCPSocketControl *tcpControl = TCPSocketControl::sharedTCPSocketControl();
		tcpControl->stopSocket(SOCKET_LOBBY);
		tcpControl->removeTCPSocket(SOCKET_LOBBY);
		//重新连接
		TCPSocket *tcp = TCPSocketControl::sharedTCPSocketControl()->getTCPSocket(SOCKET_LOBBY);
		if (tcp){
			tcp->createSocket(DataModel::sharedDataModel()->sLobbyIp.c_str(), DataModel::sharedDataModel()->lLobbyProt, new LobbyGameListerner());
		}



	}
	break;
	}
}
//更新任务列表
void PopDialogBoxTask::updateListTask(){
	UIListView *pLVTemp = pLVTask;
	pLVTemp->removeAllItems();

	int tempSize = vecTaskInfo.size();
	if (tempSize == 0)
	{
		return;
	}

	for (int i = 0; i < tempSize; i++)
	{
		int inserterPos = pLVTemp->getItems()->count();
		pLVTemp->insertDefaultItem(inserterPos);
		//任务名称
		UILabel *pTaskName = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("ImageTitle")->getChildByName("LabelTitle"));
		pTaskName->setText(GBKToUTF8(vecTaskInfo[i].szTitle));
		//任务说明
		UILabel *pTaskEcplain = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("LabelExplain"));
		pTaskEcplain->setText(GBKToUTF8(vecTaskInfo[i].szRemarks));
		//进度条
		UILoadingBar *pLBTask = static_cast<UILoadingBar*>(pLVTemp->getItem(inserterPos)->getChildByName("ImageProgressBar")->getChildByName("ProgressBarTask"));
		float fCurProgress = vecTaskInfo[i].dwCurProgress;
		float fPercent = fCurProgress / vecTaskInfo[i].dwProgress;
		if (fPercent>1)
		{
			fPercent = 1;
		}
		pLBTask->setPercent(fPercent * 100);
		//当前进度
		UILabel *pLCurProgress = static_cast<UILabel*>(pLVTemp->getItem(inserterPos)->getChildByName("LabelCurProgress"));
		pLCurProgress->setText(CCString::createWithFormat("%ld/%ld",vecTaskInfo[i].dwCurProgress,vecTaskInfo[i].dwProgress)->getCString());
		//领取按键
		UIButton *pButton = static_cast<UIButton*>(pLVTemp->getItem(inserterPos)->getChildByName("ButtonReward"));
		pButton->setTag(inserterPos);
		pButton->addTouchEventListener(this, SEL_TouchEvent(&PopDialogBoxTask::onMenuReward));

		if (vecTaskInfo[i].dwCurProgress>=vecTaskInfo[i].dwProgress)
		{
			pButton->setEnabled(true);
			pLCurProgress->setVisible(false);
		}
		else
		{
			pButton->setEnabled(false);
			pLCurProgress->setVisible(true);
		}
	}
}
//领取奖励按键
void PopDialogBoxTask::onMenuReward(CCObject *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED){
		UIButton *pBTemp = (UIButton*)object;
		CCLog("tag:%d <<%s>>",pBTemp->getTag(), __FUNCTION__);
	}
}
//更新
void PopDialogBoxTask::update(float delta){
	MessageQueueLobby::update(delta);
}
//////////////////////////////////////////////////////////////////////////
//网络数据
void PopDialogBoxTask::onEventReadMessageLobby(WORD wMainCmdID, WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wMainCmdID)
	{
	case MDM_GL_C_DATA:
	{
		onEventTask(wSubCmdID, pDataBuffer, wDataSize);
	}
		break;
	default:
		CCLog("main:%d %d<<%s>>",wMainCmdID,wSubCmdID, __FUNCTION__);
		break;
	}
}
//任务
void PopDialogBoxTask::onEventTask(WORD wSubCmdID, void * pDataBuffer, unsigned short wDataSize){
	switch (wSubCmdID)
	{
	case SUB_GL_C_TASK_LOAD:
	{
		onSubTaskList(pDataBuffer, wDataSize);
	}
		break;
	default:
		CCLog("sub:%d <<%s>>",wSubCmdID, __FUNCTION__);
		break;
	}
}
//任务列表
void PopDialogBoxTask::onSubTaskList(void * pDataBuffer, unsigned short wDataSize){
	assert(wDataSize >= sizeof(CMD_GL_TaskInfo));
	int count = wDataSize / sizeof(CMD_GL_TaskInfo);
	
	BYTE cbDataBuffer[SOCKET_TCP_PACKET + sizeof(TCP_Head)];
	CopyMemory(cbDataBuffer, pDataBuffer, wDataSize);

	for (int i = 0; i < count; i++)
	{
		void * pDataBuffer = cbDataBuffer + i*sizeof(CMD_GL_TaskInfo);
		CMD_GL_TaskInfo *pTaskInfo = (CMD_GL_TaskInfo*)pDataBuffer;
	
		CMD_GL_TaskInfo taskInfo;

		taskInfo.dwTaskID = pTaskInfo->dwTaskID;
		taskInfo.dwSortID = pTaskInfo->dwSortID;
		taskInfo.dwProgress = pTaskInfo->dwProgress;
		taskInfo.dwCurProgress = pTaskInfo->dwCurProgress;

		strcpy(taskInfo.szTitle,pTaskInfo->szTitle);
		strcpy(taskInfo.szRemarks, pTaskInfo->szRemarks);
		strcpy(taskInfo.szImgName, pTaskInfo->szImgName);
		//
		taskInfo.Award.dwAward1 = pTaskInfo->Award.dwAward1;
		taskInfo.Award.dwAward2 = pTaskInfo->Award.dwAward2;
		taskInfo.Award.dwAward3 = pTaskInfo->Award.dwAward3;
		strcpy(taskInfo.Award.szAward1, pTaskInfo->Award.szAward1);
		strcpy(taskInfo.Award.szAward2, pTaskInfo->Award.szAward2);
		strcpy(taskInfo.Award.szAward3, pTaskInfo->Award.szAward3);
		
		vecTaskInfo.push_back(taskInfo);
	}
	updateListTask();
	//移除loading
	this->getChildByTag(TAG_LOADING)->removeFromParentAndCleanup(true);
}