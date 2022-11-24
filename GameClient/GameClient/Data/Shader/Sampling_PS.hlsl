
struct SamplingPixelIn
{
	float4 pos		   : SV_POSITION;
	float3 posW		   : POSITION;
	float4 color	   : COLOR;
	float2 uv		   : UV;
	float3 normal	   : NORMAL;
	float3 tangent	   : TANGENT;
};

Texture2D SampleTexture : register(t0);
SamplerState samLinearClamp : register(s0);

float4 Sampling_PS(SamplingPixelIn input) : SV_Target
{
	float4 color = input.color;

	color = SampleTexture.Sample(samLinearClamp, input.uv);

	color.a = 1.f;

	return color;
}