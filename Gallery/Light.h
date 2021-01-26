#pragma once

#include <DirectXMath.h>

#define MAX_SPOTLIGHTS 4

struct DirectionalLight
{
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	DirectionalLight( 
		const DirectX::XMFLOAT4& ambient, const DirectX::XMFLOAT4& diffuse, const DirectX::XMFLOAT4& specular,
		const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
		: Ambient(ambient), Diffuse(diffuse), Specular(specular),  
		  Direction(direction), Position(position) {}

	DirectX::XMFLOAT4 Ambient;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular;
	DirectX::XMFLOAT3 Direction;
	float			  Pad{ 0.0f };
	DirectX::XMFLOAT3 Position;
	float			  Pad2{ 0.0f };
};

struct PointLight
{
	PointLight() { ZeroMemory(this, sizeof(this)); }

	PointLight(const DirectX::XMFLOAT4& ambient, const DirectX::XMFLOAT4& diffuse, const DirectX::XMFLOAT4& specular,
		const DirectX::XMFLOAT3& position, const float range, const DirectX::XMFLOAT3& attenuation)
		: Ambient(ambient), Diffuse(diffuse), Specular(specular), Position(position), Range(range),
		  Attenuation(attenuation) {}

	DirectX::XMFLOAT4 Ambient;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular;
	DirectX::XMFLOAT3 Position;
	float			  Range;
	DirectX::XMFLOAT3 Attenuation;
	float             Pad{0.0f};
};

struct SpotLight
{
	SpotLight() { ZeroMemory(this, sizeof(this)); }

	SpotLight(
		const DirectX::XMFLOAT4& ambient, const DirectX::XMFLOAT4& diffuse, const DirectX::XMFLOAT4& specular,
		const DirectX::XMFLOAT3& position, const float range, const DirectX::XMFLOAT3& direction, const float spot,
		const DirectX::XMFLOAT3& attenuation)
		: Ambient(ambient), Diffuse(diffuse), Specular(specular),
		Position(position), Range(range), Direction(direction),
		Spot(spot), Attenuation(attenuation) {}

	DirectX::XMFLOAT4 Ambient;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular;
	DirectX::XMFLOAT3 Position;
	float			  Range;
	DirectX::XMFLOAT3 Direction;
	float			  Spot;
	DirectX::XMFLOAT3 Attenuation;
	float             Pad{ 0.0f };
};