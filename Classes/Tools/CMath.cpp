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

