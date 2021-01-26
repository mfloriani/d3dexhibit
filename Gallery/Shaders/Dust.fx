struct VS_IN
{
    float4 color : COLOR;
    uint   id    : SV_VERTEXID;
};

struct VS_OUT
{
    float3 pos	 : POSITION;
    float4 color : COLOR;
    //float  time  : TEXCOORD;
};               
                 
struct GS_OUT    
{                
    float2 tex	 : TEXCOORD0;
    float4 color : COLOR;
    float4 pos	 : SV_POSITION;
    //float  time  : TEXCOORD1;
};               

struct PS_IN     
{                
    float2 tex	 : TEXCOORD0;
    float4 color : COLOR;
    //float  time  : TEXCOORD1;
};

struct Particle
{
    float4 startPos;
    float4 pos;
    float4 vel;
    float  startTime;
    float  time;
};

Texture2D		            g_txDiffuse;
StructuredBuffer<Particle>  g_particleBuffer;


SamplerState g_samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

cbuffer cb0
{
    row_major float4x4 g_mWorldViewProj;
    row_major float4x4 g_mInvView;
};

cbuffer cb1
{
    static float g_fParticleRad = 0.01f;
};

cbuffer cbImmutable
{
    static float3 g_positions[4] =
    {
        float3(-1, 1, 0),
        float3(1, 1, 0),
        float3(-1, -1, 0),
        float3(1, -1, 0),
    };

    static float2 g_texcoords[4] =
    {
        float2(0,0),
        float2(1,0),
        float2(0,1),
        float2(1,1),
    };
};

//
// Vertex shader for drawing the point-sprite particles
//
VS_OUT VS(VS_IN input)
{
    VS_OUT output;

    //output.time = g_particleBuffer[input.id].time;
    output.pos = g_particleBuffer[input.id].pos.xyz;
    float mag = g_particleBuffer[input.id].vel.w / 9;
    output.color = lerp(float4(1, 1, 1, 1), input.color, mag);

    return output;
}

//
// GS for rendering point sprite particles.  Takes a point and turns it into 2 tris.
//
[maxvertexcount(4)]
void GS(point VS_OUT input[1], inout TriangleStream<GS_OUT> SpriteStream)
{
    GS_OUT output;

    //
    // Emit two new triangles
    //
    for (int i = 0; i < 4; i++)
    {
        float3 position = g_positions[i] * g_fParticleRad;
        position = mul(position, (float3x3)g_mInvView) + input[0].pos;
        output.pos = mul(float4(position, 1.0), g_mWorldViewProj);

        output.color = input[0].color;
        output.tex   = g_texcoords[i];
        //output.time  = input[0].time;
        SpriteStream.Append(output);
    }
    SpriteStream.RestartStrip();
}

//
// PS for drawing particles
//
float4 PS(PS_IN input) : SV_Target
{
    //if (input.time < 0.1)
    //    discard;
    return g_txDiffuse.Sample(g_samLinear, input.tex) * input.color;
}