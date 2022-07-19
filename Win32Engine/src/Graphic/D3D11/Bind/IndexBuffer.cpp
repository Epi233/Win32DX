#include "IndexBuffer.h"

D3D11::IndexBuffer::IndexBuffer(const GraphicD3D11& gfx, const std::vector<unsigned short>& indices)
    : _count(static_cast<UINT>(indices.size()))
{
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.CPUAccessFlags = 0u;
    bufferDesc.MiscFlags = 0u;
    bufferDesc.ByteWidth = static_cast<UINT>(_count * sizeof(unsigned short));
    bufferDesc.StructureByteStride = sizeof( unsigned short );

    D3D11_SUBRESOURCE_DATA subResData = {};
    subResData.pSysMem = indices.data();

    gfx.getDevice()->CreateBuffer(&bufferDesc, &subResData, _pIndexBuffer.GetAddressOf());
}
