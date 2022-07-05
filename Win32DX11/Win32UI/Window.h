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
        static constexpr LPCWSTR WND_CLASS_NAME = L"DX11 Engine";

    private:
        int _width;
        int _height;
        HINSTANCE _hInst;
        HWND _hWnd;

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


