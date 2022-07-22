#pragma once

#include <string>
#include <exception>
#include <vector>
#include "../DefinwWindows.h"

class BaseException : public std::exception
{
public:
    BaseException(int line, const char* file) noexcept;
    virtual const wchar_t* whatW() const noexcept;
    virtual const char* getType() const noexcept;
    int getLine() const noexcept;
    const std::wstring& getFile() const noexcept;
    std::wstring getOriginalString() const noexcept;

private:
    int _line;
    std::wstring _file;

protected:
    mutable std::wstring _whatBuffer;
    mutable wchar_t* _whatBufferW;
};

/*
 * 用来显示HResult的异常
 */
class HResultException : public BaseException
{
public:
    HResultException(int line, const char* file, HRESULT hResult);
    const wchar_t* whatW() const noexcept override;
    virtual const char* getType() const noexcept override;

public:
    static void check(HRESULT hr, int line, const char* file);

protected:
    static std::wstring hResultToString(HRESULT hResult) noexcept;
    HRESULT getHResult() const noexcept;
    std::wstring getExceptionString() const noexcept;
        
private:
    HRESULT _hResult;
};