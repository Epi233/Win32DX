#include "EngineException.h"
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

HResultException::HResultException(int line, const char* file, HRESULT hResult)
    : BaseException(line, file)
    , _hResult(hResult)
{
}

const wchar_t* HResultException::whatW() const noexcept
{
    std::wostringstream oss;
    oss << getType() << std::endl;
    oss << "[H Result] " << getHResult() << std::endl;
    oss << "[Desc] " << getExceptionString() << std::endl;
    oss << getOriginalString();

    _whatBuffer = oss.str();
    return _whatBuffer.c_str();
}

const char* HResultException::getType() const noexcept
{
    return "HResult Exception";
}

void HResultException::check(HRESULT hr, int line, const char* file)
{
    if (FAILED(hr))
        throw HResultException(line, file, hr);
}

HRESULT HResultException::getHResult() const noexcept
{
    return _hResult;
}

std::wstring HResultException::getExceptionString() const noexcept
{
    return hResultToString(_hResult);
}

std::wstring HResultException::hResultToString(HRESULT hResult) noexcept
{
    wchar_t* pMsgBuffer = nullptr;
    DWORD nMsgLen = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        hResult,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPWSTR>(&pMsgBuffer),
        0,
        nullptr);

    if (nMsgLen == 0)
        return L"UnKnow Code";

    std::wstring errorStr = pMsgBuffer;
    LocalFree(pMsgBuffer);
    return errorStr;
}
