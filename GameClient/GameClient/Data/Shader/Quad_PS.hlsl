
struct QuadPixelIN
{
	float4 pos     : SV_POSITION;
	float3 posW	   : POSITION;
	float4 color   : COLOR;
	float2 uv	   : UV;
	float3 normal  : NORMAL;
	float3 tangent : TANGENT;
};

Texture2D gDiffuseMap : register(t0);
SamplerState samAnisotropicWrap : register(s0);

float4 Quad_PS(QuadPixelIN input) : SV_Target
{
	input.color = gDiffuseMap.Sample(samAnisotropicWrap, input.uv);

	input.color.a = 1.0f;

	return input.color;
}