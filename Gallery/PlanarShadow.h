#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <atlbase.h>
#include <vector>
#include "Light.h"
#include "Material.h"

class GameObject;

class PlanarShadow
{
public:
	PlanarShadow(const Material& mat, ID3D11Buffer* const  buffer);
	~PlanarShadow() = default;
	PlanarShadow(const PlanarShadow&) = delete;
	PlanarShadow& operator=(const PlanarShadow&) = delete;

	void render(ID3D11DeviceContext* dc);

	inline void addSceneObject(GameObject* const go) { _sceneObjects.push_back(go); }
	inline void clearSceneObjects() { _sceneObjects.clear(); }

	inline void shadowMaterial(const Material& mat) { _shadowMaterial = mat; }

	void lightPositions(const std::vector<DirectX::XMFLOAT3>& lights) { _lightPos = lights; }

private:
	DirectX::XMMATRIX              _shadowMatrix;
	DirectX::XMMATRIX              _shadowOffsetY;
	std::vector<GameObject*>       _sceneObjects;
	Material                       _shadowMaterial;
	ID3D11Buffer*                  _buffer;
	std::vector<DirectX::XMFLOAT3> _lightPos;
};