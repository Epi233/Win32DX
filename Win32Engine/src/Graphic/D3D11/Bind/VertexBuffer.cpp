#include "VertexBuffer.h"

void D3D11::VertexBuffer::bind(GraphicD3D11& gfx)
{
    gfx.getContext()->IASetVertexBuffers(
        0u,
        1u,
        _pVertexBuffer.GetAddressOf(),
        &_stride,
        &_offset);
}
