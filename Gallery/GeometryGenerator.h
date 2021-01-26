// Code extracted from Frank Luna D3D12 book.

#pragma once

#include "MeshManager.h"

class GeometryGenerator
{
public:
	GeometryGenerator() = default;

	static void CreateBox(float width, float height, float depth, uint32_t numSubdivisions, MeshData& meshData);
	static void CreateSphere(float radius, uint32_t sliceCount, uint32_t stackCount, MeshData& meshData);
	static void CreateQuad(float x, float y, float w, float h, float depth, MeshData& meshData);

private:
	static void Subdivide(MeshData& meshData);
	static Vertex MidPoint(const Vertex& v0, const Vertex& v1);
};