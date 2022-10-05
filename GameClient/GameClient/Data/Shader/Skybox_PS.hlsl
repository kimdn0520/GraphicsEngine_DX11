#include "LightPBRHeader.hlsli"
#include "ConstantBufferHeader.hlsli"

struct SkyBoxPixelIN
{
	float4 posH     : SV_POSITION;
	float3 posL     : POSITION;
	float3 posW		: POSITION2;
};

struct PS_OUT
{
	float4 albedo   : SV_Target0;
	float4 DMRAO    : SV_Target1;			// Depth, Metallic, Roughness, Ambient Occlusion
	float4 normal   : SV_Target2;
	float4 pos	    : SV_Target3;
	float4 emissive : SV_Target4;
};

TextureCube CubeMap : register(t0);
SamplerState samLinearClamp : register(s0);

PS_OUT Skybox_PS(SkyBoxPixelIN input) : SV_Target
{
	PS_OUT pout;

	pout.DMRAO = float4(input.posH.z, 0.f, 0.f, 1.f);

	pout.pos = float4(input.posW, isLight);

	pout.albedo = pow(CubeMap.Sample(samLinearClamp, input.posL), 2.2f);

	pout.emissive = float4(0.f, 0.f, 0.f, 1.f);

	return pout;
}
