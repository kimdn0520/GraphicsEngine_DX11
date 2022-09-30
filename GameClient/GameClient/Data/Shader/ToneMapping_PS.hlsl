#include "LightHeader.hlsli"
#include "ConstantBufferHeader.hlsli"

static const float gamma = 2.2f;
static const float pureWhite = 1.f;

struct TonePixelIn
{
	float4 pos		   : SV_POSITION;
	float3 posW		   : POSITION;
	float4 color	   : COLOR;
	float2 uv		   : UV;
	float3 normal	   : NORMAL;
	float3 tangent	   : TANGENT;
};

Texture2D Final_Texture : register(t0);

SamplerState samLinearClamp : register(s0);

float4 ToneMapping_PS(TonePixelIn input) : SV_Target
{
	float4 color = (float)0.f;

	color = Final_Texture.Sample(samLinearClamp, input.uv) * exposure;

	float luminance = dot(color.xyz, float3(0.2126f, 0.7152f, 0.0722f));

	float mappedLuminance = (luminance * (1.0f + luminance / (pureWhite * pureWhite))) / (1.0f + luminance);

	float3 mappedColor = (mappedLuminance / luminance) * color.xyz;

	// 마지막에 감마 연산 필요
	color = float4(pow(mappedColor, (1 / gamma)), 1.f);

	return color;
}