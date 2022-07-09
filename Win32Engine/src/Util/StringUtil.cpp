#include "StringUtil.h"

#include "../DefinwWindows.h"

std::wstring StringUtil::charPtrToWString(const char* str)
{
    const int len = MultiByteToWideChar(CP_ACP, 0, str, -1, nullptr, 0);
    wchar_t* pwc = new wchar_t[len + 1];
    
    MultiByteToWideChar(CP_ACP, 0, str, -1, pwc, len);
    pwc[len] = L'\0';

    std::wstring result = pwc;
    
    delete[] pwc;

    return result;
}

std::string StringUtil::wStringToString(const std::wstring& wStr)
{
    const int len = WideCharToMultiByte(CP_ACP, 0, wStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (len == 0)
        return std::string("");
    
    char* psz = new char[len];
    if (!psz)
        return std::string("");
    
    WideCharToMultiByte(CP_ACP, 0, wStr.c_str(), -1, psz, len, nullptr, nullptr);
    
    std::string str(psz);
    
    delete[] psz;
    psz = nullptr;
    
    return str;
}

std::wstring StringUtil::stringToWString(const std::string& str)
{
    const int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
    if (len == 0)
        return std::wstring(L"");

    wchar_t* pwc = new wchar_t[len];
    if (!pwc)
        return std::wstring(L"");

    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pwc, len);

    std::wstring result(pwc);

    delete[] pwc;
    pwc = nullptr;

    return result;
}
