#include "D3DApp.h"
#include "Resource.h"
#include "GeometryGenerator.h"
#include "SceneLoader.h"
#include "RenderStates.h"
#include "Vertex.h"
#include "Constants.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#pragma comment( lib, "dxguid.lib")
#include "DDSTextureLoader.h"

// Solution that I found to call member data from window callback
LRESULT CALLBACK staticWndProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	D3DApp* const pWnd = reinterpret_cast<D3DApp*>(GetWindowLongPtrW(hWnd, 0));
	if (pWnd) {
		return pWnd->wndProc(hWnd, message, wParam, lParam);
	}
	else {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

LRESULT CALLBACK D3DApp::wndProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (message) {
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;

		case VK_F1:
			if (_cameraSettings.size() > 0)
			{
				_camera.resetView();
				_camera.lookAt(_cameraSettings[0].eye, _cameraSettings[0].at);
			}
			break;

		case VK_F2:
			if (_cameraSettings.size() > 1)
			{
				_camera.resetView();
				_camera.lookAt(_cameraSettings[1].eye, _cameraSettings[1].at);
			}
			break;

		case VK_F3:
			if (_exhibitMode == ExhibitMode::Sphere)
			{
				_exhibitMode = ExhibitMode::Dragon;
				_activeExhibit = &_exhibits[1];
			}
			else if (_exhibitMode == ExhibitMode::Dragon)
			{
				_exhibitMode = ExhibitMode::AsianDragon;
				_activeExhibit = &_exhibits[2];
			}
			else if (_exhibitMode == ExhibitMode::AsianDragon)
			{
				_exhibitMode = ExhibitMode::Sphere;
				_activeExhibit = &_exhibits[0];
			}
			setShadowScene();
			break;

		case VK_F4:
			if (_renderTechnique == RenderMode::Gouraud) 
			{
				_renderTechnique = RenderMode::Phong;
				_pCurrentVS = _shaderManager->vs("Gallery_Phong_VS");
				_pCurrentPS = _shaderManager->ps("Gallery_Phong_PS");
			}
			else if (_renderTechnique == RenderMode::Phong)
			{
				_renderTechnique = RenderMode::Gouraud;
				_pCurrentVS = _shaderManager->vs("Gallery_Gouraud_VS");
				_pCurrentPS = _shaderManager->ps("Gallery_Gouraud_PS");
			}

			break;

		case VK_F5:
			if (_lightingMode == LightingMode::Ambient_StaticPoint)
			{
				_lightingMode = LightingMode::Ambient_DynamicPoint;
				_activePointLight = _pointLights[0];
				_numSpotLights = 0;
			}
			else if (_lightingMode == LightingMode::Ambient_DynamicPoint)
			{
				_lightingMode = LightingMode::Ambient_Spotlights;
				_activePointLight = PointLight();
				_numSpotLights = _spotLights.size();
			}
			else if (_lightingMode == LightingMode::Ambient_Spotlights)
			{
				_lightingMode = LightingMode::Ambient_StaticPoint;
				_activePointLight = _pointLights[0];
				_numSpotLights = 0;
			}
			updateShadowLights();
			break;

		case VK_F6:
			
			if (_shadowMode == ShadowMode::Planar)
			{
				_shadowMode = ShadowMode::Mapping;
			}
			else if (_shadowMode == ShadowMode::Mapping)
			{
				_shadowMode = ShadowMode::Planar;
			}

			break;

		case VK_F7:
			// F7 switches between scene level rendering modes(advanced feature, optional)
			break;

		case VK_F8:
			// F8 start / stop the dust falling
			_dustEnabled = !_dustEnabled;
			break;

		case VK_F12:
			// Debug mode 

			break;

		case VK_PRIOR:
			if (GetKeyState(VK_CONTROL) & KEYISDOWN)
			{
				_camera.moveY(TRANSLATION_SPEED);
			}
			else if (GetKeyState(VK_LSHIFT) & KEYISDOWN)
			{
				_directionalLights[0].Position.y += LIGHT_SPEED;
				_sun.position(_directionalLights[0].Position);
				updateShadowLights();
			}
			else if (GetKeyState(VK_RSHIFT) & KEYISDOWN)
			{
				_spotLights[0].Position.y += LIGHT_SPEED;
				//_spotControl->position(_spotLights[0].Position);
			}
			break;

		case VK_NEXT:
			if (GetKeyState(VK_CONTROL) & KEYISDOWN)
			{
				_camera.moveY(-TRANSLATION_SPEED);
			}
			else if (GetKeyState(VK_LSHIFT) & KEYISDOWN)
			{
				_directionalLights[0].Position.y -= LIGHT_SPEED;
				_sun.position(_directionalLights[0].Position);
				updateShadowLights();
			}
			else if (GetKeyState(VK_RSHIFT) & KEYISDOWN)
			{
				_spotLights[0].Position.y -= LIGHT_SPEED;
				//_spotControl->position(_spotLights[0].Position);
			}
			break;

		case VK_UP:
		case VK_W:
		case VK_I:
			
			if (GetKeyState(VK_CONTROL) & KEYISDOWN)
			{
				_camera.moveZ(TRANSLATION_SPEED);
			}
			else if (GetKeyState(VK_LSHIFT) & KEYISDOWN)
			{
				_directionalLights[0].Position.z += LIGHT_SPEED;
				_sun.position(_directionalLights[0].Position);
				updateShadowLights();
			}
			else if (GetKeyState(VK_RSHIFT) & KEYISDOWN)
			{
				_spotLights[0].Position.z += LIGHT_SPEED;
				//_spotControl->position(_spotLights[0].Position);
			}
			else
			{
				_camera.rotateX(-ROTATION_SPEED);
			}

			break;

		case VK_DOWN:
		case VK_S:
		case VK_K:
			
			if (GetKeyState(VK_CONTROL) & KEYISDOWN)
			{
				_camera.moveZ(-TRANSLATION_SPEED);
			}
			else if (GetKeyState(VK_LSHIFT) & KEYISDOWN)
			{
				_directionalLights[0].Position.z -= LIGHT_SPEED;
				_sun.position(_directionalLights[0].Position);
				updateShadowLights();
			}
			else if (GetKeyState(VK_RSHIFT) & KEYISDOWN)
			{
				_spotLights[0].Position.z -= LIGHT_SPEED;
				//_spotControl->position(_spotLights[0].Position);
			}
			else
			{
				_camera.rotateX(ROTATION_SPEED);
			}

			break;

		case VK_RIGHT:
		case VK_D:
		case VK_L:
			
			if (GetKeyState(VK_CONTROL) & KEYISDOWN)
			{
				_camera.moveX(TRANSLATION_SPEED);
			}
			else if (GetKeyState(VK_LSHIFT) & KEYISDOWN)
			{
				_directionalLights[0].Position.x += LIGHT_SPEED;
				_sun.position(_directionalLights[0].Position);
				updateShadowLights();
			}
			else if (GetKeyState(VK_RSHIFT) & KEYISDOWN)
			{
				_spotLights[0].Position.x += LIGHT_SPEED;
				//_spotControl->position(_spotLights[0].Position);
			}
			else
			{
				_camera.rotateY(ROTATION_SPEED);
			}

			break;

		case VK_LEFT:
		case VK_A:
		case VK_J:

			if (GetKeyState(VK_CONTROL) & KEYISDOWN)
			{
				_camera.moveX(-TRANSLATION_SPEED);
			}
			else if (GetKeyState(VK_LSHIFT) & KEYISDOWN)
			{
				_directionalLights[0].Position.x -= LIGHT_SPEED;
				_sun.position( _directionalLights[0].Position );
				updateShadowLights();
			}
			else if (GetKeyState(VK_RSHIFT) & KEYISDOWN)
			{
				_spotLights[0].Position.x -= LIGHT_SPEED;
				//_spotControl->position(_spotLights[0].Position);
			}
			else
			{
				_camera.rotateY(-ROTATION_SPEED);
			}

			break;

		case VK_T:
			
			if (GetKeyState(VK_SHIFT) & KEYISDOWN)
			{
				// increase a factor that globally speeds - up time - dependent effects
				_timeFactor -= TIME_FACTOR_RATE;
				if (_timeFactor < 0.f)
					_timeFactor = 0.f;
			}
			else
			{
				// decrease a factor that globally slows - up time - dependent effects
				_timeFactor += TIME_FACTOR_RATE;
			}
			break;

		case VK_R:
			
			resetScene();

			break;
		}

		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT D3DApp::initWindow(const HINSTANCE hInstance, const int nCmdShow)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = staticWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(D3DApp);
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, reinterpret_cast<LPCTSTR>(IDI_GALLERY));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"Starter Template";
	wcex.hIconSm = LoadIcon(wcex.hInstance, reinterpret_cast<LPCTSTR>(IDI_GALLERY));
	if (!RegisterClassEx(&wcex))
		return -1;// E_FAIL;

	// Create window
	_hInst = hInstance;
	RECT rc = { 0, 0, 800, 600 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	_hWnd = CreateWindow(L"Starter Template", L"Direct3D 11 Gallery",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	if (!_hWnd)
		return -1;// E_FAIL;

	// passing `this` pointer to the window, so I can call member function in wnd callback
	SetWindowLongPtrW(_hWnd, 0, reinterpret_cast<LONG_PTR>(this));

	ShowWindow(_hWnd, nCmdShow);

	return 0;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT D3DApp::initDevice()
{
	HRESULT hr = 0;

	RECT rc;
	GetClientRect(_hWnd, &rc);
	_wWidth = rc.right - rc.left;
	_wHeight = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	std::array<D3D_DRIVER_TYPE,3> driverTypes {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	const auto numDriverTypes = driverTypes.size();

	std::array<D3D_FEATURE_LEVEL,4> featureLevels {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	const auto numFeatureLevels = featureLevels.size();

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++) {
		_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels.data(), numFeatureLevels, D3D11_SDK_VERSION, &_d3dDevice, &_featureLevel, &_d3dDC);

		if (hr == static_cast<HRESULT>(0x80070057L))
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDevice(nullptr, _driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1, D3D11_SDK_VERSION, &_d3dDevice, &_featureLevel, &_d3dDC);

		if (SUCCEEDED(hr)) break;
	}
	if (FAILED(hr)) return hr;

	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
	CComPtr <IDXGIFactory1> dxgiFactory;
	{
		CComPtr <IDXGIDevice> dxgiDevice;
		hr = _d3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr)) {
			CComPtr <IDXGIAdapter> adapter;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr)) {
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
			}
		}
	}
	if (FAILED(hr)) return hr;

	// Create swap chain
	CComPtr <IDXGIFactory2> dxgiFactory2;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));

	// DirectX 11.1 or later
	hr = _d3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&_d3dDevice1));
	if (SUCCEEDED(hr)) {
		_d3dDC->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&_d3dDC1));
	}

	DXGI_SWAP_CHAIN_DESC1 sd{};
	sd.Width = _wWidth;
	sd.Height = _wHeight;
	sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;

	hr = dxgiFactory2->CreateSwapChainForHwnd(_d3dDevice, _hWnd, &sd, nullptr, nullptr, &_swapChain1);
	if (SUCCEEDED(hr)) {
		hr = _swapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&_swapChain));
	}

	// Note this tutorial doesn't handle full-screen swap chains so we block the ALT+ENTER shortcut
	dxgiFactory->MakeWindowAssociation(_hWnd, DXGI_MWA_NO_ALT_ENTER);

	if (FAILED(hr)) return hr;

	// Create a render target view
	CComPtr<ID3D11Texture2D> pBackBuffer;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr)) return hr;

	hr = _d3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
	if (FAILED(hr)) return hr;

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = _wWidth;
	depthStencilDesc.Height = _wHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;	
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	
	hr = _d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &_pDepthStencilBuffer);
	if (FAILED(hr)) return hr;

	hr = _d3dDevice->CreateDepthStencilView(_pDepthStencilBuffer, nullptr, &_pDepthStencilView);
	if (FAILED(hr)) return hr;
	
	_screenViewport.Width = static_cast<FLOAT>(_wWidth);
	_screenViewport.Height = static_cast<FLOAT>(_wHeight);
	_screenViewport.MinDepth = 0.0f;
	_screenViewport.MaxDepth = 1.0f;
	_screenViewport.TopLeftX = 0;
	_screenViewport.TopLeftY = 0;
	
	hr = createShaders();
	if (FAILED(hr)) return hr;

	hr = createConstantBuffers();
	if (FAILED(hr)) return hr;

	RenderStates::init(_d3dDevice);

	_meshManager = std::make_unique<MeshManager>();
	_meshManager->init(_d3dDevice);

	return 0;
}

HRESULT D3DApp::initScene()
{
	HRESULT hr = 0;

	hr = loadScene();
	if (FAILED(hr)) 
		return hr;

	_shadowMap = std::make_unique<ShadowMap>(
		_directionalLights[0],
		_shaderManager->vs("ShadowMap_VS"),
		_shaderManager->ps("ShadowMap_PS")
		);

	hr = _shadowMap->init(_d3dDevice);
	if (FAILED(hr)) return hr;

	Material * const shadowMat = material("shadow");
	if (!shadowMat) return -1;

	_planarShadow = std::make_unique<PlanarShadow>( *shadowMat, _pCBPerObject.p	);

	hr = resetScene();
	if (FAILED(hr)) return hr;


	hr = CreateDDSTextureFromFile(_d3dDevice, L"Assets/brickwall.dds", nullptr, &_wallTexture_SRV);
	if (FAILED(hr)) return hr;

	hr = CreateDDSTextureFromFile(_d3dDevice, L"Assets/brickwall_normal.dds", nullptr, &_wallNormalMapTexture_SRV);
	if (FAILED(hr)) return hr;

	return hr;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
D3DApp::~D3DApp()
{
	try
	{
		if (_d3dDC) _d3dDC->ClearState();
	}
	catch (...)
	{

	}
}

HRESULT D3DApp::createConstantBuffers()
{
	HRESULT hr = 0;

	// Create the constant buffer
	D3D11_BUFFER_DESC bd = {};

	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBOnResize);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = _d3dDevice->CreateBuffer(&bd, nullptr, &_pCBOnResize);
	if (FAILED(hr)) return hr;

	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBPerFrame);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = _d3dDevice->CreateBuffer(&bd, nullptr, &_pCBPerFrame);
	if (FAILED(hr)) return hr;

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBPerObject);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = _d3dDevice->CreateBuffer(&bd, nullptr, &_pCBPerObject);
	if (FAILED(hr)) return hr;

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBSpotLights);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = _d3dDevice->CreateBuffer(&bd, nullptr, &_pCBSpotLights);
	if (FAILED(hr)) return hr;
	
	return hr;
}

HRESULT D3DApp::createShaders()
{
	HRESULT hr = 0;
	
	_shaderManager = std::make_unique<ShaderManager>();

	std::vector<unsigned char> byteCode;

	hr = _shaderManager->createVertexShader(_d3dDevice, "Gallery_Phong", "VS", byteCode);
	if (FAILED(hr)) return hr;

	hr = InputLayouts::createBasic(_d3dDevice, byteCode);
	if (FAILED(hr)) return hr;

	hr = _shaderManager->createPixelShader(_d3dDevice, "Gallery_Phong", "PS");
	if (FAILED(hr)) return hr;

	hr = _shaderManager->createVertexShader(_d3dDevice, "Gallery_Gouraud", "VS", byteCode);
	if (FAILED(hr)) return hr;

	hr = _shaderManager->createPixelShader(_d3dDevice, "Gallery_Gouraud", "PS");
	if (FAILED(hr)) return hr;

	hr = _shaderManager->createVertexShader(_d3dDevice, "ShadowMap", "VS", byteCode);
	if (FAILED(hr)) return hr;

	hr = _shaderManager->createPixelShader(_d3dDevice, "ShadowMap", "PS");
	if (FAILED(hr)) return hr;

	// Dust

	hr = _shaderManager->createVertexShader(_d3dDevice, "Dust", "VS", byteCode);
	if (FAILED(hr)) return hr;

	hr = InputLayouts::createParticle(_d3dDevice, byteCode);
	if (FAILED(hr)) return hr;

	hr = _shaderManager->createGeometryShader(_d3dDevice, "Dust", "GS");
	if (FAILED(hr)) return hr;

	hr = _shaderManager->createPixelShader(_d3dDevice, "Dust", "PS");
	if (FAILED(hr)) return hr;

	hr = _shaderManager->createComputeShader(_d3dDevice, "Dust", "CS");
	if (FAILED(hr)) return hr;

	return hr;
}

HRESULT D3DApp::resetScene()
{
	HRESULT hr;

	_camera = Camera();
	if (_cameraSettings.size() > 0)
		_camera.lookAt(_cameraSettings[0].eye, _cameraSettings[0].at);

	_camera.setProjection(XM_PIDIV2, _wWidth / static_cast<FLOAT>(_wHeight), 0.01f, 100.f);

	CBOnResize cbOnResize;
	cbOnResize.Projection = XMMatrixTranspose(_camera.projection());
	cbOnResize.LightProj = XMMatrixTranspose(_shadowMap->lightProjection());
	_d3dDC->UpdateSubresource(_pCBOnResize, 0, nullptr, &cbOnResize, 0, 0);

	_renderTechnique = RenderMode::Gouraud;
	_pCurrentVS = _shaderManager->vs("Gallery_Gouraud_VS");
	_pCurrentPS = _shaderManager->ps("Gallery_Gouraud_PS");

	_lightingMode = LightingMode::Ambient_StaticPoint;
	_activePointLight = _pointLights[0];
	_numSpotLights = 0;

	_timeFactor = 1.0f;

	_exhibitMode = ExhibitMode::Sphere;
	_activeExhibit = &_exhibits[0];

	_shadowMode = ShadowMode::Planar;

	setShadowScene();

	if(_particleSys)
		_particleSys.release();

	_particleSys = std::make_unique<ParticleSystem>(
		_shaderManager->cs("Dust_CS"),
		_shaderManager->vs("Dust_VS"),
		_shaderManager->gs("Dust_GS"),
		_shaderManager->ps("Dust_PS")
	);

	hr = _particleSys->init(_d3dDevice);
	if (FAILED(hr)) return hr;

	updateShadowLights();

	return hr;
}

void D3DApp::setShadowScene()
{
	_shadowMap->clearSceneObjects();
	_shadowMap->addSceneObject(_activeExhibit);
	_shadowMap->addSceneObject(&_plinth);

	_planarShadow->clearSceneObjects();
	_planarShadow->addSceneObject(_activeExhibit);
	_planarShadow->addSceneObject(&_plinth);
}

Material* D3DApp::material(const std::string& id)
{
	const auto it = _materialMap.find(id);
	if (it != _materialMap.end())
		return &_materials[it->second];

	return nullptr;
}

HRESULT D3DApp::loadScene()
{
	SceneLoader loader;
	if (loader.load("scene.data"))
	{
		for (auto& mat : loader.materials())
		{
			const Material material{ mat.ambient, mat.diffuse, mat.specular };
			_materials.push_back( material );
			_materialMap.insert(std::make_pair<std::string, int>(mat.id.c_str(), _materials.size()-1));
		}

		for (auto& geo : loader.meshObjects())
		{
			GameObject go;

			Transform transform;
			transform.position = geo.position;
			transform.rotation = geo.rotation;
			transform.scale = geo.scale;

			go.transform(transform);

			go.mesh(_meshManager->model(geo.shape));

			Material material;
			const auto it = _materialMap.find(geo.materialId);
			if (it != _materialMap.end())
				material = _materials[it->second];

			go.material(material);

			if (geo.tag == "exhibit")
				_exhibits.push_back(go);
			else if (geo.tag == "plinth")
				_plinth = go;
			else if (geo.tag == "wall")
				_walls.push_back(go);
			else if (geo.tag == "ceiling")
				_ceiling = go;
			else if (geo.tag == "floor")
				_floor = go;
			
		}

		for (auto& cam : loader.cameras())
		{
			_cameraSettings.push_back(
				{
					XMFLOAT3(cam.position.x, cam.position.y, cam.position.z),
					XMFLOAT3(cam.rotation.x, cam.rotation.y, cam.rotation.z),
				}
			);
		}


		for (auto& light : loader.lights())
		{
			if (light.type == "Directional")
			{
				
				_directionalLights.push_back(
					DirectionalLight(light.ambient, light.diffuse, light.specular, light.direction, light.position)
				);
			}
			else if (light.type == "Point")
			{
				_pointLights.push_back(
					PointLight(light.ambient, light.diffuse, light.specular,
					           light.position, light.range, light.attenuation
				));
			}
			else if (light.type == "Spot")
			{
				_spotLights.push_back(
					SpotLight(light.ambient, light.diffuse, light.specular,
					          light.position, light.range, light.direction,
					          light.spot, light.attenuation
				));
			}
		}
	}
	return 0;
}

void D3DApp::updateShadowLights()
{
	if (_lightingMode == LightingMode::Ambient_Spotlights)
	{
		std::vector<DirectX::XMFLOAT3> positions;
		for (const auto& spot : _spotLights)
			positions.push_back(spot.Position);

		_planarShadow->lightPositions(positions);
	}
	else
	{
		std::vector<DirectX::XMFLOAT3> positions;
		positions.push_back(_directionalLights[0].Position);
		_planarShadow->lightPositions(positions);
	}
}

void D3DApp::update(const float dt)
{
	//std::wstringstream s;
	//s << L"dt: " << dt << "\n";
	//std::wstring ws = s.str();
	//OutputDebugString(ws.c_str());

	static float t = 0.0f;
	static ULONGLONG timeStart = 0;	
	const ULONGLONG timeCur = GetTickCount64();
	if (timeStart == 0) timeStart = timeCur;	
	t = static_cast<float>(timeCur - timeStart) / 1000.0f;
	
	//std::wstringstream s;
	//s << L"_prevTime: " << _prevTime << L" timeCur: " << timeCur << L" _dt: " << _dt << "\n";
	//std::wstring ws = s.str();
	//OutputDebugString(ws.c_str());

	_camera.updateView();

	// Update directional light direction
	const XMVECTOR lLookAt = { 0, 0, 0 };
	const XMVECTOR lPos = XMLoadFloat3(&_directionalLights[0].Position);
	const XMVECTOR lDir = -XMVector3Normalize(lPos - lLookAt);
	XMStoreFloat3(&_directionalLights[0].Direction, lDir);

	// check if is far enough to make the walls transparent
	const XMVECTOR origin = { 0.f, 2.f, 0.f, 0.f };
	const XMVECTOR camPos = XMLoadFloat3(&_camera.eye());
	float camDistFromOrigin = 0.f;
	XMStoreFloat(&camDistFromOrigin, XMVector3Length(origin - camPos));
	_turnWallTransparent = camDistFromOrigin > TRANSPARENCY_DISTANCE;

	// update the point light to move around the ceiling
	if (_lightingMode == LightingMode::Ambient_DynamicPoint)
	{
		XMMATRIX orbit = XMMatrixRotationY(t * _timeFactor);
		XMMATRIX offset = XMMatrixTranslation(0.0f, _activePointLight.Position.y, 10.0f);
		const XMMATRIX world = offset * orbit;
		const XMVECTOR initPos = XMVectorSet(0, 0, 0, 0);
		const XMVECTOR newPos = XMVector3Transform(initPos, world);
		XMStoreFloat3(&_activePointLight.Position, newPos);
	}

	if (_dustEnabled)
		_particleSys->update(_d3dDC, dt * _timeFactor);
}


void D3DApp::renderScene()
{
	_d3dDC->OMSetRenderTargets(1, &_pRenderTargetView.p, _pDepthStencilView);
	_d3dDC->ClearRenderTargetView(_pRenderTargetView, Colors::DarkBlue);
	_d3dDC->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	_d3dDC->RSSetViewports(1, &_screenViewport);

	CBPerFrame cbf;
	cbf.View = XMMatrixTranspose(_camera.view());
	cbf.CameraEye = XMFLOAT4(_camera.eye().x, _camera.eye().y, _camera.eye().z, 0.0f);
	cbf.DirLight = _directionalLights[0];
	cbf.PointLight = _activePointLight;
	cbf.Misc.x = 0; // shadow mapping? 0 -> no | 1 -> yes
	cbf.Misc.y = 0; // wall texture? 0 -> no | 1 -> yes

	if (_shadowMode == ShadowMode::Mapping)
		cbf.Misc.x = 1;

	_d3dDC->UpdateSubresource(_pCBPerFrame, 0, nullptr, &cbf, 0, 0);

	CBSpotLights cbsl;
	cbsl.numSpotLight = _numSpotLights;
	for (int i = 0; i < _spotLights.size(); ++i)
		cbsl.SpotLight[i] = _spotLights[i];
	_d3dDC->UpdateSubresource(_pCBSpotLights, 0, nullptr, &cbsl, 0, 0);


	_d3dDC->VSSetShader(_pCurrentVS, nullptr, 0);
	_d3dDC->VSSetConstantBuffers(0, 1, &_pCBOnResize.p);
	_d3dDC->VSSetConstantBuffers(1, 1, &_pCBPerFrame.p);
	_d3dDC->VSSetConstantBuffers(2, 1, &_pCBPerObject.p);
	_d3dDC->VSSetConstantBuffers(3, 1, &_pCBSpotLights.p);

	_d3dDC->PSSetShader(_pCurrentPS, nullptr, 0);
	_d3dDC->PSSetConstantBuffers(0, 1, &_pCBOnResize.p);
	_d3dDC->PSSetConstantBuffers(1, 1, &_pCBPerFrame.p);
	_d3dDC->PSSetConstantBuffers(2, 1, &_pCBPerObject.p);
	_d3dDC->PSSetConstantBuffers(3, 1, &_pCBSpotLights.p);

	const auto shadowMapSRV = _shadowMap->srv();
	const auto shadowMapSampler = RenderStates::sampleStateLinear();
	_d3dDC->PSSetShaderResources(0, 1, &shadowMapSRV);
	_d3dDC->PSSetSamplers(0, 1, &shadowMapSampler);
	
	renderObject(_activeExhibit);

	renderObject(&_plinth);

	renderObject(&_floor);

	const auto wallSampler = RenderStates::sampleStateLinear();
	_d3dDC->PSSetSamplers(1, 1, &wallSampler);
	_d3dDC->PSSetShaderResources(1, 1, &_wallTexture_SRV.p);
	_d3dDC->PSSetShaderResources(2, 1, &_wallNormalMapTexture_SRV.p);

	if (_turnWallTransparent)
	{
		std::vector<float> bf{ 0.f, 0.f, 0.f, 0.f };
		_d3dDC->OMSetBlendState(RenderStates::additiveBlending(), bf.data(), 0xFFFFFFFF);

		renderObject(&_ceiling);

		for (const auto& obj : _walls)
			renderObject(&obj);

		std::vector<float> bf2{ 1.f, 1.f, 1.f, 1.f };
		_d3dDC->OMSetBlendState(nullptr, bf2.data(), 0xFFFFFFFF);
	}
	else
	{
		renderObject(&_ceiling);

		cbf.Misc.y = 1; // activate wall texturing
		_d3dDC->UpdateSubresource(_pCBPerFrame, 0, nullptr, &cbf, 0, 0);

		for (const auto& obj : _walls)
			renderObject(&obj);

		cbf.Misc.y = 0; // deactivate wall texturing
		_d3dDC->UpdateSubresource(_pCBPerFrame, 0, nullptr, &cbf, 0, 0);
	}
}


void D3DApp::render()
{
	const auto vb = _meshManager->vb();
	const auto ib = _meshManager->ib();

	const auto stride = static_cast<UINT>(sizeof(Vertex));
	const auto offset = static_cast<UINT>(0);
	_d3dDC->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	_d3dDC->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
	_d3dDC->IASetInputLayout(InputLayouts::basic());
	_d3dDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (_shadowMode == ShadowMode::Mapping)
		_shadowMap->render(_d3dDC);

	renderScene();

	if (_shadowMode == ShadowMode::Planar)
		_planarShadow->render(_d3dDC);

	if (_dustEnabled)
	{
		const XMMATRIX view = _camera.view();
		const XMMATRIX proj = _camera.projection();
		_particleSys->render(_d3dDC, view, proj);
	}

	_swapChain->Present(0, 0);
}


void D3DApp::renderObject(const GameObject * const go)
{
	CBPerObject cbo;
	const Transform& transform = go->transform();
	XMMATRIX translation = XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z);
	XMMATRIX rotationX = XMMatrixRotationX(transform.rotation.x);
	XMMATRIX rotationY = XMMatrixRotationY(transform.rotation.y);
	XMMATRIX rotationZ = XMMatrixRotationZ(transform.rotation.z);
	XMMATRIX scale = XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);

	const XMMATRIX world = scale * rotationX * rotationY * rotationZ * translation;

	cbo.World = XMMatrixTranspose(world);
	cbo.Material = go->material();
	_d3dDC->UpdateSubresource(_pCBPerObject, 0, nullptr, &cbo, 0, 0);

	_d3dDC->DrawIndexed(go->mesh().IndexCount, go->mesh().IndexStartLocation, go->mesh().BaseVextexLocation);
}

