#pragma once

#include "Bindable.h"
#include <vector>

namespace D3D11
{
    class InputLayout : public Bindable 
    {
    public:
        InputLayout(const GraphicD3D11& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderByte);

    public:
        void bind(GraphicD3D11& gfx) override;

    protected:
        Microsoft::WRL::ComPtr<ID3D11InputLayout> _pInputLayout;
    };

}
