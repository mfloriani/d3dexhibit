#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <atlbase.h>
#include "Constants.h"

struct Particle
{
	DirectX::XMFLOAT4 startPos;
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 velocity;
	float             startTime;
	float             time;
};

struct CBuffer_CS
{
	UINT  param[4];
	float paramf[4];
};

struct CBuffer_GS
{
	DirectX::XMFLOAT4X4 WorldViewProj;
	DirectX::XMFLOAT4X4 InvView;
};

class ParticleSystem
{
public:
	ParticleSystem(
		ID3D11ComputeShader* const cs,
		ID3D11VertexShader* const vs,
		ID3D11GeometryShader* const gs,
		ID3D11PixelShader* const ps);
	~ParticleSystem();

	ParticleSystem(const ParticleSystem&) = delete;
	ParticleSystem& operator=(const ParticleSystem&) = delete;

	HRESULT init(ID3D11Device* device);
	void update(ID3D11DeviceContext* dc, float dt);
	void render(ID3D11DeviceContext* const dc, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj) const;

private:	
	CComPtr<ID3D11Buffer>              _vertexBuffer;	
	CComPtr<ID3D11Buffer>              _cbufferCS;
	CComPtr<ID3D11Buffer>              _cbufferGS;
	CComPtr<ID3D11Buffer>              _bufferA;
	CComPtr<ID3D11Buffer>              _bufferB;
	CComPtr<ID3D11ShaderResourceView>  _bufferA_SRV;
	CComPtr<ID3D11ShaderResourceView>  _bufferB_SRV;
	CComPtr<ID3D11UnorderedAccessView> _bufferA_UAV;
	CComPtr<ID3D11UnorderedAccessView> _bufferB_UAV;
	CComPtr<ID3D11ShaderResourceView>  _texture_SRV;

	ID3D11ComputeShader * const        _cs;
	ID3D11VertexShader * const         _vs;
	ID3D11GeometryShader * const       _gs;
	ID3D11PixelShader * const	       _ps;

};