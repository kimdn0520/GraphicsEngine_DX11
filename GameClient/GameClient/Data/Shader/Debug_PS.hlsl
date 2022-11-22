#define OUT_BUFFER_COUNT 6

static const float xStep = 1.f / OUT_BUFFER_COUNT;

struct DebugPixelIN
{
	float4 pos     : SV_POSITION;
	float3 posW	   : POSITION;
	float4 color   : COLOR;
	float2 uv	   : UV;
	float3 normal  : NORMAL;
	float3 tangent : TANGENT;
};

Texture2D Albedo : register(t0);

Texture2D Depth : register(t1);

Texture2D Normal : register(t2);

Texture2D Position : register(t3);

Texture2D Shadow : register(t4);

Texture2D Emissive : register(t5);

SamplerState samLinearClamp : register(s0);

float4 Debug_PS(DebugPixelIN input) : SV_Target
{
    // 1. Depth
    if ((input.uv.x >= 0.f) && (input.uv.x <= xStep))
    {
        return float4(Depth.Sample(samLinearClamp, input.uv * float2(OUT_BUFFER_COUNT, 1.f)).xxx, 1.f);
    }
    // 2. Normal
    else if ((input.uv.x >= 1 * xStep) && (input.uv.x <= 2 * xStep))
    {
        return float4(Normal.Sample(samLinearClamp, (input.uv - float2(xStep, 0.f)) * float2(OUT_BUFFER_COUNT, 1.f)).xyz, 1.f);
    }
    // 3. Position
    else if (((input.uv.x >= 2 * xStep) && (input.uv.x <= 3 * xStep)))
    {
        return float4(Position.Sample(samLinearClamp, (input.uv - float2(2 * xStep, 0.f)) * float2(OUT_BUFFER_COUNT, 1.f)).xyz, 1.f);
    }
    // 4. Albedo
    else if (((input.uv.x >= 3 * xStep) && (input.uv.x <= 4 * xStep)))
    {
        return float4(pow(Albedo.Sample(samLinearClamp, (input.uv - float2(3 * xStep, 0.f)) * float2(OUT_BUFFER_COUNT, 1.f)).xyz, 1 / 2.2f), 1.f);
    }
    // 5. Shadow
    else if (((input.uv.x >= 4 * xStep) && (input.uv.x <= 5 * xStep)))
    {
        return float4(Shadow.Sample(samLinearClamp, (input.uv - float2(4 * xStep, 0.f)) * float2(OUT_BUFFER_COUNT, 1.f)).xyz, 1.f);
    }
    // 6. Emissive
    else if (((input.uv.x >= 5 * xStep) && (input.uv.x <= 6 * xStep)))
    {
        return float4(Emissive.Sample(samLinearClamp, (input.uv - float2(5 * xStep, 0.f)) * float2(OUT_BUFFER_COUNT, 1.f)).xyz, 1.f);
    }

    return float4(0.f, 0.f, 0.f, 1.f);
}