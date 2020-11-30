#include "Graphics.h"

Graphics::Graphics(HWND hwnd)
{
}

Graphics::~Graphics()
{
    if (pContext)
    {
        pContext->Release();
    }

    if (pSwapChain)
    {
        pSwapChain->Release();
    }


    if (pDevice)
    {
        pDevice->Release();
    }
}

void Graphics::EndFrame()
{
    pSwapChain->Present(1u, 0u);
}
