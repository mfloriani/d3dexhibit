#include "ShadowMap.h"

using namespace DirectX;

ShadowMap::ShadowMap(DirectionalLight& light, ID3D11VertexShader* const vs, ID3D11PixelShader* const ps)
	: _vertexShader(vs), _pixelShader(ps), _directionalLight(light)
{
	_lightProjection = XMMatrixOrthographicLH(10.0f, 10.0f, 0.1f, 100.0f);
}

ShadowMap::~ShadowMap()
{
}

HRESULT ShadowMap::init(ID3D11Device* const device)
{
	HRESULT hr = 0;

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = _size;
	texDesc.Height = _size;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	CComPtr<ID3D11Texture2D> depthMap;
	hr = (device->CreateTexture2D(&texDesc, nullptr, &depthMap));
	if (FAILED(hr)) return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = (device->CreateDepthStencilView(depthMap, &dsvDesc, &_depthStencilView));
	if (FAILED(hr)) return hr;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	hr = (device->CreateShaderResourceView(depthMap, &srvDesc, &_shaderResourceView));
	if (FAILED(hr)) return hr;

	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ShadowMapBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = device->CreateBuffer(&bd, nullptr, &_buffer);
	if (FAILED(hr)) return hr;

	_viewport.Width = static_cast<FLOAT>(_size);
	_viewport.Height = static_cast<FLOAT>(_size);
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
	_viewport.TopLeftX = 0;
	_viewport.TopLeftY = 0;
	
	_constantBuffer.world = XMMatrixIdentity();
	_constantBuffer.lightProj = XMMatrixTranspose(_lightProjection);
	_constantBuffer.lightPos = _directionalLight.Position;

	return hr;
}

void ShadowMap::render(ID3D11DeviceContext* const dc)
{
	dc->OMSetRenderTargets(0, nullptr, _depthStencilView);
	dc->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->RSSetViewports(1, &_viewport);
	dc->RSSetState(nullptr);

	dc->VSSetShader(_vertexShader, nullptr, 0);
	dc->VSSetConstantBuffers(0, 1, &_buffer.p);
	dc->PSSetShader(_pixelShader, nullptr, 0);

	_constantBuffer.lightPos = _directionalLight.Position;

	for (const auto o : _sceneObjects)
	{
		XMMATRIX translation = XMMatrixTranslation(o->transform().position.x, o->transform().position.y, o->transform().position.z);
		XMMATRIX rotationX = XMMatrixRotationX(o->transform().rotation.x);
		XMMATRIX rotationY = XMMatrixRotationY(o->transform().rotation.y);
		XMMATRIX rotationZ = XMMatrixRotationZ(o->transform().rotation.z);
		XMMATRIX scale = XMMatrixScaling(o->transform().scale.x, o->transform().scale.y, o->transform().scale.z);

		const XMMATRIX world = scale * rotationX * rotationY * rotationZ * translation;

		_constantBuffer.world = XMMatrixTranspose(world);
		dc->UpdateSubresource(_buffer, 0, nullptr, &_constantBuffer, 0, 0);

		dc->DrawIndexed(o->mesh().IndexCount, o->mesh().IndexStartLocation, o->mesh().BaseVextexLocation);
	}
}

