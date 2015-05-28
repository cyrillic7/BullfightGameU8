#include "CMath.h"
#include "DataModel.h"
#include "GameConfig.h"
CMath::CMath()
{
}
CMath::~CMath() {
}
//坐标点转换
CCPoint CMath::designResolutionToFrame(CCPoint designPos){
	CCPoint pos;
	float posScaleX = designPos.x / SCENE_SIZE.width;
	pos.x = posScaleX*DataModel::sharedDataModel()->deviceSize.width;
	float posScaleY = designPos.y / SCENE_SIZE.height;
	pos.y = posScaleY*DataModel::sharedDataModel()->deviceSize.height;
	return pos;
}
std::string CMath::formatNumber(std::string strNum) {
	std::string formatString = "";
	int strLen = strNum.length();
	if (strLen > 9){
		formatString = strNum.substr(0, strLen - 7);
		formatString.insert(formatString.length() - 2, ".");
		formatString.append("十亿");
	}
	else if (strLen > 6){
		formatString = strNum.substr(0, strLen - 4);
		formatString.insert(formatString.length() - 2, ".");
		formatString.append("百万");
	}
	else if (strLen > 4){  //大于10K才行
		formatString = strNum.substr(0, strLen - 1);
		formatString.insert(formatString.length() - 2, ".");
		formatString.append("千 ");
	}
	else {
		formatString = strNum;
	}
	return formatString;
}

