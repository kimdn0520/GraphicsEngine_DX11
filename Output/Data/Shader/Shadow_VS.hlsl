#include "LightHeader.hlsli"
#include "ConstantBufferHeader.hlsli"

#ifdef Skinned
struct ShadowVertexIn
{
	float3 pos     : POSITION;
	float4 color   : COLOR;
	float2 uv	   : UV;
	float3 normal  : NORMAL;
	float3 tangent : TANGENT;

	float4 weight1 : WEIGHT;
	float4 weight2 : WEIGHTS;

	uint4 boneIndex1 : BONEINDEX;
	uint4 boneIndex2 : BONEINDEXES;
};
#else
struct ShadowVertexIn
{
	float3 pos     : POSITION;
	float4 color   : COLOR;
	float2 uv	   : UV;
	float3 normal  : NORMAL;
	float3 tangent : TANGENT;
};
#endif

struct ShadowVertexOut
{
	float4 pos : SV_POSITION;
};

ShadowVertexOut Shadow_VS(ShadowVertexIn input)
{
	ShadowVertexOut output;

#ifdef Skinned
	float3 pos = float3(0.0f, 0.0f, 0.0f);

	float totalWeight = (float)0.f;

	for (int k = 0; k < 4; k++)
	{
		totalWeight += input.weight1[k];

		totalWeight += input.weight2[k];
	}

	for (int i = 0; i < 4; i++)
	{
		pos += (input.weight1[i] / totalWeight) * mul(float4(input.pos, 1.0f), gBoneTransforms[input.boneIndex1[i]]).xyz;
	}

	for (int j = 0; j < 4; j++)
	{
		pos += (input.weight2[j] / totalWeight) * mul(float4(input.pos, 1.0f), gBoneTransforms[input.boneIndex2[j]]).xyz;
	}

	output.pos = mul(float4(pos, 1.0f), gWorld);

	output.pos = mul(output.pos, lightViewProj);

#else
	output.pos = mul(float4(input.pos, 1.0f), gWorld);

	output.pos = mul(output.pos, lightViewProj);

#endif

	return output;
}