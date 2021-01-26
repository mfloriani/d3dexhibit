#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <atlbase.h>
#include <unordered_map>
#include <string>

class ShaderManager
{
public:
	ShaderManager() = default;

	HRESULT createVertexShader(
		ID3D11Device* const device, 
		const std::string& filename, 
		const std::string& entryPoint, 
		std::vector<unsigned char>& byteCodeOut
	);
	HRESULT createGeometryShader(ID3D11Device* const device, const std::string& filename, const std::string& entryPoint);
	HRESULT createPixelShader(ID3D11Device* const device, const std::string& filename, const std::string& entryPoint);
	HRESULT createComputeShader(ID3D11Device* const device, const std::string& filename, const std::string& entryPoint);

	ID3D11VertexShader*   vs(const std::string& id);
	ID3D11GeometryShader* gs(const std::string& id);
	ID3D11PixelShader*    ps(const std::string& id);
	ID3D11ComputeShader*  cs(const std::string& id);

private:
	std::unordered_map<std::string, CComPtr<ID3D11VertexShader>>   _vertexShaders;
	std::unordered_map<std::string, CComPtr<ID3D11GeometryShader>> _geometryShaders;
	std::unordered_map<std::string, CComPtr<ID3D11PixelShader>>    _pixelShaders;
	std::unordered_map<std::string, CComPtr<ID3D11ComputeShader>>  _computeShaders;

	bool _isInputLayoutCreated{ false };
};