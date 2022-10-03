#include "LightHeader.hlsli"
#include "ConstantBufferHeader.hlsli"

#ifdef Skinned
struct ModelVertexIn
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
struct ModelVertexIn
{
	float3 pos     : POSITION;
	float4 color   : COLOR;
	float2 uv	   : UV;
	float3 normal  : NORMAL;
	float3 tangent : TANGENT;
};
#endif

struct ModelVertexOut
{
	float4 pos     : SV_POSITION;
	float3 posW	   : POSITION;
	float4 color   : COLOR;
	float2 uv	   : UV;
	float3 normal  : NORMAL;
	float3 tangent : TANGENT;

	float metallic : METALLIC;
	float roughness : ROUGHNESS;
};

ModelVertexOut Model_PBR_VS(ModelVertexIn input)
{
	ModelVertexOut output;

#ifdef Skinned
	float3 pos = float3(0.0f, 0.0f, 0.0f);
	float3 normal = float3(0.0f, 0.0f, 0.0f);

	float totalWeight = (float)0.f;

	for (int k = 0; k < 4; k++)
	{
		totalWeight += input.weight1[k];

		totalWeight += input.weight2[k];
	}

	for (int i = 0; i < 4; i++)
	{
		// 논유니폼 스케일이 없다고 가정하므로, 노말값을 위한 역행렬의 전치행렬이 필요없다.
		pos += (input.weight1[i] / totalWeight) * mul(float4(input.pos, 1.0f), gBoneTransforms[input.boneIndex1[i]]).xyz;
		normal += (input.weight1[i] / totalWeight) * mul(input.normal, (float3x3)gBoneTransforms[input.boneIndex1[i]]);
	}

	for (int j = 0; j < 4; j++)
	{
		// 논유니폼 스케일이 없다고 가정하므로, 노말값을 위한 역행렬의 전치행렬이 필요없다.
		pos += (input.weight2[j] / totalWeight) * mul(float4(input.pos, 1.0f), gBoneTransforms[input.boneIndex2[j]]).xyz;
		normal += (input.weight2[j] / totalWeight) * mul(input.normal, (float3x3)gBoneTransforms[input.boneIndex2[j]]);
	}

	output.posW = mul(float4(pos, 1.0f), gWorld).xyz;

	output.pos = mul(float4(pos, 1.0f), gWorldViewProj);

	// 접선벡터와 법선벡터가 여전히 직교가되는 법선 벡터 변환행렬인 역전치행렬을 노말(법선벡터)에 곱해준다.
	output.normal = normalize(mul(normal, (float3x3)gWorldInvTranspose));
#else
	output.posW = mul(float4(input.pos, 1.0f), gWorld).xyz;

	output.pos = mul(float4(input.pos, 1.0f), gWorldViewProj);

	output.normal = normalize(mul(input.normal, (float3x3)gWorldInvTranspose));
#endif

	output.uv = input.uv;

	// output.탄젠트월드 = mul(탄젠트로컬, gWorld) 월드로 바꿔줌
	output.tangent = mul(input.tangent, (float3x3)gWorld);

	output.color = input.color;

	output.metallic = metallic;

	output.roughness = roughness;

	return output;
}