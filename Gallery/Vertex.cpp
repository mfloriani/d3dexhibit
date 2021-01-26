#include "Vertex.h"

const std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc::Basic {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

const std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc::Particle {
	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

CComPtr<ID3D11InputLayout> InputLayouts::_basic = nullptr;
CComPtr<ID3D11InputLayout> InputLayouts::_particle = nullptr;

HRESULT InputLayouts::createBasic(ID3D11Device* const device, std::vector<unsigned char>& byteCode)
{


	const auto numElements = InputLayoutDesc::Basic.size();
	const HRESULT hr = device->CreateInputLayout(InputLayoutDesc::Basic.data(), numElements, &byteCode[0], byteCode.size(), &InputLayouts::_basic);
	if (FAILED(hr)) return hr;

	return 0;
}

HRESULT InputLayouts::createParticle(ID3D11Device* const device, std::vector<unsigned char>& byteCode)
{
	const auto numElements = InputLayoutDesc::Particle.size();
	const HRESULT hr = device->CreateInputLayout(InputLayoutDesc::Particle.data(), numElements, &byteCode[0], byteCode.size(), &InputLayouts::_particle);
	if (FAILED(hr)) return hr;

	return 0;
}
