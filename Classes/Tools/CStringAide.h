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

    
	//GBK转UTF-8////////////////////////////////////////////////////////////////////////
	const char * GBKToUTF8(const char *strChar);
	//UTF-8转GBK////////////////////////////////////////////////////////////////////////
	const char * UTF8ToGBK(const char *strChar);
};

