#pragma once

#include "GraphicsThrowMacros.h"

template<typename C>
class ConstantBuffer:public Bindable
{

public:
    void Update(Graphics& gfx, const C& consts)
    {
        INFOMAN(gfx);
        D3D11_MAPPED_SUBRESOURCE msr;
        GetContext(gfx)->Map(constantBuffer,0u, D3D11_MAP_WRITE_DISCARD,0u
        , &msr)

            GetContext(gfx)->Unmap(constantBuffer.Get(), 0u);
    }


protected:
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
};

