#include "ShaderManager.h"
#include "Vertex.h"

#include <fstream>

HRESULT ShaderManager::createVertexShader(
	ID3D11Device* const device, const std::string& filename, const std::string& entryPoint, std::vector<unsigned char>& byteCodeOut)
{
	auto hr = 0;
	std::string id(filename + "_" + entryPoint);
	std::string fileName{ "..\\x64\\Debug\\" + id + ".cso" };
	std::ifstream fin(fileName, std::ios::binary);
	if (!fin) 
	{
		auto tmp = ("Failed to load " + fileName);
		const auto tmp2 = std::wstring(tmp.begin(), tmp.end());
		const wchar_t* const message = tmp2.c_str();
		MessageBox(nullptr, message, L"Error", MB_OK);
		return -1;
	}
	
	std::vector<unsigned char> byteCode(std::istreambuf_iterator<char>(fin), {});

	CComPtr<ID3D11VertexShader> vs;
	hr = device->CreateVertexShader(&byteCode[0], byteCode.size(), nullptr, &vs.p);
	if (FAILED(hr)) {
		return hr;
	}
	byteCodeOut = byteCode;

	std::pair<std::string, CComPtr<ID3D11VertexShader>> shader = std::make_pair(id, vs);
	_vertexShaders.insert(shader);

	
	return hr;
}

HRESULT ShaderManager::createGeometryShader(ID3D11Device* const device, const std::string& filename, const std::string& entryPoint)
{
	auto hr = 0;
	std::string id(filename + "_" + entryPoint);
	std::string fileName{ "..\\x64\\Debug\\" + id + ".cso" };
	std::ifstream fin(fileName, std::ios::binary);
	if (!fin)
	{
		auto tmp = ("Failed to load " + fileName);
		const auto tmp2 = std::wstring(tmp.begin(), tmp.end());
		const wchar_t* const message = tmp2.c_str();
		MessageBox(nullptr, message, L"Error", MB_OK);
		return -1;
	}
	std::vector<unsigned char> byteCode(std::istreambuf_iterator<char>(fin), {});

	CComPtr<ID3D11GeometryShader> gs;
	hr = device->CreateGeometryShader(&byteCode[0], byteCode.size(), nullptr, &gs.p);
	if (FAILED(hr)) {
		return hr;
	}

	std::pair<std::string, CComPtr<ID3D11GeometryShader>> shader = std::make_pair(id, gs);
	_geometryShaders.insert(shader);

	return hr;
}

HRESULT ShaderManager::createPixelShader(ID3D11Device* const device, const std::string& filename, const std::string& entryPoint)
{
	auto hr = 0;
	std::string id(filename + "_" + entryPoint);
	const std::string fileName{ "..\\x64\\Debug\\" + id + ".cso" };
	std::ifstream fin(fileName, std::ios::binary);
	if (!fin) 
	{
		auto tmp = ("Failed to load " + fileName);
		const auto tmp2 = std::wstring(tmp.begin(), tmp.end());
		const wchar_t* const message = tmp2.c_str();
		MessageBox(nullptr, message, L"Error", MB_OK);
		return -1;
	}
	std::vector<unsigned char> byteCode(std::istreambuf_iterator<char>(fin), {});

	CComPtr<ID3D11PixelShader> ps;
	hr = device->CreatePixelShader(&byteCode[0], byteCode.size(), nullptr, &ps.p);
	if (FAILED(hr)) return hr;

	std::pair<std::string, CComPtr<ID3D11PixelShader>> shader = std::make_pair(id, ps);
	_pixelShaders.insert(shader);

	return hr;
}

HRESULT ShaderManager::createComputeShader(ID3D11Device* const device, const std::string& filename, const std::string& entryPoint)
{
	auto hr = 0;
	std::string id(filename + "_" + entryPoint);
	std::string fileName{ "..\\x64\\Debug\\" + id + ".cso" };
	std::ifstream fin(fileName, std::ios::binary);
	if (!fin)
	{
		auto tmp = ("Failed to load " + fileName);
		const auto tmp2 = std::wstring(tmp.begin(), tmp.end());
		const wchar_t* const message = tmp2.c_str();
		MessageBox(nullptr, message, L"Error", MB_OK);
		return -1;
	}
	std::vector<unsigned char> byteCode(std::istreambuf_iterator<char>(fin), {});

	CComPtr<ID3D11ComputeShader> cs;
	hr = device->CreateComputeShader(&byteCode[0], byteCode.size(), nullptr, &cs.p);
	if (FAILED(hr)) {
		return hr;
	}

	std::pair<std::string, CComPtr<ID3D11ComputeShader>> shader = std::make_pair(id, cs);
	_computeShaders.insert(shader);

	return hr;
}

ID3D11VertexShader* ShaderManager::vs(const std::string& id)
{
	const auto it = _vertexShaders.find(id);
	if (it != _vertexShaders.end())
		return it->second.p;

	return nullptr;
}

ID3D11GeometryShader* ShaderManager::gs(const std::string& id)
{
	const auto it = _geometryShaders.find(id);
	if (it != _geometryShaders.end())
		return it->second.p;

	return nullptr;
}

ID3D11PixelShader* ShaderManager::ps(const std::string& id)
{
	const auto it = _pixelShaders.find(id);
	if (it != _pixelShaders.end())
		return it->second.p;

	return nullptr;
}

ID3D11ComputeShader* ShaderManager::cs(const std::string& id)
{
	const auto it = _computeShaders.find(id);
	if (it != _computeShaders.end())
		return it->second.p;

	return nullptr;
}
