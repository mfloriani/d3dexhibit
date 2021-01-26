#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <atlbase.h>
#include <vector>

#include "GameObject.h"
#include "Light.h"

struct ShadowMapBuffer
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX lightProj;
	DirectX::XMFLOAT3 lightPos;
};

class ShadowMap
{
public:
	ShadowMap(DirectionalLight& light, ID3D11VertexShader* const vs, ID3D11PixelShader* const ps);
	~ShadowMap();
	ShadowMap(const ShadowMap&) = delete;
	ShadowMap& operator=(const ShadowMap& other) = delete;

	HRESULT init(ID3D11Device* device);
	void render(ID3D11DeviceContext* dc);

	inline ID3D11DepthStencilView* dsv() const { return _depthStencilView.p; }
	inline ID3D11ShaderResourceView* srv() const { return _shaderResourceView.p; }
	inline ID3D11VertexShader* vs() const { return _vertexShader; }
	inline ID3D11PixelShader* ps() const { return _pixelShader; }
	inline const D3D11_VIEWPORT& viewport() const { return _viewport; }
	inline ID3D11Buffer* buffer() const { return _buffer; }
	inline const DirectX::XMMATRIX& lightProjection() const { return _lightProjection; }

	inline void addSceneObject(GameObject* const go) { _sceneObjects.push_back(go); }
	inline void clearSceneObjects() { _sceneObjects.clear(); }

private:
	unsigned int                      _size{ 1024 };
	D3D11_VIEWPORT                    _viewport;
	CComPtr<ID3D11ShaderResourceView> _shaderResourceView;
	CComPtr<ID3D11DepthStencilView>   _depthStencilView;
	CComPtr<ID3D11Buffer>             _buffer;
	ID3D11VertexShader*               _vertexShader;
	ID3D11PixelShader*                _pixelShader;
	ShadowMapBuffer                   _constantBuffer;
	std::vector<GameObject*>          _sceneObjects;

	DirectX::XMMATRIX _lightProjection;
	DirectionalLight& _directionalLight;
};
