#include "DefinwWindows.h"

#include "Win32UI/Window.h"
#include "Util/StringUtil.h"

int WINAPI wWinMain(
    HINSTANCE hInst,
    HINSTANCE hInstPrev,
    PWSTR lpCmdLine,
    int nCmdShow)
{
    try
    {
        Win32UI::Window wnd(800, 600, L"Engine");

        MSG msg;
        BOOL gResult;

        while ((gResult = GetMessageW(&msg, nullptr, 0, 0)) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        if (gResult == -1)
            return -1;
    
        return static_cast<int>(msg.wParam);
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
