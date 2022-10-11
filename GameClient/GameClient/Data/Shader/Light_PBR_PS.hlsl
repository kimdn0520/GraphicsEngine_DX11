#include "LightPBRHeader.hlsli"
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

Texture2D Albedo : register(t0);

Texture2D DMRAO : register(t1);			// depth, metallic, roughness, ambient occlusion

Texture2D Normal : register(t2);

Texture2D Position : register(t3);

Texture2D Emissive : register(t4);

Texture2D Shadow : register(t5);

Texture2D SSAO : register(t6);

SamplerState samLinearClamp : register(s0);
SamplerComparisonState samLinearPointBoarder : register(s1);

float4 Light_PBR_PS(LightPixelIN input) : SV_Target
{
	float depth = DMRAO.Sample(samLinearClamp, input.uv).x;

	float3 normal = Normal.Sample(samLinearClamp, input.uv).xyz;

	float4 position = Position.Sample(samLinearClamp, input.uv).xyzw;

	float4 albedo = Albedo.Sample(samLinearClamp, input.uv).xyzw;

	float3 emissive = Emissive.Sample(samLinearClamp, input.uv).xyz;

	float4 shadow = mul(float4(position.xyz, 1.0f), gDirLight[0].lightViewProj);

	float shadowVal = 1.0f;

#ifdef SHADOW_MACRO
	shadow.xyz /= shadow.w;

	shadow.x = shadow.x * 0.5f + 0.5f;
	shadow.y = shadow.y * -0.5f + 0.5f;

	shadowVal = CalcShadowFactor(samLinearPointBoarder, Shadow, float3(shadow.xyz), textureInfo);
#endif

#ifdef SSAO_MACRO
	float ambientAccess = SSAO.Sample(samLinearClamp, input.uv).x;
#else
	float ambientAccess = 1.f;
#endif	

	// 금속성
	float metallic = DMRAO.Sample(samLinearClamp, input.uv).y;

	// 거칠기
	float roughness = DMRAO.Sample(samLinearClamp, input.uv).z;

	// 차폐도
	float ambientOcclusion = DMRAO.Sample(samLinearClamp, input.uv).w * ambientAccess;

	float3 toEye = normalize(gEyePosW - position.xyz);

	bool isLight = position.w;

	float3 litColor = float3(0.0f, 0.0f, 0.0f);

	if (isLight)
	{
		litColor = PBR_DirectionalLight(toEye, normal, gDirLight[0],
			albedo.xyz, ambientOcclusion, roughness, metallic, shadowVal);

		litColor += PBR_PointLight(toEye, normal, gPointLight, gPointLightCnt, position.xyz,
			albedo.xyz, ambientOcclusion, roughness, metallic);

		litColor += PBR_SpotLight(toEye, normal, gSpotLight, gSpotLightCnt, position.xyz,
			albedo.xyz, ambientOcclusion, roughness, metallic);

		litColor += emissive;

		// 노말 벡터 방향에서의 Diffuse Irradiace를 가져온다.
		//float3 irradiance = Irradiance.Sample(samLinearClamp, normal).rgb;
	}
	else
	{
		litColor += albedo.xyz;
	}

	// 직접광 + 간접광
	float3 ambientLighting = albedo * (1.1f - roughness) * 0.3f;

	return float4(litColor + ambientLighting, 1.0f);
}