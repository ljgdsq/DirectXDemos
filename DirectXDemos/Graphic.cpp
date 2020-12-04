#include "Graphic.h"
#include <d3dcompiler.h>
#include "dxerr.h"
#include <sstream>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")
using namespace  Microsoft::WRL;


// graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Graphics::InfoException( __LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

Graphics::Graphics(HWND hWnd)
{

    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = hWnd;
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    UINT swapCreateFlags = 0u;
#ifndef NDEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // for checking results of d3d functions
    HRESULT hr;


    GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
        nullptr,D3D_DRIVER_TYPE_HARDWARE,nullptr,0,nullptr,0,D3D11_SDK_VERSION
    ,&sd,&swapChain,&device,nullptr,&context));



    ComPtr<ID3D11Resource> backBuffer;
    swapChain->GetBuffer(0, __uuidof(ID3D11Resource),&backBuffer);

    device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTarget);
}


void Graphics::EndFrame()
{
  
    if (swapChain)
    {
        swapChain->Present(1u,0u);
    }
}

void Graphics::DrawTestShape(float angle)
{
    HRESULT hr;

    struct Vertex
    {
        struct Pos
        {
            float x;
            float y;
        }pos;
     
        struct Color
        {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
        }color;
    
    };

     Vertex vertices[] =
    {
        {0,0.5f,255,0,0,255},
        {0,0.5f,255,0,0,255},
        {0.5f,-0.5f,0,255,0,255},
        {-0.5f,-0.5f,0,0,255,255}
    };
     vertices[0].pos = { 0,0 };
     vertices[1].pos = { 0.0f,1.f };
     vertices[2].pos = { 1.f,1.f };
     vertices[3].pos = {1.f,0 };


     const unsigned short indices[] = {
        0,1,2,
       0,2,3
     };
    //vertex buffer
    ComPtr<ID3D11Buffer> buffer;
    D3D11_BUFFER_DESC desc = {};
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.CPUAccessFlags = 0u;
    desc.MiscFlags = 0u;
    desc.ByteWidth = sizeof(vertices);
    desc.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem = vertices;
    GFX_THROW_INFO(device->CreateBuffer(&desc, &data, &buffer));
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;

    context->IASetVertexBuffers(0u, 1u, buffer.GetAddressOf(), &stride, &offset);

    //index buffer
    ComPtr<ID3D11Buffer> ibuffer;
    D3D11_BUFFER_DESC idesc = {};
    idesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    idesc.Usage = D3D11_USAGE_DEFAULT;
    idesc.CPUAccessFlags = 0u;
    idesc.MiscFlags = 0u;
    idesc.ByteWidth = sizeof(indices);
    idesc.StructureByteStride = sizeof(unsigned short);

    D3D11_SUBRESOURCE_DATA idata = {};
    idata.pSysMem = indices;
    GFX_THROW_INFO(device->CreateBuffer(&idesc, &idata, &ibuffer));

    context->IASetIndexBuffer(ibuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);



    //constant buffer
    struct ConstantBuffer
    {
        struct 
        {
            float element[4][4];
        }transform;
    };
    
    auto scale = 200.0f / 300;
    const ConstantBuffer cb = {
        {
            scale*std::cos(angle),std::sin(angle),0,0,
            scale*-std::sin(angle),std::cos(angle),0,0,
           0,0,1,0,
           0,0,0,1,
            }
    };

    ComPtr<ID3D11Buffer> cbuffer;
    D3D11_BUFFER_DESC cdesc = {};
    cdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cdesc.Usage = D3D11_USAGE_DYNAMIC;
    cdesc.CPUAccessFlags =D3D11_CPU_ACCESS_WRITE;
    cdesc.MiscFlags = 0u;
    cdesc.ByteWidth = sizeof(cb);
    cdesc.StructureByteStride = 0;
    
   
    D3D11_SUBRESOURCE_DATA cdata = {};
    cdata.pSysMem = &cb;
    device->CreateBuffer(&cdesc, &cdata, &cbuffer);
    context->VSSetConstantBuffers(0u, 1u, cbuffer.GetAddressOf());

    ComPtr<ID3DBlob> blob;

    //pixel shader
    ComPtr<ID3D11PixelShader> pixelShader;
    GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &blob));
    GFX_THROW_INFO(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader));
    context->PSSetShader(pixelShader.Get(), nullptr, 0u);
   
    //vertex shader
    ComPtr<ID3D11VertexShader> vertexShader;
    GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &blob));


    GFX_THROW_INFO(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader));
    context->VSSetShader(vertexShader.Get(), nullptr, 0u);


    ComPtr<ID3D11InputLayout> inputLayout;
   const D3D11_INPUT_ELEMENT_DESC inputdesc[] = {
        {"Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0, D3D11_INPUT_PER_VERTEX_DATA,0},
        {"Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,8u, D3D11_INPUT_PER_VERTEX_DATA,0}
    };

    GFX_THROW_INFO (device->CreateInputLayout(inputdesc, std::size(inputdesc), blob->GetBufferPointer(),
        blob->GetBufferSize(), &inputLayout));
    context->IASetInputLayout(inputLayout.Get());

    // bind render target
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->OMSetRenderTargets(1u, renderTarget.GetAddressOf(), nullptr);

    D3D11_VIEWPORT vp;
    vp.Width = 300;
    vp.Height = 200;
    vp.MinDepth = 0;
    vp.MaxDepth = 1; 
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;

    context->RSSetViewports(1u, &vp);
    //GFX_THROW_INFO_ONLY(context->Draw(std::size(vertices), 0u));
    GFX_THROW_INFO_ONLY(context->DrawIndexed(std::size(indices),0u,0u));

}


// Graphics exception stuff
Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
    :
    Exception(line, file),
    hr(hr)
{
    // join all info messages with newlines into single string
    for (const auto& m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }
    // remove final newline if exists
    if (!info.empty())
    {
        info.pop_back();
    }
}

const char* Graphics::HrException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
        << "[Error String] " << GetErrorString() << std::endl
        << "[Description] " << GetErrorDescription() << std::endl;
    if (!info.empty())
    {
        oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    }
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
    return "Chili Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
    return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
    return DXGetErrorString(hr);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
    char buf[512];
    DXGetErrorDescription(hr, buf, sizeof(buf));
    return buf;
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
    return info;
}


const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
    return "Chili Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}


Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
    :
    Exception(line, file)
{
    // join all info messages with newlines into single string
    for (const auto& m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }
    // remove final newline if exists
    if (!info.empty())
    {
        info.pop_back();
    }
}


const char* Graphics::InfoException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
    return "Chili Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
    return info;
}