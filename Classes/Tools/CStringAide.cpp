#include "CStringAide.h"
CStringAide::CStringAide()
{
}
CStringAide::~CStringAide() {
}


#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "iconv\iconv.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include <iconv.h>
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../../../../libiconv/include/iconv.h"
#endif
static char g_GBKConvUTF8Buf[5000] = { 0 };


const char * CStringAide::GBKToUTF8(const char * strChar){
	iconv_t iconvH;
	//iconvH = iconv_open("unicode","ascii");
	iconvH = iconv_open("utf-8", "gb2312");
	//iconvH = iconv_open("gbk","utf-8");
	if (iconvH == 0)
	{
		return NULL;
	}
	size_t strLength = strlen(strChar);
	size_t outLength = strLength << 2;
	size_t copyLength = outLength;
	memset(g_GBKConvUTF8Buf, 0, 5000);

	char* outbuf = (char*)malloc(outLength);
	char* pBuff = outbuf;
	memset(outbuf, 0, outLength);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (-1 == iconv(iconvH, &strChar, &strLength, &outbuf, &outLength))
	{
		free(pBuff);
		iconv_close(iconvH);
		return "";
	}
#else
	if (-1 == iconv(iconvH, (char **)&strChar, &strLength, &outbuf, &outLength))
	{
		free(pBuff);
		iconv_close(iconvH);
		return "";
	}
#endif

	memcpy(g_GBKConvUTF8Buf, pBuff, copyLength);
	free(pBuff);
	iconv_close(iconvH);
	return g_GBKConvUTF8Buf;
}
const char * CStringAide::UTF8ToGBK(const char * strChar){
	iconv_t iconvH;
	//iconvH = iconv_open("unicode","ascii");
	//iconvH = iconv_open("utf-8", "gb2312");
	iconvH = iconv_open("gbk","utf-8");
	if (iconvH == 0)
	{
		return NULL;
	}
	size_t strLength = strlen(strChar);
	size_t outLength = strLength << 2;
	size_t copyLength = outLength;
	memset(g_GBKConvUTF8Buf, 0, 5000);

	char* outbuf = (char*)malloc(outLength);
	char* pBuff = outbuf;
	memset(outbuf, 0, outLength);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (-1 == iconv(iconvH, &strChar, &strLength, &outbuf, &outLength))
	{
		free(pBuff);
		iconv_close(iconvH);
		return "";
	}
#else
	if (-1 == iconv(iconvH, (char **)&strChar, &strLength, &outbuf, &outLength))
	{
		free(pBuff);
		iconv_close(iconvH);
		return "";
	}
#endif

	memcpy(g_GBKConvUTF8Buf, pBuff, copyLength);
	free(pBuff);
	iconv_close(iconvH);
	return g_GBKConvUTF8Buf;
}
//////////////////////////////////////////////////////////////////////////
std::vector<std::string> CStringAide::parseUTF8(const std::string &str)
{
	int l = str.length();
	std::vector<std::string> ret;
	ret.clear();
	for (int p = 0; p < l;)
	{
		int size = 0;
		unsigned char c = str[p];
		if (c < 0x80) {
			size = 1;
		}
		else if (c < 0xc2)
		{
			size = 2;
		}
		else if (c < 0xe0)
		{
			size = 2;
		}
		else if (c < 0xf0)
		{
			size = 3;
		}
		else if (c < 0xf8)
		{
			size = 4;
		}
		else if (c < 0xfc)
		{
			size = 5;
		}
		else if (c < 0xfe)
		{
			size = 6;
		}
		else
			size = 7;
		std::string temp = "";
		temp = str.substr(p, size);
		ret.push_back(temp);
		p += size;
	}
	return ret;
}

std::string CStringAide::subUTF8(const std::string &str, int from, int to)
{
	if (from > to) return "";
	if (str.length() < to) return "";
	std::vector<std::string> vstr = parseUTF8(str);
	if (to > vstr.size())
	{
		to = vstr.size();
	}
	std::vector<std::string>::iterator iter = vstr.begin();
	std::string res;
	std::string result;
	for (iter = (vstr.begin() + from); iter != (vstr.begin() + to); iter++)
	{
		res += *iter;
	}
	if (vstr.size() > from + to)
	{
		res += "...";
	}
	return res;
}
