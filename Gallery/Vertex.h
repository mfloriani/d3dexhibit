#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <cstdint>
#include <atlbase.h>

struct Vertex
{
	Vertex() = default;
	Vertex(
		const DirectX::XMFLOAT3& p,
		const DirectX::XMFLOAT3& n,
		const DirectX::XMFLOAT3& t,
		const DirectX::XMFLOAT2& uv
	) :
		Position(p),
		Normal(n),
		TangentU(t),
		TexCoord(uv) {}
	Vertex(
		float px, float py, float pz,
		float nx, float ny, float nz,
		float tx, float ty, float tz,
		float u, float v
	) :
		Position(px, py, pz),
		Normal(nx, ny, nz),
		TangentU(tx, ty, tz),
		TexCoord(u, v) {}

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT3 TangentU;
	DirectX::XMFLOAT2 TexCoord;
};

struct ParticleVertex
{
	ParticleVertex() = default;
	explicit ParticleVertex(const DirectX::XMFLOAT4& c) : Color(c) {}

	DirectX::XMFLOAT4 Color;
};

struct InputLayoutDesc
{
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> Basic;
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> Particle;
};

class InputLayouts
{
public:
	InputLayouts() = default;

	static HRESULT createBasic(ID3D11Device* device, std::vector<unsigned char>& byteCode);
	static HRESULT createParticle(ID3D11Device* device, std::vector<unsigned char>& byteCode);

	static ID3D11InputLayout* basic() { return _basic; };
	static ID3D11InputLayout* particle() { return _particle; };

private:
	static CComPtr<ID3D11InputLayout> _basic;
	static CComPtr<ID3D11InputLayout> _particle;
};