#pragma once

#include <optional>

#include "../DefinwWindows.h"
#include "../Exception/BaseException.h"
#include "../Input/Keyboard.h"
#include "../Input/Mouse.h"

class Window
{
public:
    Window(int width, int height, const wchar_t* name) noexcept;
    ~Window();
    Window(const Window&) = delete;
    Window& operator= (const Window&) = delete;

public:
    static std::optional<int> processMessage();

private:
    static LRESULT CALLBACK handleMsgSetup(HWND, UINT, WPARAM, LPARAM);
    static LRESULT CALLBACK handleMsgReally(HWND, UINT, WPARAM, LPARAM);
    LRESULT handleMsg(HWND, UINT, WPARAM, LPARAM) noexcept;

private:
    static constexpr LPCWSTR WND_CLASS_NAME = L"DX11 Engine";

private:
    int _width;
    int _height;
    HINSTANCE _hInst;
    HWND _hWnd;

private:
    Keyboard _keyboard;
    Mouse _mouse;

public:
    const Keyboard* getKeyboard() const;
    const Mouse* getMouse() const;

private:
    /* System */
    LRESULT onMsgWmClose(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT onMsgWmKillFocus(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    /* Input - Keyboard */
    LRESULT onMsgWmKeyDownAndSysKeyDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT onMsgWmKeyUpAndSysKeyUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT onMsgWmChar(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    /* Input - Mouse */
    LRESULT onMsgWmMouseMove(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT onMsgWmLButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT onMsgWmLButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT onMsgWmMButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT onMsgWmMButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT onMsgWmRButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT onMsgWmRButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT onMsgWmMouseWheel(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
    class Exception : public BaseException
    {
    public:
        Exception(int line, const char* file, HRESULT hResult);
        const wchar_t* whatW() const noexcept override;
        virtual const char* getType() const noexcept override;

    public:
        static std::wstring hResultToString(HRESULT hResult) noexcept;
        HRESULT getHResult() const noexcept;
        std::wstring getExceptionString() const noexcept;
        
    private:
        HRESULT _hResult;
    };
};

