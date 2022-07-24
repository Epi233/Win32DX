#include "InputLayout.h"
#include "../../../Exception/EngineException.h"

D3D11::InputLayout::InputLayout(const GraphicD3D11& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
                                ID3DBlob* pVertexShaderByte)
{
    auto hr = gfx.getDevice()->CreateInputLayout(
        layout.data(),
        static_cast<UINT>(layout.size()),
        pVertexShaderByte->GetBufferPointer(),
        pVertexShaderByte->GetBufferSize(),
        _pInputLayout.GetAddressOf()
        );

    if (FAILED(hr))
        throw HResultException(__LINE__, __FILE__, hr);
}

void D3D11::InputLayout::bind(GraphicD3D11& gfx)
{
    gfx.getContext()->IASetInputLayout(_pInputLayout.Get());
}
