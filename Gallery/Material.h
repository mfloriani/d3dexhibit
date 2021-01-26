#pragma once
#include <DirectXMath.h>

struct Material
{
	DirectX::XMFLOAT4 Ambient{ 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 Diffuse{ 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 Specular{ 0.0f, 0.0f, 0.0f, 0.0f };
};