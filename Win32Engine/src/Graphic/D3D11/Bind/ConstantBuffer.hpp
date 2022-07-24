#pragma once

#include "Bindable.h"
#include "../../../Exception/EngineException.h"

namespace D3D11
{
    template<typename T>
    class ConstantBuffer : public Bindable
    {
    public:
        ConstantBuffer(const GraphicD3D11& gfx, const T& constant)
        {
            D3D11_BUFFER_DESC bufferDesc;
            bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            bufferDesc.MiscFlags = 0u;
            bufferDesc.ByteWidth = static_cast<UINT>(sizeof(constant));
            bufferDesc.StructureByteStride = 0;

            D3D11_SUBRESOURCE_DATA subResData = {};
            subResData.pSysMem = &constant;

            gfx.getDevice()->CreateBuffer(&bufferDesc, &subResData, _pConstantBuffer.GetAddressOf());
        }

    public:
        void updateConstantValue(const GraphicD3D11& gfx, const T& constant)
        {
            D3D11_MAPPED_SUBRESOURCE mappedSubRes;
            auto hr = gfx.getContext()->Map(_pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubRes);
            if (FAILED(hr))
                throw HResultException(__LINE__, __FILE__, hr);

            memccpy(mappedSubRes.pData, &constant, sizeof(constant));

            gfx.getContext()->Unmap(_pConstantBuffer.Get(), 0u);
        }

    protected:
        Microsoft::WRL::ComPtr<ID3D11Buffer> _pConstantBuffer;
    };

    template<typename T>
    class VertexConstantBuffer : public ConstantBuffer<T>
    {
    public:
        VertexConstantBuffer(const GraphicD3D11& gfx, const T& constant)
            : ConstantBuffer<T>(gfx, constant)
        {
        }
        
        void bind(GraphicD3D11& gfx) override
        {
            gfx.getContext()->VSSetConstantBuffers(0u, 1u, ConstantBuffer<T>::_pConstantBuffer.GetAddressOf());
        }
    };

    template<typename T>
    class PixelConstantBuffer : public ConstantBuffer<T>
    {
    public:
        PixelConstantBuffer(const GraphicD3D11& gfx, const T& constant)
            : ConstantBuffer<T>(gfx, constant)
        {
        }

        void bind(GraphicD3D11& gfx) override
        {
            gfx.getContext()->PSSetConstantBuffers(0u, 1u, ConstantBuffer<T>::_pConstantBuffer.GetAddressOf());
        }
    };
    
}

