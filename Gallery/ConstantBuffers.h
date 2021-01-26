#pragma once

#include <DirectXMath.h>
#include "Light.h"
#include "Material.h"

struct CBOnResize
{
	DirectX::XMMATRIX Projection;
	DirectX::XMMATRIX LightProj;
};

struct CBPerFrame
{
	DirectX::XMMATRIX View;
	DirectX::XMFLOAT4 CameraEye;
	DirectionalLight  DirLight;
	PointLight        PointLight;
	DirectX::XMFLOAT4 Misc;
};

struct CBSpotLights
{
	float             numSpotLight;
	DirectX::XMFLOAT3 pad;
	SpotLight         SpotLight[MAX_SPOTLIGHTS];
};

struct CBPerObject
{
	DirectX::XMMATRIX World;
	Material          Material;
};