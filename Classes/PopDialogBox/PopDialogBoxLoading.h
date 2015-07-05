/*
 * PopDialogBoxLoading.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
struct IOutTime//连接超时
{
	virtual ~IOutTime(){}
	//超时回调
	virtual void onOutTime() = 0;
};
class PopDialogBoxLoading: public PopDialogBox {
public:
	CC_SYNTHESIZE(IOutTime*, pIOutTime,IOutTime);
private:
    
	UIImageView *pWidgetBg;
	UILabel *pLLoading;
	int timerIndex;
	
public:
	PopDialogBoxLoading();
	~PopDialogBoxLoading();
	CREATE_FUNC(PopDialogBoxLoading);

private:
	virtual void onEnter();
	virtual void onExit();

	virtual void playAnimation();
	//更新加载文字动画
	void updateLoadingLabel(float dt);
	//超时关闭
	void outTimeExit(float dt);
};
