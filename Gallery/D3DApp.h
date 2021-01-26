#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <atlbase.h>
#include <vector>
#include <unordered_map>
#include <memory>

#include "Camera.h"
#include "Light.h"
#include "GameObject.h"
#include "ShadowMap.h"
#include "ShaderManager.h"
#include "MeshManager.h"
#include "ConstantBuffers.h"
#include "PlanarShadow.h"
#include "ParticleSystem.h"

using namespace DirectX;

enum class RenderMode
{
	Gouraud,
	Phong
};

enum class LightingMode
{
	Ambient_StaticPoint,
	Ambient_DynamicPoint,
	Ambient_Spotlights
};

enum class ExhibitMode
{
	Sphere,
	Dragon,
	AsianDragon
};

enum class ShadowMode
{
	Planar,
	Mapping
};

class D3DApp
{	
	HINSTANCE _hInst = nullptr;
	HWND      _hWnd = nullptr;

	D3D_DRIVER_TYPE   _driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL _featureLevel = D3D_FEATURE_LEVEL_11_1;	

	CComPtr<ID3D11Device>           _d3dDevice;
	CComPtr<ID3D11DeviceContext>    _d3dDC;
	CComPtr<ID3D11Device1>          _d3dDevice1;	
	CComPtr<ID3D11DeviceContext1>   _d3dDC1;
	CComPtr<IDXGISwapChain1>        _swapChain;
	CComPtr<IDXGISwapChain1>        _swapChain1;
	CComPtr<ID3D11RenderTargetView> _pRenderTargetView;
	CComPtr<ID3D11DepthStencilView> _pDepthStencilView;
	CComPtr<ID3D11Texture2D>        _pDepthStencilBuffer;
	CComPtr<ID3D11Buffer>           _pVertexBuffer;
	CComPtr<ID3D11Buffer>           _pIndexBuffer;
	
	CComPtr<ID3D11Buffer> _pCBOnResize;
	CComPtr<ID3D11Buffer> _pCBPerFrame;
	CComPtr<ID3D11Buffer> _pCBPerObject;
	CComPtr<ID3D11Buffer> _pCBSpotLights;
	
	D3D11_VIEWPORT _screenViewport;

	RenderMode   _renderTechnique;
	LightingMode _lightingMode;
	ExhibitMode  _exhibitMode;
	ShadowMode   _shadowMode;

	ID3D11VertexShader* _pCurrentVS{ nullptr };
	ID3D11PixelShader*  _pCurrentPS{ nullptr };

	UINT      _wWidth{ 0 };
	UINT      _wHeight{ 0 };
	size_t    _numSpotLights{ 0 };
	float     _timeFactor{ 1.0f };
	bool      _turnWallTransparent{ false };
	bool      _dustEnabled{ false };
	
	Camera _camera;
	
	std::vector<DirectionalLight> _directionalLights;
	std::vector<PointLight>       _pointLights;
	std::vector<SpotLight>        _spotLights;
	std::vector<CameraSettings>   _cameraSettings;
	std::vector<Material>         _materials;
	std::vector<GameObject>		  _exhibits;
	std::vector<GameObject>       _walls;
	
	GameObject* _activeExhibit{ nullptr };
	GameObject	_ceiling;
	GameObject	_floor;
	GameObject  _sun;
	GameObject  _plinth;
	PointLight  _activePointLight;

	std::unordered_map<std::string, int> _materialMap;

	std::unique_ptr<ShadowMap>      _shadowMap{ nullptr };
	std::unique_ptr<PlanarShadow>   _planarShadow{ nullptr };
	std::unique_ptr<ShaderManager>  _shaderManager{ nullptr };
	std::unique_ptr<MeshManager>    _meshManager{ nullptr };
	std::unique_ptr<ParticleSystem> _particleSys{ nullptr };

	CComPtr<ID3D11ShaderResourceView>  _wallTexture_SRV;
	CComPtr<ID3D11ShaderResourceView>  _wallNormalMapTexture_SRV;

private:
	HRESULT createConstantBuffers();
	HRESULT createShaders();
	HRESULT loadScene();
	
	HRESULT resetScene();
	void setShadowScene();

	Material* material(const std::string& id);

public:

	D3DApp() = default;
	D3DApp(D3DApp&) = delete;
	D3DApp(D3DApp&&) = delete;
	D3DApp& operator=(const D3DApp&) = delete;
	D3DApp& operator=(const D3DApp&&) = delete;
	~D3DApp();

	LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	HRESULT initWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT initDevice();	
	HRESULT initScene();

	void updateShadowLights();
	void update(float dt);
	void renderScene();
	void render();
	void renderObject(const GameObject * const go);
	
};