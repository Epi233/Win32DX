#include "Window.h"
#include <sstream>
#include "../resource.h"

namespace Win32UI
{
    // static member init
    Window::InternalWindow Window::InternalWindow::_instance;

    Window::InternalWindow::InternalWindow()
        : _hInst(GetModuleHandle(nullptr))
    {
        // icon
        HICON hIcon = static_cast<HICON>(LoadIcon(_hInst, MAKEINTRESOURCE(IDI_ICON1)));
        
        WNDCLASSEX wc;
        wc.cbSize = sizeof(wc);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = HandleMsgSetup;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = getInstance();
        wc.hIcon = hIcon;
        wc.hCursor = nullptr;
        wc.hbrBackground = nullptr;
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = getWindowName();
        wc.hIconSm = hIcon;

        RegisterClassExW(&wc);
    }

    Window::InternalWindow::~InternalWindow()
    {
        UnregisterClassW(WND_NAME, getInstance());
    }

    LPCWSTR Window::InternalWindow::getWindowName() noexcept
    {
        return WND_NAME;
    }

    HINSTANCE Window::InternalWindow::getInstance() noexcept
    {
        return _instance._hInst;
    }

    Window::Window(int width, int height, const wchar_t* name) noexcept
    {
        RECT rect;
        rect.left = 100;
        rect.right = width + rect.left;
        rect.top = 100;
        rect.bottom = height + rect.top;

        DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

        AdjustWindowRect(&rect, style, FALSE);

        _hWnd = CreateWindowExW(
            0,
            InternalWindow::getWindowName(),
            name,
            style,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            rect.right - rect.left,
            rect.bottom - rect.top,
            nullptr,
            nullptr,
            InternalWindow::getInstance(),
            this // 通过WM_NCCREATE把自己发出去，通过这个方式建立实例指针和winApi之间的联系
            );

        ShowWindow(_hWnd, SW_SHOWDEFAULT);
    }

    Window::~Window()
    {
        DestroyWindow(_hWnd);
    }

    LRESULT Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        // 首次创建窗口时，在WM_CREATE消息之前发送，LPARAM指向 CREATESTRUCT 结构的指针，其中包含有关正在创建的窗口的信息
        if (msg == WM_NCCREATE)
        {
            const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
            Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

            // 把窗口实例的指针存到winApi一侧，这样只要通过hWnd，总能拿到Window*
            SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

            // 把处理函数强制调整为第二个
            SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgReally));
            
            return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    LRESULT Window::HandleMsgReally(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }

    LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
    {
        switch (msg)
        {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    /*
     * Exception
     */

    Window::Exception::Exception(int line, const char* file, HRESULT hResult)
        : BaseException(line, file)
        , _hResult(hResult)
    {
    }

    const wchar_t* Window::Exception::whatW() const noexcept
    {
        std::wostringstream oss;
        oss << getType() << std::endl;
        oss << "[H Result] " << getHResult() << std::endl;
        oss << "[Desc] " << getExceptionString() << std::endl;
        oss << getOriginalString();

        _whatBuffer = oss.str();
        return _whatBuffer.c_str();
    }

    const char* Window::Exception::getType() const noexcept
    {
        return "Window Exception";
    }

    HRESULT Window::Exception::getHResult() const noexcept
    {
        return _hResult;
    }

    std::wstring Window::Exception::getExceptionString() const noexcept
    {
        return hResultToString(_hResult);
    }

    std::wstring Window::Exception::hResultToString(HRESULT hResult) noexcept
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



}