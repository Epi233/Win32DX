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

private:
    // shit pointer
    Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;

private:
#if _DEBUG
    DxgiInfoBridge _dxgiDebugInfoBridge;
#endif

private:
    void dxgiDebugInfoSetLogPosition();
    std::vector<std::wstring> dxgiDebugInfoGetLog() const;
};
