#pragma once

#include "ChiliWin.h"
#include "ChiliException.h"
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "DxgiInfoManager.h"

class Graphics
{
    friend class Bindable;
public:
    class Exception : public ChiliException
    {
        using ChiliException::ChiliException;
    };
    class HrException : public Exception
    {
    public:
        HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
        const char* what() const noexcept override;
        const char* GetType() const noexcept override;
        HRESULT GetErrorCode() const noexcept;
        std::string GetErrorString() const noexcept;
        std::string GetErrorDescription() const noexcept;
        std::string GetErrorInfo() const noexcept;
    private:
        HRESULT hr;
        std::string info;
    };
    class InfoException : public Exception
    {
    public:
        InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
        const char* what() const noexcept override;
        const char* GetType() const noexcept override;
        std::string GetErrorInfo() const noexcept;
    private:
        std::string info;
    };
    class DeviceRemovedException : public HrException
    {
        using HrException::HrException;
    public:
        const char* GetType() const noexcept override;
    private:
        std::string reason;
    };
public:
    Graphics(HWND hWnd);
    Graphics(const Graphics&)=delete;
    Graphics& operator=(const Graphics&) = delete;
    ~Graphics()=default;
#ifndef NDEBUG
    DxgiInfoManager infoManager;
#endif
    void EndFrame();
    void ClearBuffer(float r, float g, float b) noexcept
    {
        const float color[] = {r,g,b,1.0f};
        context->ClearRenderTargetView(renderTarget.Get(), color);
    }

    void DrawTestShape(float angle);
private:
    Microsoft::WRL::ComPtr<IDXGISwapChain>swapChain;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>context;
    Microsoft::WRL::ComPtr<ID3D11Device>device;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>renderTarget;

};

