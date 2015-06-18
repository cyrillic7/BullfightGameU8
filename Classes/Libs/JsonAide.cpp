//源文件
#include "cocos2d.h"
#include "JsonAide.h"

USING_NS_CC;
using namespace rapidjson;

JsonAide::JsonAide(const std::string& sJsonString)
: mSrcString(sJsonString)
{
	try
	{
		mDoc.Parse<rapidjson::kParseDefaultFlags>(sJsonString.c_str());
	}
	catch (...)
	{
	}
}

const std::string& JsonAide::srcString() const
{
	return mSrcString;
}

bool JsonAide::hasError() const
{
	return mDoc.HasParseError();
}

const char*	JsonAide::getError() const
{
	return mDoc.GetParseError();
}

int JsonAide::getInt(const std::string& key, int iDefault) const
{
	do 
	{
		if (!mDoc.HasMember(key.c_str()))
			break;
		if (!mDoc[key.c_str()].IsInt())
			break;
		return mDoc[key.c_str()].GetInt();
	} while (0);
	return iDefault;
}

unsigned int JsonAide::getUint(const std::string& key, unsigned int iDefault) const
{
	do
	{
		if (!mDoc.HasMember(key.c_str()))
			break;
		if (!mDoc[key.c_str()].IsUint())
			break;
		return mDoc[key.c_str()].GetUint();
	} while (0);
	return iDefault;
}


long long JsonAide::getInt64(const std::string& key, long long lDefault) const
{
	do
	{
		if (!mDoc.HasMember(key.c_str()))
			break;
		if (!mDoc[key.c_str()].IsInt64())
			break;
		return mDoc[key.c_str()].GetInt64();
	} while (0);
	return lDefault;
}

unsigned long long JsonAide::getUint64(const std::string& key, unsigned long long lDefault) const
{
	do
	{
		if (!mDoc.HasMember(key.c_str()))
			break;
		if (!mDoc[key.c_str()].IsUint64())
			break;
		return mDoc[key.c_str()].GetUint64();
	} while (0);
	return lDefault;
}

double JsonAide::getDouble(const std::string& key, double dDefault) const
{
	do
	{
		if (!mDoc.HasMember(key.c_str()))
			break;
		if (!mDoc[key.c_str()].IsDouble())
			break;
		return mDoc[key.c_str()].GetDouble();
	} while (0);
	return dDefault;
}


std::string JsonAide::getString(const std::string& key, const std::string& sDefault) const
{
	do
	{
		if (!mDoc.HasMember(key.c_str()))
			break;
		if (!mDoc[key.c_str()].IsString())
			break;
		return mDoc[key.c_str()].GetString();
	} while (0);
	return sDefault;
}

