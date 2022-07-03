#pragma once

#include "../DefinwWindows.h"
#include "../Exception/BaseException.h"

namespace Win32UI
{
    class Window
    {
    public:
        Window(int width, int height, const wchar_t* name) noexcept;
        ~Window();
        Window(const Window&) = delete;
        Window& operator= (const Window&) = delete;

    private:
        static LRESULT CALLBACK HandleMsgSetup(HWND, UINT, WPARAM, LPARAM);
        static LRESULT CALLBACK HandleMsgReally(HWND, UINT, WPARAM, LPARAM);
        LRESULT HandleMsg(HWND, UINT, WPARAM, LPARAM) noexcept;

    private:
        int _width;
        int _height;
        HWND _hWnd;
        
    private:
        class InternalWindow
        {
        public:
            static LPCWSTR getWindowName() noexcept;
            static HINSTANCE getInstance() noexcept;
        private:
            InternalWindow();
            ~InternalWindow();
            InternalWindow(const InternalWindow&) = delete;
            InternalWindow& operator= (const InternalWindow&) = delete;
        private:
            static constexpr LPCWSTR WND_NAME = L"DX11 Engine";
            static InternalWindow _instance;
            HINSTANCE _hInst;
        };

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
}


