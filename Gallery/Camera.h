#pragma once

#include "Transform.h"

struct CameraSettings
{
	DirectX::XMFLOAT3 eye;
	DirectX::XMFLOAT3 at;
};

class Camera
{
	DirectX::XMFLOAT3 _eye;
	DirectX::XMFLOAT3 _at;
	DirectX::XMFLOAT3 _up;
	DirectX::XMFLOAT3 _right;

	DirectX::XMFLOAT4X4 _view;
	DirectX::XMFLOAT4X4 _projection;

	float _fov;
	float _aspectRatio;
	float _nearZ;
	float _farZ;

public:

	Camera();
	~Camera() = default;

	void resetView();

	void setProjection(float fov, float aspectRatio, float nearZ, float farZ);

	const DirectX::XMFLOAT3& eye() const { return _eye; };
	void eye(const DirectX::XMFLOAT3& eye) { _eye = eye; };
	void lookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& at);

	void updateView();
	void updateProjection();

	void moveX(const float s);
	void moveY(const float s);
	void moveZ(const float s);
	void rotateX(const float angle);
	void rotateY(const float angle);

	const DirectX::XMMATRIX& view() const { return DirectX::XMLoadFloat4x4(&_view); }
	const DirectX::XMMATRIX& projection() const { return DirectX::XMLoadFloat4x4(&_projection); }
	
};