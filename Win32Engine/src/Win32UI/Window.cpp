#include "Window.h"
#include "../../resource.h"

Window::Window(int width, int height, const wchar_t* name)
        : _width(width)
        , _height(height)
        , _hInst(GetModuleHandle(nullptr))
{
    // icon
    HANDLE hIcon = LoadImage(_hInst, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 64, 64, 0);

    WNDCLASSEX wc;

    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = handleMsgSetup;
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

    _pGraphicD3D11 = std::make_unique<GraphicD3D11>(_hWnd);
}

Window::~Window()
{
    DestroyWindow(_hWnd);
    UnregisterClassW(WND_CLASS_NAME, _hInst);
}

std::optional<int> Window::processMessage()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return static_cast<int>(msg.wParam);

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return {};
}

LRESULT Window::handleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // 首次创建窗口时，在WM_CREATE消息之前发送，LPARAM指向 CREATESTRUCT 结构的指针，其中包含有关正在创建的窗口的信息
    if (msg == WM_NCCREATE)
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

        // 把窗口实例的指针存到winApi一侧，这样只要通过hWnd，总能拿到Window*
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

        // 把处理函数强制调整为第二个
        SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::handleMsgReally));
        
        return pWnd->handleMsg(hWnd, msg, wParam, lParam);
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::handleMsgReally(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
    return pWnd->handleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (msg)
    {
    case WM_CLOSE:
        return onMsgWmClose(hWnd, msg, wParam, lParam);
    case WM_KILLFOCUS:
        return onMsgWmKillFocus(hWnd, msg, wParam, lParam);
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN: // alt等系统按键
        return onMsgWmKeyDownAndSysKeyDown(hWnd, msg, wParam, lParam);
    case WM_KEYUP:
    case WM_SYSKEYUP:
        return onMsgWmKeyUpAndSysKeyUp(hWnd, msg, wParam, lParam);
    case WM_CHAR:
        return onMsgWmChar(hWnd, msg, wParam, lParam);
    case WM_MOUSEMOVE:
        return onMsgWmMouseMove(hWnd, msg, wParam, lParam);
    case WM_LBUTTONDOWN:
        return onMsgWmLButtonDown(hWnd, msg, wParam, lParam);
    case WM_LBUTTONUP:
        return onMsgWmLButtonUp(hWnd, msg, wParam, lParam);
    case WM_MBUTTONDOWN:
        return onMsgWmMButtonDown(hWnd, msg, wParam, lParam);
    case WM_MBUTTONUP:
        return onMsgWmMButtonUp(hWnd, msg, wParam, lParam);
    case WM_RBUTTONDOWN:
        return onMsgWmRButtonDown(hWnd, msg, wParam, lParam);
    case WM_RBUTTONUP:
        return onMsgWmRButtonUp(hWnd, msg, wParam, lParam);
    case WM_MOUSEWHEEL:
        return onMsgWmMouseWheel(hWnd, msg, wParam, lParam);
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

const Keyboard* Window::getKeyboard() const
{
    return &_keyboard;
}

const Mouse* Window::getMouse() const
{
    return &_mouse;
}

LRESULT Window::onMsgWmClose(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PostQuitMessage(0);
    return 0;
}

LRESULT Window::onMsgWmKillFocus(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // 如果按下引起了其他窗口，那么抬起发不到主窗口，状态会残留
    _keyboard.clearState();

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::onMsgWmKeyDownAndSysKeyDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // 0x40000000为高的时候，为第一次按下，后续都是0
    if (!(lParam & 0x40000000) || _keyboard.isAutoRepeatEnabled())
        _keyboard.onKeyPressed(static_cast<unsigned char>(wParam));

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::onMsgWmKeyUpAndSysKeyUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    _keyboard.onKeyReleased(static_cast<unsigned char>(wParam));

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::onMsgWmChar(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    _keyboard.onCharW(static_cast<wchar_t>(wParam));

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::onMsgWmMouseMove(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);

    if (pt.x >= 0 && pt.x < _width && pt.y > 0 && pt.y < _height)
    {
        _mouse.onMouseMove(pt.x, pt.y);
        if (!_mouse.isInWindow())
        {
            SetCapture(_hWnd);
            _mouse.onMouseEnter();
        }
    }
    else
    {
        // 按住的时候离开窗口，不许离开
        if (wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON))
        {
            _mouse.onMouseMove(pt.x, pt.y);
        }
        else
        {
            ReleaseCapture();
            _mouse.onMouseLeave();
        }
    }
   
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::onMsgWmLButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.onLeftMousePressed(pt.x, pt.y);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::onMsgWmLButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.onLeftMouseReleased(pt.x, pt.y);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::onMsgWmMButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.onMiddleMousePressed(pt.x, pt.y);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::onMsgWmMButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.onMiddleMouseReleased(pt.x, pt.y);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::onMsgWmRButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.onRightMousePressed(pt.x, pt.y);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::onMsgWmRButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.onRightMouseReleased(pt.x, pt.y);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::onMsgWmMouseWheel(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.OnWheelDelta(pt.x, pt.y, GET_WHEEL_DELTA_WPARAM(wParam));
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Window::graphicSwapChain() const
{
    _pGraphicD3D11->swapBuffer();
}