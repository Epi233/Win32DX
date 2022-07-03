#include "BaseException.h"
#include <sstream>
#include "../Util/StringUtil.h"

BaseException::BaseException(int line, const char* file) noexcept
    : _line(line)
    , _file(StringUtil::charPtrToWString(file))
{
}

const wchar_t* BaseException::whatW() const noexcept
{
    std::wostringstream oss;
    oss << getType() << std::endl << getOriginalString();

    _whatBuffer = oss.str();
    return _whatBuffer.c_str();
}

const char* BaseException::getType() const noexcept
{
    return "BaseException";
}

int BaseException::getLine() const noexcept
{
    return _line;
}

const std::wstring& BaseException::getFile() const noexcept
{
    return _file;
}

std::wstring BaseException::getOriginalString() const noexcept
{
    std::wostringstream oss;
    oss << L"[File] " << _file << std::endl << L"[Line] " << _line;
    return oss.str();
}





