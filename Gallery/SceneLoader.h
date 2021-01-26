#pragma once

#include <vector>
#include <string>
#include <DirectXMath.h>

struct MaterialData
{
	std::string       type;
	std::string       id;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
};
struct MeshObjectData
{
	std::string       shape;
	std::string       materialId;
	std::string       tag;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
};
struct LightData
{
	std::string       type;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	DirectX::XMFLOAT3 attenuation;
	float             range;
	float             spot;
	std::string       debugMaterialId;
	std::string       tag;
};
struct CameraData
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
};


class SceneLoader
{
	std::vector<MaterialData>   _materials;
	std::vector<MeshObjectData> _meshObjects;
	std::vector<LightData>	    _lights;
	std::vector<CameraData>     _cameras;

	bool parseMaterial(std::ifstream& fin);
	bool parseMeshObject(std::ifstream& fin);
	bool parseDirectionalLight(std::ifstream& fin);
	bool parsePointLight(std::ifstream& fin);
	bool parseSpotLight(std::ifstream& fin);
	bool parseCamera(std::ifstream& fin);

public:
	SceneLoader() = default;
	~SceneLoader() = default;

	bool load(const char* filename);

	const std::vector<MaterialData>& materials() const { return _materials; }
	const std::vector<MeshObjectData>& meshObjects() const { return _meshObjects; }
	const std::vector<LightData>& lights() const { return _lights; }
	const std::vector<CameraData>& cameras() const { return _cameras; }
};