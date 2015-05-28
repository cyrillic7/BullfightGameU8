/*
 * PopDialogBoxLoading.h
 *
 *  Created on: 2015年3月17日
 *      Author: 恒
 */

#pragma once

#include "PopDialogBox.h"
class PopDialogBoxLoading: public PopDialogBox {
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
};
