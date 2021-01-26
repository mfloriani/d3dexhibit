#include "LightFX.fx"

Texture2D ShadowMapTex : register(t0);
Texture2D WallTex : register(t1);
Texture2D WallNormalMapTex : register(t2);

SamplerState ShadowMapSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

SamplerState WallSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

cbuffer cbOnResize : register(b0)
{
    matrix gProjection;
    matrix gLightProj;
};

cbuffer cbPerFrame : register(b1)
{
    matrix           gView;
    float4           gCamEye;
    DirectionalLight gDirectLight;
    PointLight       gPointLight;
    float            gCompShadowMap;
    float            gWallTexturing;
    float            pad2[2];
};

cbuffer cbPerObject : register(b2)
{
    matrix   gWorld;
    Material gMaterial;
};

cbuffer cbSpotLight : register(b3)
{
    float     gNumSpotLights;
    float3    pad;
    SpotLight gSpotLight[MAX_SPOTLIGHTS];
};

struct VS_IN
{
    float4 Pos    : POSITION;
    float3 Normal : NORMAL;
    float3 Tang   : TANGENT;
    float2 Tex    : TEXCOORD;
};

struct VS_OUT
{
    float4 Pos   : SV_POSITION;
    float4 Color : COLOR;
    float2 Tex   : TEXCOORD;
};

VS_OUT VS(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    float4 posW = mul(input.Pos, gWorld);
    posW.w = 1.0f;

    output.Pos = posW;
    output.Pos = mul(output.Pos, gView);
    output.Pos = mul(output.Pos, gProjection);

    float3 normal = normalize(mul(input.Normal, gWorld)).xyz;
    float3 toCamEye = normalize(gCamEye - posW);

    float4 posLightSpace = ComputeLightViewSpace(gDirectLight.Position, gLightProj, posW);

    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float4 amb, diff, spec;

    ComputeDirectionalLight(gMaterial, gDirectLight, normal, toCamEye, amb, diff, spec);
    ambient += amb;
    diffuse += diff;
    specular += spec;

    ComputePointLight(gMaterial, gPointLight, posW, normal, toCamEye, amb, diff, spec);
    ambient += amb;
    diffuse += diff;
    specular += spec;

    for (int i = 0; i < gNumSpotLights; ++i)
    {
        ComputeSpotLight(gMaterial, gSpotLight[i], posW, normal, toCamEye, amb, diff, spec);
        ambient += amb;
        diffuse += diff;
        specular += spec;
    }

    output.Color = ambient + diffuse + specular;
    output.Color.a = gMaterial.Diffuse.a;
    output.Tex = input.Tex;

    return output;
}

float4 PS(VS_OUT input) : SV_Target
{
    clip(gMaterial.Diffuse.a < 0.1f ? -1 : 1);

    if (gWallTexturing)
        return input.Color * WallTex.Sample(WallSampler, input.Tex);

    return input.Color;
}
