#include "RenderStates.h"

CComPtr<ID3D11DepthStencilState> RenderStates::_noDoubleBlendDSS;
CComPtr<ID3D11DepthStencilState> RenderStates::_noDepthWrites;
CComPtr<ID3D11BlendState> RenderStates::_transparentBlendState;
CComPtr<ID3D11BlendState> RenderStates::_additiveBlending;
CComPtr<ID3D11SamplerState> RenderStates::_sampleStateLinear;

HRESULT RenderStates::init(ID3D11Device* const device)
{
    HRESULT hr = 0;

    D3D11_DEPTH_STENCIL_DESC noDoubleBlendDesc;
    noDoubleBlendDesc.DepthEnable = true;
    noDoubleBlendDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    noDoubleBlendDesc.DepthFunc = D3D11_COMPARISON_LESS;
    noDoubleBlendDesc.StencilEnable = true;
    noDoubleBlendDesc.StencilReadMask = 0xff;
    noDoubleBlendDesc.StencilWriteMask = 0xff;

    noDoubleBlendDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    noDoubleBlendDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    noDoubleBlendDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
    noDoubleBlendDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

    noDoubleBlendDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    noDoubleBlendDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    noDoubleBlendDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
    noDoubleBlendDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

    hr = (device->CreateDepthStencilState(&noDoubleBlendDesc, &_noDoubleBlendDSS));
    if (FAILED(hr)) return hr;


    D3D11_BLEND_DESC transparentblendDesc;
    transparentblendDesc.AlphaToCoverageEnable = false;
    transparentblendDesc.IndependentBlendEnable = false;

    transparentblendDesc.RenderTarget[0].BlendEnable = true;
    transparentblendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    transparentblendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    transparentblendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    transparentblendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    transparentblendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    transparentblendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    transparentblendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    hr = device->CreateBlendState(&transparentblendDesc, &_transparentBlendState);
    if (FAILED(hr)) return hr;


    D3D11_SAMPLER_DESC SamplerDesc = {};
    SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    hr = device->CreateSamplerState(&SamplerDesc, &_sampleStateLinear);
    if (FAILED(hr)) return hr;

    D3D11_BLEND_DESC AdditiveBlendingDesc = {};
    AdditiveBlendingDesc.RenderTarget[0].BlendEnable = true;
    AdditiveBlendingDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    AdditiveBlendingDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    AdditiveBlendingDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    AdditiveBlendingDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    AdditiveBlendingDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    AdditiveBlendingDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    AdditiveBlendingDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    hr = device->CreateBlendState(&AdditiveBlendingDesc, &_additiveBlending);
    if (FAILED(hr)) return hr;
    
    D3D11_DEPTH_STENCIL_DESC DepthStencilDesc = {};
    DepthStencilDesc.DepthEnable = true;
    DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    hr = device->CreateDepthStencilState(&DepthStencilDesc, &_noDepthWrites);
    if (FAILED(hr)) return hr;

    return 0;
}
