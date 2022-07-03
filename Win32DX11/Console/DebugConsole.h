#pragma once

#include "../DefinwWindows.h"
#include <string>

class DebugConsole
{
public:
    DebugConsole();
    ~DebugConsole();
    DebugConsole(const DebugConsole&) = delete;
    DebugConsole& operator= (const DebugConsole&) = delete;

public:
    enum class Level
    {
        Normal,
        Warning,
        Error,
        Important
    };

public:
    void WriteLine(const std::wstring& msg, Level level = Level::Normal) const;

private:
    HANDLE _hConsole;
};  
