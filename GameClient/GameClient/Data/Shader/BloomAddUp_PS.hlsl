
struct BloomAddUpIn
{
	float4 pos		   : SV_POSITION;
	float3 posW		   : POSITION;
	float4 color	   : COLOR;
	float2 uv		   : UV;
	float3 normal	   : NORMAL;
	float3 tangent	   : TANGENT;
};

Texture2D BlurOutput : register(t0);
Texture2D AddUpTexture : register(t1);
SamplerState samLinearClamp : register(s0);

float4 BloomAddUp_PS(BloomAddUpIn input) : SV_Target
{
	float4 color = (float4) 0.f;

	color = BlurOutput.Sample(samLinearClamp, input.uv) + AddUpTexture.Sample(samLinearClamp, input.uv);

	return color;
}