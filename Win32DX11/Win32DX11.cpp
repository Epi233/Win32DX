#include "DefinwWindows.h"

#include "Win32UI/Window.h"
#include "Application.h"

int WINAPI wWinMain(
    HINSTANCE hInst,
    HINSTANCE hInstPrev,
    PWSTR lpCmdLine,
    int nCmdShow)
{
    
    try
    {
        Application app(800, 600, L"Engine");

        app.begin();
    }
    catch (const BaseException& e)
    {
        MessageBoxW(nullptr, e.whatW(), L"Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        MessageBoxA(nullptr, e.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        MessageBoxW(nullptr, L"No Desc Exception", L"Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    return -1;

}
