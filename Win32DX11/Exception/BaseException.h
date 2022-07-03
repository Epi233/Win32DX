#pragma once

#include <string>
#include <exception>

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
