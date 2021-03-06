#include <d3dcompiler.h>
#include "GraphicD3D11.h"
#include "../../Exception/EngineException.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

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
    
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        swapCreateFlags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        _pSwapChain.GetAddressOf(),
        _pDevice.GetAddressOf(),
        nullptr,
        _pContext.GetAddressOf()
        );

    if (FAILED(hr))
        throw HResultException(__LINE__, __FILE__, hr);
}

GraphicD3D11::~GraphicD3D11()
{
}

void GraphicD3D11::swapBuffer()
{
    HRESULT hr = _pSwapChain->Present(1, 0);
    
    if (FAILED(hr))
    {
        throw HResultException(__LINE__, __FILE__, hr); 
    }
}

void GraphicD3D11::test()
{
    struct Vertex
    {
        float x;
        float y;
    };

    // 一个三角形的顶点
    const Vertex vertices[] =
    {
        {0.0f, 0.5f},
        {0.5f, -0.5f},
        {-0.5f, -0.5f}
    };

    // 缓冲区描述
    D3D11_BUFFER_DESC bufferDesc {};
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.CPUAccessFlags = 0u;
    bufferDesc.MiscFlags = 0u;
    bufferDesc.ByteWidth = sizeof(vertices);    // 带宽
    bufferDesc.StructureByteStride = sizeof(Vertex);    // 步长

    // 数据描述
    D3D11_SUBRESOURCE_DATA subResData {};
    subResData.pSysMem = vertices;

    // 创建数据缓冲区
    Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
    {
        auto hCreateBuffer = _pDevice->CreateBuffer(&bufferDesc, &subResData, pVertexBuffer.GetAddressOf());
        if (FAILED(hCreateBuffer))
            throw HResultException(__LINE__, __FILE__, hCreateBuffer);
    }

    // 设置顶点缓冲
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    _pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

    // Vertex Shader
    Microsoft::WRL::ComPtr<ID3DBlob> pVertexShaderBlob = compileShader(
        L"F:\\Github\\Win32DX11\\Win32DX11\\Win32Engine\\src\\Shader\\vertex.hlsl",
        nullptr, "VS", "vs_5_0");

    Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
    _pDevice->CreateVertexShader(pVertexShaderBlob->GetBufferPointer(), pVertexShaderBlob->GetBufferSize(), nullptr, &pVertexShader);
    _pContext->VSSetShader(pVertexShader.Get(), nullptr, 0);
    

    _pContext->Draw(static_cast<UINT>(std::size(vertices)), 0u);
}

Microsoft::WRL::ComPtr<ID3DBlob> GraphicD3D11::compileShader(const std::wstring& filename, const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target)
{
    UINT compileFlags = 0;
#if defined(_DEBUG) 
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    Microsoft::WRL::ComPtr<ID3DBlob> byteCode = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> errors;

    auto hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

    if (FAILED(hr))
        throw HResultException(__LINE__, __FILE__, hr);

    if (errors != nullptr)
        OutputDebugString(static_cast<wchar_t*>(errors->GetBufferPointer()));

    return byteCode;
}

Microsoft::WRL::ComPtr<ID3D11Device> GraphicD3D11::getDevice() const
{
    return _pDevice;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> GraphicD3D11::getContext() const
{
    return _pContext;
}

Microsoft::WRL::ComPtr<IDXGISwapChain> GraphicD3D11::getSwapChain() const
{
    return _pSwapChain;
}