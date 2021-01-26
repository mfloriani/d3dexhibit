#include "ParticleSystem.h"
#include "Vertex.h"
#include "RenderStates.h"

#pragma comment( lib, "dxguid.lib")
#include "DDSTextureLoader.h"

#include <vector>
#include <array>

using namespace DirectX;


static float RandF()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}
// Returns random float in [a, b).
static float RandF(const float a, const float b)
{
    return a + RandF() * (b - a);
}

XMVECTOR RandUnitVec3()
{
    const XMVECTOR One = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);    
    // Keep trying until we get a point on/in the hemisphere.
    while (true)
    {
        // Generate random point in the cube [-1,1]^3.
        const XMVECTOR v = XMVectorSet(
            RandF(-1.0f, 1.0f),
            RandF(-1.0f, 1.0f),
            RandF(-1.0f, 1.0f), 0.0f);
        // Ignore points outside the unit sphere in order to
        // get an even distribution over the unit sphere. Otherwise
        // points will clump more on the sphere near the corners
        // of the cube.
        if (XMVector3Greater(XMVector3LengthSq(v), One))
            continue;
        return XMVector3Normalize(v);
    }
}



ParticleSystem::ParticleSystem(
    ID3D11ComputeShader * const cs, 
    ID3D11VertexShader * const vs, 
    ID3D11GeometryShader * const gs, 
    ID3D11PixelShader * const ps)
    : _cs(cs), _vs(vs), _gs(gs), _ps(ps)
{
    
}

ParticleSystem::~ParticleSystem()
{
}

HRESULT ParticleSystem::init(ID3D11Device* const device)
{
	HRESULT hr = 0;

    D3D11_BUFFER_DESC Desc;
    Desc.Usage = D3D11_USAGE_DYNAMIC;
    Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    Desc.MiscFlags = 0;
    
    Desc.ByteWidth = sizeof(CBuffer_GS);
    hr = device->CreateBuffer(&Desc, nullptr, &_cbufferGS);
    if (FAILED(hr)) return hr;

    Desc.ByteWidth = sizeof(CBuffer_CS);
    hr = device->CreateBuffer(&Desc, nullptr, &_cbufferCS);
    if (FAILED(hr)) return hr;

    D3D11_BUFFER_DESC desc = {};
    desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
    desc.ByteWidth = MAX_PARTICLES * sizeof(Particle);
    desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    desc.StructureByteStride = sizeof(Particle);
    desc.Usage = D3D11_USAGE_DEFAULT;

    // Initialize the data in the buffers
    std::vector<Particle> data(MAX_PARTICLES);
    //Particle * const data = new Particle[MAX_PARTICLES];
    
    for (int i = 0; i < MAX_PARTICLES; ++i)
    {
        const float x = RandF(-10, 10);
        const float y = RandF(-2, 2);
        const float z = RandF(-10, 10);
        const float t = RandF(50, 150);

        data[i].position.x = x;
        data[i].position.y = 20.f + y;
        data[i].position.z = z;

        data[i].startPos = data[i].position;

        data[i].velocity.x = 0;
        data[i].velocity.y = 0;
        data[i].velocity.z = 0;

        data[i].time = t;
        data[i].startTime = t;
    }
    
    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = data.data();
    hr = device->CreateBuffer(&desc, &InitData, &_bufferA);
    if (FAILED(hr)) return hr;
    hr = device->CreateBuffer(&desc, &InitData, &_bufferB);
    if (FAILED(hr)) return hr;
    
    //delete[] data;

    D3D11_SHADER_RESOURCE_VIEW_DESC DescRV = {};
    DescRV.Format = DXGI_FORMAT_UNKNOWN;
    DescRV.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    DescRV.Buffer.FirstElement = 0;
    DescRV.Buffer.NumElements = desc.ByteWidth / desc.StructureByteStride;
    hr = device->CreateShaderResourceView(_bufferA, &DescRV, &_bufferA_SRV);
    if (FAILED(hr)) return hr;
    hr = device->CreateShaderResourceView(_bufferB, &DescRV, &_bufferB_SRV);
    if (FAILED(hr)) return hr;
    
    D3D11_UNORDERED_ACCESS_VIEW_DESC DescUAV = {};
    DescUAV.Format = DXGI_FORMAT_UNKNOWN;
    DescUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    DescUAV.Buffer.FirstElement = 0;
    DescUAV.Buffer.NumElements = desc.ByteWidth / desc.StructureByteStride;
    hr = device->CreateUnorderedAccessView(_bufferA, &DescUAV, &_bufferA_UAV);
    if (FAILED(hr)) return hr;
    hr = device->CreateUnorderedAccessView(_bufferB, &DescUAV, &_bufferB_UAV);
    if (FAILED(hr)) return hr;

    // Vertex Buffer

    D3D11_SUBRESOURCE_DATA vbInitData = {};

    std::vector<ParticleVertex> vertices(MAX_PARTICLES);
    //ParticleVertex * const vertices = new ParticleVertex[MAX_PARTICLES];
    
    for (UINT i = 0; i < MAX_PARTICLES; i++)
        vertices[i].Color = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

    vbInitData.pSysMem = vertices.data();

    const D3D11_BUFFER_DESC vbdesc =
    {
        MAX_PARTICLES * sizeof(ParticleVertex),
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_VERTEX_BUFFER,
        0,
        0
    };

    hr = device->CreateBuffer(&vbdesc, &vbInitData, &_vertexBuffer);
    if (FAILED(hr)) return hr;
    
    //delete[] vertices;

    // Dust texture

    hr = CreateDDSTextureFromFile(device, L"Assets/particle_dust.dds", nullptr, &_texture_SRV);
    if (FAILED(hr)) return hr;
    
	return hr;
}


void ParticleSystem::update(ID3D11DeviceContext* const dc, const float dt)
{
	const int blockSize = static_cast<int>(ceil(MAX_PARTICLES / 512.0f));

	dc->CSSetShader(_cs, nullptr, 0);

    // CS Input
	std::array<ID3D11ShaderResourceView* const, 1> aRViews { _bufferA_SRV };
	dc->CSSetShaderResources(0, 1, aRViews.data());

    // CS Output
    std::array<ID3D11UnorderedAccessView*,1> aUAViews { _bufferB_UAV };
    dc->CSSetUnorderedAccessViews(0, 1, aUAViews.data(), nullptr);

    // CS constant buffer
    D3D11_MAPPED_SUBRESOURCE MappedResource;
    dc->Map(_cbufferCS, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
    const auto pcbCS = reinterpret_cast<CBuffer_CS*>(MappedResource.pData);
    pcbCS->param[0] = MAX_PARTICLES;
    pcbCS->param[1] = blockSize;
    pcbCS->paramf[0] = dt * 10.f;// 0.016f;
    pcbCS->paramf[1] = 1;
    dc->Unmap(_cbufferCS, 0);
    std::array<ID3D11Buffer* const,1>  ppCB { _cbufferCS };
    dc->CSSetConstantBuffers(0, 1, ppCB.data());

    // Run CS
    dc->Dispatch(blockSize, 1, 1);

    // Unbind resources for CS
    std::array<ID3D11UnorderedAccessView* const,1>  ppUAViewNULL { nullptr };
    dc->CSSetUnorderedAccessViews(0, 1, ppUAViewNULL.data(), nullptr);
    std::array<ID3D11ShaderResourceView* const, 1> ppSRVNULL { nullptr };
    dc->CSSetShaderResources(0, 1, ppSRVNULL.data());
    dc->CSSetShader( nullptr, nullptr, 0 ); // was initially commented...

    std::swap(_bufferA, _bufferB);
    std::swap(_bufferA_SRV, _bufferB_SRV);
    std::swap(_bufferA_UAV, _bufferB_UAV);
}

void ParticleSystem::render(ID3D11DeviceContext* const dc, const XMMATRIX& view, const XMMATRIX& proj) const
{
    CComPtr<ID3D11BlendState> pBlendState0 = nullptr;
    CComPtr<ID3D11DepthStencilState> pDepthStencilState0 = nullptr;
    UINT SampleMask0, StencilRef0;
    std::array<float,4> BlendFactor0;
    dc->OMGetBlendState(&pBlendState0, BlendFactor0.data(), &SampleMask0);
    dc->OMGetDepthStencilState(&pDepthStencilState0, &StencilRef0);

    dc->VSSetShader(_vs, nullptr, 0);
    dc->GSSetShader(_gs, nullptr, 0);
    dc->PSSetShader(_ps, nullptr, 0);
    
    dc->IASetInputLayout(InputLayouts::particle());

    // Set IA parameters
    std::array<ID3D11Buffer*, 1> pBuffers { _vertexBuffer };
    std::array<UINT,1> stride { sizeof(ParticleVertex) };
    std::array<UINT,1> offset { 0 };
    dc->IASetVertexBuffers(0, 1, pBuffers.data(), stride.data(), offset.data());
    dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

    std::array<ID3D11ShaderResourceView*,1> aRViews { _bufferA_SRV };
    dc->VSSetShaderResources(0, 1, aRViews.data());

    D3D11_MAPPED_SUBRESOURCE MappedResource;
    dc->Map(_cbufferGS, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
    auto cbufferGS = reinterpret_cast<CBuffer_GS*>(MappedResource.pData);
    XMStoreFloat4x4(&cbufferGS->WorldViewProj, XMMatrixMultiply(view, proj));
    XMStoreFloat4x4(&cbufferGS->InvView, XMMatrixInverse(nullptr, view));
    dc->Unmap(_cbufferGS, 0);
    dc->GSSetConstantBuffers(0, 1, &_cbufferGS.p);

    dc->PSSetShaderResources(0, 1, &_texture_SRV.p);
    const auto sampler = RenderStates::sampleStateLinear();
    dc->PSSetSamplers(0, 1, &sampler);

    std::vector<float> bf{ 0.f, 0.f, 0.f, 0.f };
    dc->OMSetBlendState(RenderStates::additiveBlending(), bf.data(), 0xFFFFFFFF);
    dc->OMSetDepthStencilState(RenderStates::noDepthWrites(), 0);

    dc->Draw(MAX_PARTICLES, 0);

    std::array<ID3D11ShaderResourceView*,1> ppSRVNULL { nullptr };
    dc->VSSetShaderResources(0, 1, ppSRVNULL.data());
    dc->PSSetShaderResources(0, 1, ppSRVNULL.data());

    dc->GSSetShader(nullptr, nullptr, 0);

    dc->OMSetBlendState(pBlendState0, BlendFactor0.data(), SampleMask0); 
    dc->OMSetDepthStencilState(pDepthStencilState0, StencilRef0); 
}


