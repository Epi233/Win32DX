#pragma once

#include "../../DefinwWindows.h"
#include <d3d11.h>
#include <wrl.h>
#include "DxgiInfoBridge.h"

class GraphicD3D11
{
public:
    GraphicD3D11(HWND targetWnd);
    ~GraphicD3D11();
    GraphicD3D11(const GraphicD3D11&) = delete;
    GraphicD3D11& operator= (const GraphicD3D11&) = delete;

public:
    void swapBuffer();
    void test();
    Microsoft::WRL::ComPtr<ID3DBlob> compileShader(const std::wstring& filename, const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target);

private:
    // shit pointer
    Microsoft::WRL::ComPtr<ID3D11Device> _pDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> _pContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> _pSwapChain;

public:
    Microsoft::WRL::ComPtr<ID3D11Device> getDevice() const;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> getContext() const;
    Microsoft::WRL::ComPtr<IDXGISwapChain> getSwapChain() const;

private:
#if _DEBUG
    DxgiInfoBridge _dxgiDebugInfoBridge;
#endif

private:
    void dxgiDebugInfoSetLogPosition();
    std::vector<std::wstring> dxgiDebugInfoGetLog() const;
};
