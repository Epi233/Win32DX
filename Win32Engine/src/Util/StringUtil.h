#pragma once

#include <string>

class StringUtil
{
public:
    static std::wstring charPtrToWString(const char* str);
    static std::string wStringToString(const std::wstring& wStr);
    static std::wstring stringToWString(const std::string& str);
};
