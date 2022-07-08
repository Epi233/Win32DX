#include "Window.h"
#include <sstream>
#include "../resource.h"

namespace Win32UI
{
    Window::Window(int width, int height, const wchar_t* name) noexcept
        : _width(width)
        , _height(height)
        , _hInst(GetModuleHandle(nullptr))
    {
        // icon
        HANDLE hIcon = LoadImage(_hInst, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 64, 64, 0);

        WNDCLASSEX wc;

        wc.cbSize = sizeof(wc);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = HandleMsgSetup;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = _hInst;
        wc.hIcon = static_cast<HICON>(hIcon);
        wc.hCursor = nullptr;
        wc.hbrBackground = nullptr;
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = WND_CLASS_NAME;
        wc.hIconSm = static_cast<HICON>(hIcon);

        RegisterClassExW(&wc);

        RECT rect;
        rect.left = 100;
        rect.right = width + rect.left;
        rect.top = 100;
        rect.bottom = height + rect.top;

        DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

        AdjustWindowRect(&rect, style, FALSE);

        // 通过WM_NCCREATE把自己发出去，通过这个方式建立实例指针和winApi之间的联系
        void* lParam = this;

        _hWnd = CreateWindowExW(
            0,
            WND_CLASS_NAME,
            name,
            style,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            rect.right - rect.left,
            rect.bottom - rect.top,
            nullptr,
            nullptr,
            _hInst,
            lParam 
            );

        ShowWindow(_hWnd, SW_SHOWDEFAULT);
    }

    Window::~Window()
    {
        DestroyWindow(_hWnd);
        UnregisterClassW(WND_CLASS_NAME, _hInst);
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
            /* Keyboard */
        case WM_KEYDOWN:
            _keyboard.onKeyPressed(static_cast<unsigned char>(wParam));
            break;
        case WM_KEYUP:
            _keyboard.onKeyReleased(static_cast<unsigned char>(wParam));
            break;
        case WM_CHAR:
            _keyboard.onCharW(static_cast<wchar_t>(wParam));
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