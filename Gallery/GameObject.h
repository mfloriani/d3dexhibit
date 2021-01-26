#pragma once

#include "Transform.h"
#include "MeshManager.h"
#include "Material.h"

class GameObject
{
	Transform    _transform;
	Mesh         _mesh;
	Material     _material;

public:
	GameObject() = default;
	~GameObject() = default;

	const Transform& transform() const { return _transform; }
	const Mesh& mesh() const { return _mesh; }
	const Material& material() const { return _material; }

	void transform(const Transform& t) { _transform = t; }
	void mesh(const Mesh * const m) { if(m) _mesh = *m; }
	void material(const Material& m) { _material = m; }

	void position(const DirectX::XMFLOAT3& pos) { _transform.position = pos; }
};