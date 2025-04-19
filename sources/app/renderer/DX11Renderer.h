#ifndef DX11RENDERER_H
#define DX11RENDERER_H
#include "GlobalsData.h"
#if PLATFORMS_WINDOWS
/////////////
// LINKING //
/////////////
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <d3d11.h>
#include <d3d10.h>
#include <directxmath.h>
struct DX11RendererProprerties
{
    IDXGISwapChain* m_swapChain = NULL;
    ID3D11Device* m_device = NULL;
    ID3D11DeviceContext* m_deviceContext = NULL;
    ID3D11RenderTargetView* m_renderTargetView = NULL;
    ID3D11Texture2D* m_depthStencilBuffer = NULL;
    ID3D11DepthStencilState* m_depthStencilState = NULL;
    ID3D11DepthStencilView* m_depthStencilView = NULL;
    ID3D11RasterizerState* m_rasterState = NULL;
    DirectX::XMMATRIX m_projectionMatrix  = {};
    DirectX::XMMATRIX m_worldMatrix = {};
    DirectX::XMMATRIX m_orthoMatrix = {};
    D3D11_VIEWPORT m_viewport = {};
};


inline void DX11Renderer_Init()
{
    HRESULT result;
    IDXGIFactory* factory;
    IDXGIAdapter* adapter;
    IDXGIOutput* adapterOutput;
    unsigned int numModes;
    unsigned int i;
    unsigned int numerator;
    unsigned int denominor;

    unsigned long long stringLength;
    DXGI_MODE_DESC* displayModeList;
    DXGI_ADAPTER_DESC adapterDesc;

    int error;

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    
}

inline void DX11Renderer_Clean()
{

}

#endif
#endif