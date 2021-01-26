#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "MeshManager.h"
#include "GeometryGenerator.h"

HRESULT MeshManager::init(ID3D11Device* const device)
{
	std::vector<Vertex>   vertices;
	std::vector<uint32_t> indices;
	
	uint32_t vertexOffset{ 0 };
	uint32_t indexOffset{ 0 };

	// Cube

	Mesh cube;		
	GeometryGenerator::CreateBox(1.f, 1.f, 1.f, 0, cube.Data);
	cube.IndexCount = cube.Data.Indices.size();
	cube.IndexStartLocation = 0;
	cube.BaseVextexLocation = 0;

	//std::pair<std::string, Mesh> m = std::make_pair("cube", cube);
	_models.insert(std::make_pair("cube", cube));

	vertices.insert(vertices.end(), cube.Data.Vertices.begin(), cube.Data.Vertices.end());
	indices.insert(indices.end(), cube.Data.Indices.begin(), cube.Data.Indices.end());

	vertexOffset += cube.Data.Vertices.size();
	indexOffset += cube.Data.Indices.size();

	

	// Sphere

	Mesh sphere;
	GeometryGenerator::CreateSphere(1.f, 20, 20, sphere.Data);
	sphere.IndexCount = sphere.Data.Indices.size();
	sphere.IndexStartLocation = indexOffset;
	sphere.BaseVextexLocation = vertexOffset;

	//std::pair<std::string, Mesh> ms = std::make_pair("sphere", sphere);
	_models.insert(std::make_pair("sphere", sphere));

	vertices.insert(vertices.end(), sphere.Data.Vertices.begin(), sphere.Data.Vertices.end());
	indices.insert(indices.end(), sphere.Data.Indices.begin(), sphere.Data.Indices.end());

	vertexOffset += sphere.Data.Vertices.size();
	indexOffset += sphere.Data.Indices.size();

	

	// Dragon
	
	std::vector<std::string> models{
		"dragon_vrip",
		"xyzrgb_dragon"
	};

	for (auto& model : models)
	{
		std::string modelId = model;
		Assimp::Importer importer;
		const aiScene* const scene = importer.ReadFile("Assets/"+ modelId +".ply", aiProcess_Triangulate | aiProcess_GenNormals);
		aiMesh* const importedMesh = scene->mMeshes[0];

		Mesh model;

		for (uint32_t i = 0; i < importedMesh->mNumVertices; ++i)
		{
			const float px = importedMesh->mVertices[i].x;
			const float py = importedMesh->mVertices[i].y;
			const float pz = importedMesh->mVertices[i].z;

			const float nx = importedMesh->mNormals[i].x;
			const float ny = importedMesh->mNormals[i].y;
			const float nz = importedMesh->mNormals[i].z;

			const Vertex vertex(
				px, py, pz,
				nx, ny, nz,
				0.0f, 0.0f, 0.0f,
				0.0f, 0.0f
			);

			model.Data.Vertices.push_back(vertex);
		}

		for (UINT i = 0; i < importedMesh->mNumFaces; ++i)
		{
			const aiFace face = importedMesh->mFaces[i];
			for (UINT j = 0; j < face.mNumIndices; ++j)
			{
				model.Data.Indices.push_back(face.mIndices[j]);
			}
		}

		model.IndexCount = model.Data.Indices.size();
		model.IndexStartLocation = indexOffset;
		model.BaseVextexLocation = vertexOffset;

		//std::pair<std::string, Mesh> m = std::make_pair(modelId, model);
		_models.insert(std::make_pair(modelId, model));

		vertices.insert(vertices.end(), model.Data.Vertices.begin(), model.Data.Vertices.end());
		indices.insert(indices.end(), model.Data.Indices.begin(), model.Data.Indices.end());

		vertexOffset += model.Data.Vertices.size();
		indexOffset += model.Data.Indices.size();

	}



	// Vertex & Index Buffers

	HRESULT hr = 0;

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = vertices.data();
	hr = device->CreateBuffer(&bd, &InitData, &_vertexBuffer);
	if (FAILED(hr)) return hr;

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT) * indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices.data();
	hr = device->CreateBuffer(&bd, &InitData, &_indexBuffer);
	if (FAILED(hr)) return hr;

	return 0;
}

Mesh* MeshManager::model(const std::string& id)
{
	const auto it = _models.find(id);
	if (it != _models.end())
		return &it->second;

	return nullptr;
}
