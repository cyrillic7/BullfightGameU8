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
std::string CStringAide::GBKToUTF8(const char * strChar){
	std::string result = "";
	if (strcmp(strChar, "")==0)
	{
		return result;
	}
	iconv_t iconvH;
	//iconvH = iconv_open("unicode","ascii");
	iconvH = iconv_open("utf-8", "gbk");
	//iconvH = iconv_open("gbk","utf-8");
	if (iconvH == 0)
	{
		return NULL;
	}
	size_t strLength = strlen(strChar);
	size_t outLength = strLength << 2;
	size_t copyLength = outLength;
	
	char* outbuf = (char*)malloc(outLength);
	char* pBuff = outbuf;
	memset(outbuf, 0, outLength);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (-1 == iconv(iconvH, &strChar, &strLength, &outbuf, &outLength))
	{
		free(pBuff);
		iconv_close(iconvH);
		return result;
	}
#else
	if (-1 == iconv(iconvH, (char **)&strChar, &strLength, &outbuf, &outLength))
	{
		free(pBuff);
		iconv_close(iconvH);
		return result;
	}
#endif

	//memcpy(g_GBKConvUTF8Buf, pBuff, copyLength);
	result = pBuff;
	free(pBuff);
	iconv_close(iconvH);
	
	return result;
}
std::string  CStringAide::UTF8ToGBK(const char * strChar){
	std::string result = "";
	if (strcmp(strChar,"")==0)
	{
		return result;
	}
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
	
	char* outbuf = (char*)malloc(outLength);
	char* pBuff = outbuf;
	memset(outbuf, 0, outLength);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (-1 == iconv(iconvH, &strChar, &strLength, &outbuf, &outLength))
	{
		free(pBuff);
		iconv_close(iconvH);
		return result;
	}
#else
	if (-1 == iconv(iconvH, (char **)&strChar, &strLength, &outbuf, &outLength))
	{
		free(pBuff);
		iconv_close(iconvH);
		return result;
	}
#endif

	result = pBuff;
	free(pBuff);
	iconv_close(iconvH);
	return result;
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
////是否中文
bool CStringAide::isAllChniese(std::string string){
	if (string.size() < 4)
	{
		return false;
	}
	for (std::string::iterator iter = string.begin(); iter != string.end(); ++iter)
	{
		if (*iter <= 0 || *iter >= 127)
		{
			iter += 2;
			return true;
		}
		else
		{
			
		}
	}
	return false;
}
//是否全是数字
bool CStringAide::isDigit2(std::string str) {
	for (int i = 0; i < str.length(); i++)
	{
		if ((str.at(i) > '9') || (str.at(i) < '0'))
		{
			return   false;
		}
	}
  return  true;
}
//分割字符串
CCArray* CStringAide::split(const char* srcStr, const char* sSep){
	CCArray* stringList = CCArray::create();
	int size = strlen(srcStr);
	/* 将数据转换为Cocos2d-x字符串对象 */
	CCString* str = CCString::create(srcStr);
	int startIndex = 0;
	int endIndex = 0;
	endIndex = str->m_sString.find(sSep);
	CCString* spliStr = NULL;
	/* 根据分隔符拆分字符串，并添加到列表中 */
	while (endIndex > 0) {
		spliStr = CCString::create("");
		/* 截取字符串 */
		spliStr->m_sString = str->m_sString.substr(startIndex, endIndex);
		/* 添加字符串到列表 */
		stringList->addObject(spliStr);
		/* 截取剩下的字符串 */
		str->m_sString = str->m_sString.substr(endIndex + 1, size);
		/* 查找下一个分隔符所在下标 */
		endIndex = str->m_sString.find(sSep);
	}
	/* 剩下的字符串也添加进列表 */
	if (str->m_sString.compare("") != 0) {
		stringList->addObject(CCString::create(str->m_sString));
	}
	return stringList;
}
string&   CStringAide::replace_all(string&   str, const   string&   old_value, const   string&   new_value)
{
	while (true)   {
		string::size_type   pos(0);
		if ((pos = str.find(old_value)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else   break;
	}
	return   str;
}