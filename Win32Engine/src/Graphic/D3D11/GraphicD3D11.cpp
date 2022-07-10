﻿#include "GraphicD3D11.h"
#include "../../Exception/EngineException.h"

#pragma comment(lib, "d3d11.lib")

GraphicD3D11::GraphicD3D11(HWND targetWnd)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

    swapChainDesc.BufferDesc.Width = 0;
    swapChainDesc.BufferDesc.Height = 0;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = targetWnd;
    swapChainDesc.Windowed = true;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;

    UINT swapCreateFlags = 0u;
#if _DEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

#if _DEBUG
    _dxgiDebugInfoBridge.setLogPosition();
#endif
    
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        swapCreateFlags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &pSwapChain,
        &pDevice,
        nullptr,
        &pContext
        );


    if (FAILED(hr))
    {
#if _DEBUG
        throw HResultException(__LINE__, __FILE__, hr, _dxgiDebugInfoBridge.getLog());
#else
        throw HResultException(__LINE__, __FILE__, hr);
#endif
    }
}

GraphicD3D11::~GraphicD3D11()
{
    if (pSwapChain != nullptr)
        pSwapChain->Release();
    if (pContext != nullptr)
        pContext->Release();
    if (pDevice != nullptr)
        pDevice->Release();
}

void GraphicD3D11::swapBuffer()
{
#if _DEBUG
    _dxgiDebugInfoBridge.setLogPosition();
#endif
    
    HRESULT hr = pSwapChain->Present(1, 0);
    
    if (FAILED(hr))
    {
        // 这个异常有额外的信息可以显示
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw HResultException(__LINE__, __FILE__, pDevice->GetDeviceRemovedReason());
        }
        else
        {
            if (FAILED(hr))
            {
#if _DEBUG
                throw HResultException(__LINE__, __FILE__, hr, _dxgiDebugInfoBridge.getLog());
#else
                throw HResultException(__LINE__, __FILE__, hr);
#endif
            }
        }
    }
}
