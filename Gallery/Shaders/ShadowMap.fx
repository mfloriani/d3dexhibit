#include "LightFX.fx"

struct VS_IN
{
	float4 Pos    : POSITION;
	float3 Normal : NORMAL;
	float3 Tang   : TANGENT;
	float2 Tex    : TEXCOORD;
};

struct VS_OUT
{
	float4 Pos      : SV_POSITION;
	float3 LightVec : TEXCOORD;
};

cbuffer WorldViewProjBuffer : register(b0)
{
	matrix gWorld;
	matrix gLightProj;
	float3 gLightPos;
}

#define distanceScale 0.00148

VS_OUT VS(VS_IN input)
{
	VS_OUT output;

	input.Pos.w = 1.0f;
	float4 posW = mul(input.Pos, gWorld);

	output.Pos = ComputeLightViewSpace(gLightPos, gLightProj, posW);
	output.LightVec = distanceScale * posW;

	return output;
}

float4 PS(VS_OUT input) : SV_TARGET
{
	float d = length(input.LightVec);
	return float4(d, d, d, 1);
}