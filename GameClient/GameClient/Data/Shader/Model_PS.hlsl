#include "LightHeader.hlsli"

cbuffer cbMesh : register(b0)
{
	float4x4 gWorld;
	float4x4 gProj;
	float4x4 gWorldViewProj;
	float4x4 gWorldInvTranspose;

	uint objectID;
};

cbuffer cbMaterial : register(b1)
{
	float4 gMaterialAmbient;
	float4 gMaterialDiffuse;
	float4 gMaterialSpecular;
	float4 gMaterialReflection;

	bool isDiffuseTexture;			// hlsl���� boolŸ���� 4byte �̹Ƿ� �Ϸ� ����ü �Ѱ��ٶ� BOOL�� �Ѱ�����Ѵ�
	bool isNormalTexture;
	bool isSpecularTexture;
	bool isLight;
}

struct ModelPixelIn
{
	float4 pos		   : SV_POSITION;
	float3 posW		   : POSITION;
	float4 color	   : COLOR;
	float2 uv		   : UV;
	float3 normal	   : NORMAL;
	float3 tangent	   : TANGENT;
};

struct PS_OUT
{
	float4 depth  : SV_Target0;
	float4 normal : SV_Target1;
	float4 pos	  : SV_Target2;
	float4 albedo : SV_Target3;

	uint objectID  : SV_Target4;
};

Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);
Texture2D gSpecularMap : register(t2);

SamplerState samAnisotropicWrap : register(s0);
SamplerState samLinearWrap : register(s1);
SamplerState samLinearClamp : register(s2);

PS_OUT Model_PS(ModelPixelIn input) : SV_Target
{
	if (isDiffuseTexture)
	{
		// GammaCorrection ����
		input.color = pow(gDiffuseMap.Sample(samLinearClamp, input.uv), 2.2f);
	}

	// 0 <= r, g, b <= 1 �� �����ϴ� ����ȭ�� ���е�� �̷���� (r,g,b)�� �����Ǿ���. 
	// [0, 255] ������ ������ 255�� ����� [0, 1] ������ �ε��Ҽ��� ���� ����� �۾��� �� ���̴�.
	// NormalSampleToWorldSpace �Լ����� �������͹����� [-1, 1]�� ��� �����ش�.
	if (isNormalTexture)
	{
		float3 normalMapSample = gNormalMap.Sample(samLinearClamp, input.uv).rgb;
		input.normal = NormalSampleToWorldSpace(normalMapSample, input.normal, input.tangent);
	}

	if (isSpecularTexture)
	{
		// �ϴ� �ӽ�
		input.color.a = 1.0f;
	}

	input.color.a = 1.0f;

	input.normal = normalize(input.normal);

	PS_OUT pout = (PS_OUT) 0;

	pout.depth = float4(input.pos.zzz, 1.0f);

	pout.normal = float4(input.normal, 1.0f);

	// w�� isLight �����ֱ� ��
	pout.pos = float4(input.posW, isLight);

	pout.albedo = input.color;

	pout.objectID = objectID;

	return pout;
}