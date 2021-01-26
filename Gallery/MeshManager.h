#pragma once

#include "Vertex.h"
#include <d3d11_1.h>
#include <atlbase.h>
#include <unordered_map>

struct MeshData
{
	std::vector<Vertex>   Vertices;
	std::vector<uint32_t> Indices;
};

struct Mesh
{
	MeshData Data;
	size_t   IndexCount{ 0 };
	uint32_t IndexStartLocation{ 0 };
	uint32_t BaseVextexLocation{ 0 };
};

class MeshManager
{
public:
	MeshManager() = default;

	HRESULT init(ID3D11Device* const device);
	Mesh* model(const std::string& id);
	inline ID3D11Buffer* vb() const { return _vertexBuffer; }
	inline ID3D11Buffer* ib() const { return _indexBuffer; }

private:
	std::unordered_map<std::string, Mesh> _models;

	CComPtr<ID3D11Buffer> _vertexBuffer;
	CComPtr<ID3D11Buffer> _indexBuffer;

private:

};