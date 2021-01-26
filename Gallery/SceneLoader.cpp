#include "SceneLoader.h"
#include <fstream>
#include <iostream>

bool SceneLoader::parseMaterial(std::ifstream& fin)
{
	MaterialData data;
	std::string str;
	while (fin >> str)
	{
		if (str == "id:")
			fin >> data.id;
		else if (str == "ambient:")
			fin >> data.ambient.x >> data.ambient.y >> data.ambient.z >> data.ambient.w;
		else if (str == "diffuse:")
			fin >> data.diffuse.x >> data.diffuse.y >> data.diffuse.z >> data.diffuse.w;
		else if (str == "specular:")
			fin >> data.specular.x >> data.specular.y >> data.specular.z >> data.specular.w;
		else if (str == "}")
		{
			_materials.push_back(data);
			break;
		}
		else
		{
			std::cerr << "Invalid material object " << str << "\n";
			return false;
		}
	}
	return true;
}

bool SceneLoader::parseMeshObject(std::ifstream& fin)
{
	MeshObjectData data;
	std::string str;
	while (fin >> str)
	{
		if (str == "shape:")
			fin >> data.shape;
		else if (str == "materialId:")
			fin >> data.materialId;
		else if (str == "tag:")
			fin >> data.tag;
		else if (str == "position:")
			fin >> data.position.x >> data.position.y >> data.position.z;
		else if (str == "rotation:")
			fin >> data.rotation.x >> data.rotation.y >> data.rotation.z;
		else if (str == "scale:")
			fin >> data.scale.x >> data.scale.y >> data.scale.z;		
		else if (str == "}")
		{
			_meshObjects.push_back(data);
			break;
		}
		else
		{
			std::cerr << "Invalid geometry object " << str << "\n";
			//return false;
		}
	}
	return true;
}

bool SceneLoader::parseDirectionalLight(std::ifstream& fin)
{
	LightData data;
	data.type = "Directional";
	std::string str;
	while (fin >> str)
	{
		if (str == "color:")
			fin >> data.color.x >> data.color.y >> data.color.z >> data.color.w;
		else if (str == "color:")
			fin >> data.color.x >> data.color.y >> data.color.z >> data.color.w;
		else if (str == "position:")
			fin >> data.position.x >> data.position.y >> data.position.z;
		else if (str == "direction:")
			fin >> data.direction.x >> data.direction.y >> data.direction.z;
		else if (str == "ambient:")
			fin >> data.ambient.x >> data.ambient.y >> data.ambient.z >> data.ambient.w;
		else if (str == "diffuse:")
			fin >> data.diffuse.x >> data.diffuse.y >> data.diffuse.z >> data.diffuse.w;
		else if (str == "specular:")
			fin >> data.specular.x >> data.specular.y >> data.specular.z >> data.specular.w;		
		else if (str == "debugMaterialId:")
			fin >> data.debugMaterialId;
		else if (str == "tag:")
			fin >> data.tag;
		else if (str == "}")
		{
			_lights.push_back(data);
			break;
		}
		else
		{
			std::cerr << "Invalid light object " << str << "\n";
			return false;
		}
	}
	return true;
}

bool SceneLoader::parsePointLight(std::ifstream& fin)
{
	LightData data;
	data.type = "Point";
	std::string str;
	while (fin >> str)
	{
		if (str == "color:")
			fin >> data.color.x >> data.color.y >> data.color.z >> data.color.w;
		else if (str == "position:")
			fin >> data.position.x >> data.position.y >> data.position.z;
		else if (str == "direction:")
			fin >> data.direction.x >> data.direction.y >> data.direction.z;
		else if (str == "ambient:")
			fin >> data.ambient.x >> data.ambient.y >> data.ambient.z >> data.ambient.w;
		else if (str == "diffuse:")
			fin >> data.diffuse.x >> data.diffuse.y >> data.diffuse.z >> data.diffuse.w;
		else if (str == "specular:")
			fin >> data.specular.x >> data.specular.y >> data.specular.z >> data.specular.w;
		else if (str == "attenuation:")
			fin >> data.attenuation.x >> data.attenuation.y >> data.attenuation.z;
		else if (str == "range:")
			fin >> data.range;
		else if (str == "debugMaterialId:")
			fin >> data.debugMaterialId;
		else if (str == "tag:")
			fin >> data.tag;
		else if (str == "}")
		{
			_lights.push_back(data);
			break;
		}
		else
		{
			std::cerr << "Invalid light object " << str << "\n";
			return false;
		}
	}
	return true;
}

bool SceneLoader::parseSpotLight(std::ifstream& fin)
{
	LightData data;
	data.type = "Spot";
	std::string str;
	while (fin >> str)
	{
		if (str == "color:")
			fin >> data.color.x >> data.color.y >> data.color.z >> data.color.w;
		else if (str == "position:")
			fin >> data.position.x >> data.position.y >> data.position.z;
		else if (str == "direction:")
			fin >> data.direction.x >> data.direction.y >> data.direction.z;
		else if (str == "ambient:")
			fin >> data.ambient.x >> data.ambient.y >> data.ambient.z >> data.ambient.w;
		else if (str == "diffuse:")
			fin >> data.diffuse.x >> data.diffuse.y >> data.diffuse.z >> data.diffuse.w;
		else if (str == "specular:")
			fin >> data.specular.x >> data.specular.y >> data.specular.z >> data.specular.w;
		else if (str == "attenuation:")
			fin >> data.attenuation.x >> data.attenuation.y >> data.attenuation.z;
		else if (str == "range:")
			fin >> data.range;
		else if (str == "spot:")
			fin >> data.spot;
		else if (str == "debugMaterialId:")
			fin >> data.debugMaterialId;
		else if (str == "tag:")
			fin >> data.tag;
		else if (str == "}")
		{
			_lights.push_back(data);
			break;
		}
		else
		{
			std::cerr << "Invalid light object " << str << "\n";
			return false;
		}
	}
	return true;
}

bool SceneLoader::parseCamera(std::ifstream& fin)
{
	CameraData data;
	std::string str;
	while (fin >> str)
	{
		if (str == "position:")
			fin >> data.position.x >> data.position.y >> data.position.z;
		else if (str == "rotation:")
			fin >> data.rotation.x >> data.rotation.y >> data.rotation.z;
		else if (str == "scale:")
			fin >> data.scale.x >> data.scale.y >> data.scale.z;
		else if (str == "}")
		{
			_cameras.push_back(data);
			break;
		}
		else
		{
			std::cerr << "Invalid camera object " << str << "\n";
			return false;
		}
	}
	return true;
}

bool SceneLoader::load(const char* const filename)
{
	_materials.clear();
	_meshObjects.clear();
	_lights.clear();
	_cameras.clear();

	std::ifstream fin(filename);
	if (!fin)
	{
		std::cerr << "Failed to open file " << filename << "\n";
		return false;
	}

	std::string str;
	while (fin >> str)
	{
		if (str == "{")
		{
			std::string key, value;
			fin >> key >> value;
			if (key == "type:")
			{
				if (value == "mesh")
				{
					if (!parseMeshObject(fin))
						return false;
				}
				else if (value == "directional_light")
				{
					if (!parseDirectionalLight(fin))
						return false;
				}
				else if (value == "point_light")
				{
					if (!parsePointLight(fin))
						return false;
				}
				else if (value == "spot_light")
				{
					if (!parseSpotLight(fin))
						return false;
				}
				else if (value == "camera")
				{
					if (!parseCamera(fin))
						return false;
				}
				else if (value == "material")
				{
					if (!parseMaterial(fin))
						return false;
				}
				else
				{
					std::cerr << "Invalid scene object type " << value << "\n";
					//return false;
				}
			}
		}
	}

	return true;
}
