/*
 * PopDialogBoxOtherLoading.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxOtherLoading: public PopDialogBox {
private:
	UIImageView *pWidgetBg;
	UILabel *pLLoading;
	int timerIndex;
public:
	PopDialogBoxOtherLoading();
	~PopDialogBoxOtherLoading();
	CREATE_FUNC(PopDialogBoxOtherLoading);
private:
	virtual void onEnter();
	virtual void onExit();

	virtual void playAnimation();
	//更新加载文字动画
	void updateLoadingLabel(float dt);
};
