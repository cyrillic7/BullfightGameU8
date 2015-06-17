

#include "ChairData.h"
ChairData::ChairData()
{
}
ChairData::~ChairData() {
}
//用户坐下
void ChairData::setUserSitState(tagUserInfo userInfo,bool isSit){
	if (isSit)
	{
		pBDonw->setEnabled(false);
		pLUserName->setVisible(true);
		pIVUserIcon->setVisible(true);
		pLUserName->setText(subUTF8(GBKToUTF8(userInfo.szNickName),0,4).c_str());
	}
	else
	{
		pBDonw->setEnabled(true);
		pLUserName->setVisible(false);
		pIVUserIcon->setVisible(false);
		pLUserName->setText("");
	}
	
}