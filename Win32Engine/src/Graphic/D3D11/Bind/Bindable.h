#pragma once

#include "../GraphicD3D11.h"

namespace D3D11
{
    class Bindable
    {
    public:
        virtual ~Bindable() = default;
        
    public:
        virtual void bind(GraphicD3D11& gfx) = 0;
       
    };

}

