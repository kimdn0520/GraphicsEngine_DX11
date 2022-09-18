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

	bool isDiffuseTexture;			// hlsl에서 bool타입은 4byte 이므로 일로 구조체 넘겨줄때 BOOL로 넘겨줘야한대
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
		// GammaCorrection 적용
		input.color = pow(gDiffuseMap.Sample(samLinearClamp, input.uv), 2.2f);
	}

	// 0 <= r, g, b <= 1 을 만족하는 정규화된 성분들로 이루어진 (r,g,b)가 설정되었다. 
	// [0, 255] 구간의 정수를 255로 나누어서 [0, 1] 구간의 부동소수점 수로 만드는 작업이 된 것이다.
	// NormalSampleToWorldSpace 함수에서 단위벡터범위인 [-1, 1]로 비례 시켜준다.
	if (isNormalTexture)
	{
		float3 normalMapSample = gNormalMap.Sample(samLinearClamp, input.uv).rgb;
		input.normal = NormalSampleToWorldSpace(normalMapSample, input.normal, input.tangent);
	}

	if (isSpecularTexture)
	{
		// 일단 임시
		input.color.a = 1.0f;
	}

	input.color.a = 1.0f;

	input.normal = normalize(input.normal);

	PS_OUT pout = (PS_OUT) 0;

	pout.depth = float4(input.pos.zzz, 1.0f);

	pout.normal = float4(input.normal, 1.0f);

	// w에 isLight 끼워넣기 ㅎ
	pout.pos = float4(input.posW, isLight);

	pout.albedo = input.color;

	pout.objectID = objectID;

	return pout;
}