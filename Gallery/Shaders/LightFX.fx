#define MAX_SPOTLIGHTS 4

struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
};

struct DirectionalLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Direction;
    float  Pad;
    float3 Position;
    float  Pad2;
};

struct PointLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Position;
    float  Range;
    float3 Attenuation;
    float  Pad;
};

struct SpotLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Position;
    float  Range;
    float3 Direction;
    float  Spot;
    float3 Attenuation;
    float  Pad;
};

void ComputeDirectionalLight(
    Material mat, DirectionalLight light, float3 normal, float3 toCamEye,
    out float4 ambient, out float4 diffuse, out float4 specular
)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    ambient = mat.Ambient * light.Ambient;

    float3 lightDir = normalize(-light.Direction);

    float diffFactor = dot(lightDir, normal);

    [flatten]
    if (diffFactor > 0.0f)
    {
        float3 r = reflect(-lightDir, normal);
        float specFactor = pow(max(dot(r, toCamEye), 0.0f), mat.Specular.w);

        diffuse = diffFactor * mat.Diffuse * light.Diffuse;
        specular = specFactor * mat.Specular * light.Specular;
    }
}

void ComputePointLight(
    Material mat, PointLight light, float3 pos, float3 normal, float3 toCamEye,
    out float4 ambient, out float4 diffuse, out float4 specular
)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float3 lightDir = light.Position - pos;
    float d = length(lightDir);

    if (d > light.Range)
        return;

    lightDir /= d;

    ambient = mat.Ambient * light.Ambient;

    float diffFactor = dot(lightDir, normal);

    [flatten]
    if (diffFactor > 0.0f)
    {
        float3 r = reflect(-lightDir, normal);
        float specFactor = pow(max(dot(r, toCamEye), 0.0f), mat.Specular.w);

        diffuse = diffFactor * mat.Diffuse * light.Diffuse;
        specular = specFactor * mat.Specular * light.Specular;
    }

    float att = 1.0f / dot(light.Attenuation, float3(1.0f, d, d * d));

    ambient *= att;
    diffuse *= att;
    specular *= att;
}

void ComputeSpotLight(
    Material mat, SpotLight light, float3 pos, float3 normal, float3 toCamEye,
    out float4 ambient, out float4 diffuse, out float4 specular
)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float3 lightDir = light.Position - pos;
    float d = length(lightDir);

    if (d > light.Range)
        return;

    lightDir /= d;

    ambient = mat.Ambient * light.Ambient;

    float diffFactor = dot(lightDir, normal);

    [flatten]
    if (diffFactor > 0.0f)
    {
        float3 r = reflect(-lightDir, normal);
        float specFactor = pow(max(dot(r, toCamEye), 0.0f), mat.Specular.w);

        diffuse = diffFactor * mat.Diffuse * light.Diffuse;
        specular = specFactor * mat.Specular * light.Specular;
    }

    float spot = pow(max(dot(-lightDir, light.Direction), 0.0f), light.Spot);

    float att = spot / dot(light.Attenuation, float3(1.0f, d, d * d));

    ambient *= spot;
    diffuse *= att;
    specular *= att;
}

#define distanceScale 0.00148

float4 ComputeLightViewSpace(float3 lightPos, matrix lightProj, float4 posW)
{
    float3 dirZ = -normalize(lightPos);
    float3 up = float3(0, 0, 1);
    float3 dirX = cross(up, dirZ);
    float3 dirY = cross(dirZ, dirX);

    posW.xyz -= lightPos;

    float4 pos;
    pos.x = dot(dirX, posW);
    pos.y = dot(dirY, posW);
    pos.z = dot(dirZ, posW);
    pos.w = 1;

    return mul(lightProj, pos);
}

float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
    float3 normalT = 2.0f * normalMapSample - 1.0f;

    float3 N = unitNormalW;
    float3 T = normalize(tangentW - dot(tangentW, N) * N);
    float3 B = cross(N, T);

    float3x3 TBN = float3x3(T, B, N);

    float3 bumpedNormalW = mul(normalT, TBN);

    return bumpedNormalW;
}