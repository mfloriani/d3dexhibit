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
    float4 Pos           : SV_POSITION;
    float4 PosW          : POSITION;
    float3 NormalW       : NORMAL;
    float2 Tex           : TEXCOORD0;
    float4 PosLightSpace : TEXCOORD1;
    float3 TangW         : TANGENT;
};

VS_OUT VS( VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    float4 inPosW = mul(input.Pos, gWorld);
    inPosW.w = 1.0f;

    output.PosW = inPosW;
    output.Pos = inPosW;
    output.Pos = mul( output.Pos, gView );
    output.Pos = mul( output.Pos, gProjection );
    
    output.NormalW = normalize(mul(input.Normal, gWorld));
    output.Tex = input.Tex;

    output.PosLightSpace = ComputeLightViewSpace(gDirectLight.Position, gLightProj, inPosW);

    output.TangW = mul(input.Tang, (float3x3)gWorld);

    return output;
}

float ComputeShadow(float4 vertexPosLightSpace)
{
    float3 projCoords = vertexPosLightSpace.xyz / vertexPosLightSpace.w;
    projCoords.x = projCoords.x * 0.5 + 0.5;
    projCoords.y = -projCoords.y * 0.5 + 0.5;

    float shadowMap = ShadowMapTex.Sample(ShadowMapSampler, projCoords.xy).r;
    float depth = projCoords.z - 0.001;
    float shadow = depth < shadowMap;

    return shadow;
}

float4 PS(VS_OUT input) : SV_Target
{
    clip(gMaterial.Diffuse.a < 0.1f ? -1 : 1);

    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float3 normal = normalize(input.NormalW);
    float3 toCamEye = normalize(gCamEye - input.PosW);

    if (gWallTexturing)
    {
        diffuse = WallTex.Sample(WallSampler, input.Tex);
        float3 normalMapSample = WallNormalMapTex.Sample(WallSampler, input.Tex).rgb;
        normal = NormalSampleToWorldSpace(normalMapSample, normal, input.TangW);
    }

    float4 amb, diff, spec;

    ComputeDirectionalLight(gMaterial, gDirectLight, normal, toCamEye, amb, diff, spec);
    ambient += amb;
    diffuse += diff;
    specular += spec;

    float shadow = 1;

    if (gCompShadowMap)
        shadow = ComputeShadow(input.PosLightSpace);

    if (shadow == 1)
    {
        ComputePointLight(gMaterial, gPointLight, input.PosW, normal, toCamEye, amb, diff, spec);
        ambient += amb;
        diffuse += diff;
        specular += spec;

        for (int i = 0; i < gNumSpotLights; ++i)
        {
            ComputeSpotLight(gMaterial, gSpotLight[i], input.PosW, normal, toCamEye, amb, diff, spec);
            ambient += amb;
            diffuse += diff;
            specular += spec;
        }
    }
    

    float4 color = ambient + diffuse + specular;
    color.a = gMaterial.Diffuse.a;

    return color;
}

//----------------------------------------------------------
