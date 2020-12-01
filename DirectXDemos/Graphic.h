#pragma once

#include "ChiliWin.h"
#include <d3d11.h>

class Graphic
{
public:
    Graphic(HWND hWnd);
    Graphic(const Graphic&)=delete;
    Graphic& operator=(const Graphic&) = delete;
    ~Graphic();

    void EndFrame();
    void ClearBuffer(float r, float g, float b) noexcept
    {
        const float color[] = {r,g,b,1.0f};
        context->ClearRenderTargetView(renderTarget, color);
    }
private:
    IDXGISwapChain* swapChain=nullptr;
    ID3D11DeviceContext *context=nullptr;
    ID3D11Device* device=nullptr;
    ID3D11RenderTargetView* renderTarget;
};

