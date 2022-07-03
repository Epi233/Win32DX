#include "DebugConsole.h"

DebugConsole::DebugConsole()
{
    AllocConsole();
    _hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

DebugConsole::~DebugConsole()
{
    CloseHandle(_hConsole);
    FreeConsole();
}

void DebugConsole::WriteLine(const std::wstring& msg, Level level) const
{
    WORD textAttr;
    switch (level)
    {
    case Level::Normal:
        textAttr = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        SetConsoleTextAttribute(_hConsole, textAttr);
        break; 
    case Level::Warning:
        textAttr = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE;
        SetConsoleTextAttribute(_hConsole, textAttr);
        break;
    case Level::Error:
        textAttr = FOREGROUND_INTENSITY | FOREGROUND_RED;
        SetConsoleTextAttribute(_hConsole, textAttr);
        break;
    case Level::Important:
        textAttr = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE;
        SetConsoleTextAttribute(_hConsole, textAttr);
        break;
    }

    std::wstring outputStr(msg);
    outputStr.append(L"\n");
    
    const wchar_t* str = outputStr.c_str();
    const int len = lstrlenW(str);
    WriteConsoleW(_hConsole, str, len, nullptr, nullptr);
}
