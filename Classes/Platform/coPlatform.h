/*
 * CEvent.h
 *
 *  Created on: 2015年4月3日
 *      Author: xw007
 */
#pragma once
#include <string>
/**
* 执行动作
* @param jsonString 动作参数 {act:0, ...}
* @return string 返回值json
*/
std::string platformAction(const std::string& jsonString);