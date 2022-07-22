#pragma once

#include <vector>

#include "Bindable.h"

namespace D3D11
{
    class VertexBuffer : public Bindable
    {
    public:
        template<class Cell>
        VertexBuffer(const GraphicD3D11& gfx, const std::vector<Cell>& vertices)
            : _stride(sizeof(Cell))
        {
            // offset always 0 for now
            _offset = 0;
            
            D3D11_BUFFER_DESC bufferDesc;
            bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bufferDesc.Usage = D3D11_USAGE_DEFAULT;
            bufferDesc.CPUAccessFlags = 0u;
            bufferDesc.MiscFlags = 0u;
            bufferDesc.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(Cell));
            bufferDesc.StructureByteStride = sizeof(Cell);

            D3D11_SUBRESOURCE_DATA subResData = {};
            subResData.pSysMem = vertices.data();

            gfx.getDevice()->CreateBuffer(&bufferDesc, &subResData, _pVertexBuffer.GetAddressOf());
        }

        void bind(GraphicD3D11& gfx) override;

    private:
        UINT _offset;
        UINT _stride;
        Microsoft::WRL::ComPtr<ID3D11Buffer> _pVertexBuffer;
    };
}

