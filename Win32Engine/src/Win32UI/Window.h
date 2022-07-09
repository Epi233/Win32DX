#pragma once

#include <optional>
#include <memory>

#include "../DefinwWindows.h"
#include "../Exception/EngineException.h"
#include "../Input/Keyboard.h"
#include "../Input/Mouse.h"
#include "../Graphic/D3D11/GraphicD3D11.h"

class Window
{
public:
    Window(int width, int height, const wchar_t* name);
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

private:
    std::unique_ptr<GraphicD3D11> _pGraphicD3D11;

public:
    void graphicSwapChain() const;
    
};

