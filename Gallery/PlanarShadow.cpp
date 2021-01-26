#include "PlanarShadow.h"
#include "RenderStates.h"
#include "ConstantBuffers.h"
#include "GameObject.h"

using namespace DirectX;

PlanarShadow::PlanarShadow(	const Material& mat, ID3D11Buffer* const buffer) : _shadowMaterial(mat), _buffer(buffer)
{
}


void PlanarShadow::render(ID3D11DeviceContext* const dc)
{
	dc->OMSetDepthStencilState(RenderStates::noDoubleBlendDSS(), 0);

	CBPerObject cbo;
	cbo.Material = _shadowMaterial;

	for (const auto go : _sceneObjects)
	{
		for (const auto& light : _lightPos)
		{
			const XMVECTOR lLookAt = XMLoadFloat3( &go->transform().position );
			const XMVECTOR lPos = XMLoadFloat3(&light);
			const XMVECTOR lDir = -XMVector3Normalize(lPos - lLookAt);

			XMMATRIX translation = XMMatrixTranslation(go->transform().position.x, go->transform().position.y, go->transform().position.z);
			XMMATRIX rotationX = XMMatrixRotationX(go->transform().rotation.x);
			XMMATRIX rotationY = XMMatrixRotationY(go->transform().rotation.y);
			XMMATRIX rotationZ = XMMatrixRotationZ(go->transform().rotation.z);
			XMMATRIX scale = XMMatrixScaling(go->transform().scale.x, go->transform().scale.y, go->transform().scale.z);
			XMMATRIX world = scale * rotationX * rotationY * rotationZ * translation;

			const XMVECTOR shadowPlane = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			const XMVECTOR toLight = -lDir;
			_shadowMatrix = XMMatrixShadow(shadowPlane, toLight);
			//_shadowOffsetY = XMMatrixTranslation(0.f, 0.001f, 0.0f);
			_shadowOffsetY = XMMatrixTranslation(0.f, -1.001f, 0.0f);

			world = world * _shadowMatrix * _shadowOffsetY;
			cbo.World = XMMatrixTranspose(world);
		
			dc->UpdateSubresource(_buffer, 0, nullptr, &cbo, 0, 0);
			dc->DrawIndexed(go->mesh().IndexCount, go->mesh().IndexStartLocation, go->mesh().BaseVextexLocation);
		}
	}

	dc->OMSetDepthStencilState(nullptr, 0);
}
