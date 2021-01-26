#pragma once

#include <d3d11.h>
#include <atlbase.h>

class RenderStates
{
public:
	RenderStates() = default;

	static HRESULT init(ID3D11Device* device);

	static ID3D11DepthStencilState* noDoubleBlendDSS() { return _noDoubleBlendDSS; }
	static ID3D11DepthStencilState* noDepthWrites() { return _noDepthWrites; }
	static ID3D11BlendState* transparentBlendState() { return _transparentBlendState; }
	static ID3D11BlendState* additiveBlending() { return _additiveBlending; }
	static ID3D11SamplerState* sampleStateLinear() { return _sampleStateLinear; }

private:
	static CComPtr<ID3D11DepthStencilState> _noDoubleBlendDSS;
	static CComPtr<ID3D11DepthStencilState> _noDepthWrites;
	static CComPtr<ID3D11BlendState>		_transparentBlendState;
	static CComPtr<ID3D11BlendState>		_additiveBlending;
	static CComPtr<ID3D11SamplerState>      _sampleStateLinear;
};