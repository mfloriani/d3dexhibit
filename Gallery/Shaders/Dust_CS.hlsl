static float3 g_accel = { 0.0f, -9.8f, 0.0f };

cbuffer cbCS : register(b0)
{
    uint4   g_param;    // pcbCS->param[0] = MAX_PARTICLES;
                        // pcbCS->param[1] = dimx;              
    float4  g_paramf;   // pcbCS->paramf[0] = 0.1f;
                        // pcbCS->paramf[1] = 1; 
};

struct Particle
{
    float4 startPos;
    float4 pos;
    float4 vel;
    float  startTime;
    float  time;
};

StructuredBuffer<Particle>   oldParticle;
RWStructuredBuffer<Particle> newParticle;

[numthreads(512, 1, 1)]
void CS(uint3 Gid : SV_GroupID, uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI : SV_GroupIndex)
{
    float4 pos = oldParticle[DTid.x].pos;
    float4 vel = oldParticle[DTid.x].vel;
    float time = oldParticle[DTid.x].time;
    
    vel.xyz = g_accel * g_paramf.x;       //deltaTime;
    vel.xyz *= g_paramf.y;                //damping;
    pos.xyz += vel.xyz * g_paramf.x;      //deltaTime;    

    time -= g_paramf.x;

    if (DTid.x < g_param.x)
    {
        if (time > 0)
        {
            newParticle[DTid.x].startPos = oldParticle[DTid.x].startPos;
            newParticle[DTid.x].pos = pos;
            newParticle[DTid.x].vel = float4(vel.xyz, 0);
            newParticle[DTid.x].startTime = oldParticle[DTid.x].startTime;
            newParticle[DTid.x].time = time;
        }
        else
        {
            newParticle[DTid.x].startPos = oldParticle[DTid.x].startPos;
            newParticle[DTid.x].pos = oldParticle[DTid.x].startPos;
            newParticle[DTid.x].vel = 0;
            newParticle[DTid.x].startTime = oldParticle[DTid.x].startTime;
            newParticle[DTid.x].time = oldParticle[DTid.x].startTime;
        }
    }
}
