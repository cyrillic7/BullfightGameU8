//头文件
#ifndef _JsonAide_H_
#define _JsonAide_H_

#include <string>
#include "json/document.h"

class JsonAide
{
public:
	JsonAide(const std::string& sJsonString);
	// 原始字符串
	const std::string&	srcString() const;
	bool				hasError() const;
	const char*			getError() const;

	// 获取数据
	int				getInt(const std::string& key, int iDefault = 0) const;
	unsigned int	getUint(const std::string& key, unsigned int iDefault = 0) const;
	long long		getInt64(const std::string& key, long long lDefault = 0L) const;
	unsigned long long getUint64(const std::string& key, unsigned long long lDefault = 0L) const;
	double			getDouble(const std::string& key, double dDefault = 0) const;
	std::string		getString(const std::string& key, const std::string& sDefault = "") const;

private:
	std::string			mSrcString;
	rapidjson::Document mDoc;
};

#endif // _JsonAide_H_