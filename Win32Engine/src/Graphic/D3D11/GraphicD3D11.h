#pragma once

#include "../../DefinwWindows.h"
#include <d3d11.h>
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
    ID3D11Device* pDevice = nullptr;
    ID3D11DeviceContext* pContext = nullptr;

    IDXGISwapChain* pSwapChain = nullptr;

private:
#if _DEBUG
    DxgiInfoBridge _dxgiDebugInfoBridge;
#endif
};
