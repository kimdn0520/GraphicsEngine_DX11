#include "LightPBRHeader.hlsli"
#include "ConstantBufferHeader.hlsli"

#define BLUR_SAMPLE_COUNT 9

static const float g_Weights[BLUR_SAMPLE_COUNT] =
{
        0.013519569015984728,
        0.047662179108871855,
        0.11723004402070096,
        0.20116755999375591,
        0.240841295721373,
        0.20116755999375591,
        0.11723004402070096,
        0.047662179108871855,
        0.013519569015984728
};

static const float g_Indices[BLUR_SAMPLE_COUNT] = { -4, -3, -2, -1, 0, +1, +2, +3, +4 };

struct BlurPixelIN
{
	float4 pos		   : SV_POSITION;
	float3 posW		   : POSITION;
	float4 color	   : COLOR;
	float2 uv		   : UV;
	float3 normal	   : NORMAL;
	float3 tangent	   : TANGENT;
};

Texture2D TargetTexture : register(t0);

SamplerState samLinearClamp : register(s0);

float4 BlurX_PS(BlurPixelIN input) : SV_Target
{
    float2 dir = float2(1.0f, 0.f);

    float2 step = float2(dir.x * textureInfo.z, dir.y * textureInfo.w);

    float4 color = (float4) 0;

    for (int i = 0; i < BLUR_SAMPLE_COUNT; i++)
    {
        color += TargetTexture.Sample(samLinearClamp, input.uv + g_Indices[i] * step) * g_Weights[i];
    }

    return float4(color.xyz, 1.f);
}

float4 BlurY_PS(BlurPixelIN input) : SV_TARGET
{
    float2 dir = float2(0.f, 1.f);

    float2 step = float2(dir.x * textureInfo.z, dir.y * textureInfo.w);

    float4 color = (float4) 0;

    for (int i = 0; i < BLUR_SAMPLE_COUNT; i++)
    {
        color += TargetTexture.Sample(samLinearClamp, input.uv + g_Indices[i] * step) * g_Weights[i];
    }

    return float4(color.xyz, 1.f);
}