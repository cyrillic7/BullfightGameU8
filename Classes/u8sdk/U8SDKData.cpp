#include "U8SDKData.h"

U8PayParams::U8PayParams()
{

}
U8PayParams::~U8PayParams()
{

}

U8PayParams* U8PayParams::create()
{
	U8PayParams* ret = new U8PayParams();
	if (ret)
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret); 
	return NULL;
}


U8ExtraGameData::U8ExtraGameData()
{

}
U8ExtraGameData::~U8ExtraGameData()
{

}

U8ExtraGameData* U8ExtraGameData::create()
{
	U8ExtraGameData* ret = new U8ExtraGameData();
	if (ret)
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return NULL;
}

U8LoginResult::U8LoginResult()
{

}

U8LoginResult::~U8LoginResult()
{

}

U8LoginResult* U8LoginResult::create()
{
	U8LoginResult* ret = new U8LoginResult();
	if (ret)
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return NULL;
}