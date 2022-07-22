#pragma once

#include <vector>

#include "Bindable.h"

namespace D3D11
{
    class IndexBuffer : public Bindable
    {
    public:
        IndexBuffer(const GraphicD3D11& gfx, const std::vector<unsigned short>& indices);

    public:
        void bind(GraphicD3D11& gfx) override;
        UINT getCount() const;

    protected:
        UINT _count;
        Microsoft::WRL::ComPtr<ID3D11Buffer> _pIndexBuffer;
        
    };
}


