#include "LightPBRHeader.hlsli"
#include "ConstantBufferHeader.hlsli"

struct SkyBoxVertexIN
{
	float3 pos		: POSITION;
	float4 color	: COLOR;
	float2 uv		: UV;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
};

struct SkyBoxVertexOut
{
	float4 posH     : SV_POSITION;
	float3 posL     : POSITION;
	float3 posW		: POSITION2;
};

SkyBoxVertexOut Skybox_VS(SkyBoxVertexIN input)
{
	SkyBoxVertexOut output;

	// z/w = 1이 되도록 (즉 하늘 돔이 항상 먼 평면에 있도록) z = w로 설정한다.
	output.posH = mul(float4(input.pos, 1.0f), gWorldViewProj).xyww;

	output.posL = input.pos;

	output.posW = mul(float4(input.pos, 1.0f), gWorld).xyz;

	return output;
}
