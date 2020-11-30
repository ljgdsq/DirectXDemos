#pragma once
#include <Windows.h>
#include <d3d11.h>

#pragma comment(lib,"d3d11.lib")
class Graphics
{
public:
    Graphics(HWND hwnd);
    Graphics(const Graphics&)=delete;
    Graphics& operator=(const Graphics&)=delete;
    ~Graphics();

    void EndFrame();
private:
    ID3D11Device* pDevice = nullptr;
    IDXGISwapChain* pSwapChain = nullptr;
    ID3D11DeviceContext* pContext = nullptr;
};

