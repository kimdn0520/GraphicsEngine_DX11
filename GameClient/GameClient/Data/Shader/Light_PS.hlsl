#include "LightHeader.hlsli"
#include "ConstantBufferHeader.hlsli"

struct LightPixelIN
{
	float4 pos		   : SV_POSITION;
	float3 posW		   : POSITION;
	float4 color	   : COLOR;
	float2 uv		   : UV;
	float3 normal	   : NORMAL;
	float3 tangent	   : TANGENT;
};

Texture2D Depth : register(t0);

Texture2D Normal : register(t1);

Texture2D Position : register(t2);

Texture2D Albedo : register(t3);

Texture2D<uint> ObjectID : register(t4);

Texture2D Shadow : register(t5);

SamplerState samAnisotropicClamp : register(s0);
SamplerState samAnisotropicWrap : register(s1);
SamplerState samLinearClamp : register(s2);
SamplerState samLinearWrap : register(s3);
SamplerComparisonState samLinearPointBoarder : register(s4);

float4 Light_PS(LightPixelIN input) : SV_Target
{
	float4 depth = Depth.Sample(samLinearClamp, input.uv).x;

	float3 normal = Normal.Sample(samLinearClamp, input.uv).xyz;

	float4 position = Position.Sample(samLinearClamp, input.uv).xyzw;

	float4 albedo = Albedo.Sample(samLinearClamp, input.uv).xyzw;

	uint objectID = (uint)ObjectID.Load(int3(input.uv.x * textureInfo.x, input.uv.y * textureInfo.y, 0));

	float4 shadow = mul(float4(position.xyz, 1.0f), gDirLight[0].LightViewProj);

	float3 NormalW = normalize(input.normal);

	float shadowVal = 1.0f;

	shadow.xyz /= shadow.w;

	shadow.x = shadow.x * 0.5f + 0.5f;
	shadow.y = shadow.y * -0.5f + 0.5f;

	shadowVal = CalcShadowFactor(samLinearPointBoarder, Shadow, float3(shadow.xyz), textureInfo);

	// �ʱ�ȭ..
	float4 ambient = float4(depth.z - depth.z, objectID - objectID, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 toEye = gEyePosW - position.xyz;

	bool isLight = position.w;

	if (isLight)
	{
		toEye = normalize(toEye);

		float4 A, D, S;

		for (int i = 0; i < gDirLightCnt; i++)
		{
			ComputeDirectionalLight(float4(0.7f, 0.7f, 0.7f, 1.f), float4(0.7f, 0.7f, 0.7f, 1.f),
				float4(0.4f, 0.4f, 0.4f, 1.f), float4(0.3f, 0.3f, 0.3f, 1.f),
				gDirLight[i], normal, toEye,
				A, D, S);


			ambient += A;
			diffuse += shadowVal * D;
			spec += shadowVal * S;
		}

		for (int i = 0; i < gPointLightCnt; i++)
		{
			ComputePointLight(float4(0.6f, 0.6f, 0.6f, 1.f), float4(0.6f, 0.6f, 0.6f, 1.f),
				float4(0.3f, 0.3f, 0.3f, 1.f), float4(0.3f, 0.3f, 0.3f, 1.f),
				gPointLight[i], position.xyz, normal, toEye,
				A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;
		}

		for (int i = 0; i < gSpotLightCnt; i++)
		{
			ComputeSpotLight(float4(0.6f, 0.6f, 0.6f, 1.f), float4(0.6f, 0.6f, 0.6f, 1.f),
				float4(0.3f, 0.3f, 0.3f, 1.f), float4(0.3f, 0.3f, 0.3f, 1.f),
				gSpotLight[i], position.xyz, normal, toEye,
				A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;
		}

		albedo = albedo * (ambient + diffuse) + spec;
		albedo.a = 1.f;
	}

	return albedo;
}