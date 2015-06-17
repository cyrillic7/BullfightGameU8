#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class CStringAide
{
public:
	CStringAide();
	~CStringAide();

private:
	std::vector<std::string> parseUTF8(const std::string &str);
public:
	//GBK转UTF-8////////////////////////////////////////////////////////////////////////
	const char * GBKToUTF8(const char *strChar);
	//UTF-8转GBK////////////////////////////////////////////////////////////////////////
	const char * UTF8ToGBK(const char *strChar);
	//截取字符串（中英文）
	std::string subUTF8(const std::string &str, int from, int to);

};

