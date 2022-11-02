#include "LightPBRHeader.hlsli"
#include "ConstantBufferHeader.hlsli"

struct ModelPixelIn
{
	float4 pos		   : SV_POSITION;
	float3 posW		   : POSITION;
	float4 color	   : COLOR;
	float2 uv		   : UV;
	float3 normal	   : NORMAL;
	float3 tangent	   : TANGENT;

	float metallic	   : METALLIC;
	float roughness	   : ROUGHNESS;
};

struct PS_OUT
{
	float4 albedo   : SV_Target0;
	float4 DMRAO    : SV_Target1;			// Depth, Metallic, Roughness, Ambient Occlusion
	float4 normal   : SV_Target2;
	float4 pos	    : SV_Target3;
	float4 emissive : SV_Target4;
};

Texture2D AlbedoMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D MetallicMap : register(t2);
Texture2D RoughnessMap : register(t3);
Texture2D AmbientOcclusionMap : register(t4);
Texture2D EmissiveMap : register(t5);

SamplerState samLinearClamp : register(s0);

PS_OUT Model_PBR_PS(ModelPixelIn input) : SV_Target
{
	float4 color = input.color;

	float metallic = input.metallic;

	float roughness = input.roughness;

	float ambientOcclusion = 1.f;

	float4 emissive = float4(0.0f, 0.0f, 0.0f, 1.0f);

#ifdef USE_ALBEDO
	input.color = pow(AlbedoMap.Sample(samLinearClamp, input.uv), 2.2f);
#endif

	// 0 <= r, g, b <= 1 �� �����ϴ� ����ȭ�� ���е�� �̷���� (r,g,b)�� �����Ǿ���. 
	// [0, 255] ������ ������ 255�� ����� [0, 1] ������ �ε��Ҽ��� ���� ����� �۾��� �� ���̴�.
	// NormalSampleToWorldSpace �Լ����� �������͹����� [-1, 1]�� ��� �����ش�.
#ifdef USE_NORMAL
	float3 normalMapSample = NormalMap.Sample(samLinearClamp, input.uv).rgb;
	input.normal = NormalSampleToWorldSpace(normalMapSample, input.normal, input.tangent);
#endif

	input.normal = normalize(input.normal);

#ifdef USE_METALLIC
	metallic = MetallicMap.Sample(samLinearClamp, input.uv);
#endif
	
#ifdef USE_ROUGHNESS
	roughness = RoughnessMap.Sample(samLinearClamp, input.uv);
#endif
	
#ifdef USE_AO
	ambientOcclusion = AmbientOcclusionMap.Sample(samLinearClamp, input.uv);
#endif

#ifdef USE_EMISSIVE
	emissive = EmissiveMap.Sample(samLinearClamp, input.uv);
#endif

	input.color.rgb += AddColor.rgb;
	 
	PS_OUT pout = (PS_OUT)0;

	pout.DMRAO = float4(input.pos.z, metallic, roughness, ambientOcclusion);

	pout.normal = float4(input.normal, 1.0f);

	// position.w�� isLight ���� �ֱ�
	pout.pos = float4(input.posW, isLight);

	pout.albedo = input.color;

	pout.emissive = emissive;

	return pout;
}