#define DIRECTION_LIGHT_COUNT 1
#define POINT_LIGHT_COUNT 30
#define SPOT_LIGHT_COUNT 30

cbuffer cbPerObject : register(b0)
{
	float4x4 gWorld;
	float4x4 gWorldViewProj;
	float4x4 gWorldInvTranspose;

	float4x4 gBoneTransforms[96];
};

cbuffer cbMaterial : register(b1)
{
	float metallic;
	float roughness;
	float2 pad1;

	float3 AddColor;
	float pad2;

	float3 emissiveColor;
	float pad3;
	
	bool isLight;
}

cbuffer cbLight : register(b2)
{
	DirectionalLight gDirLight[DIRECTION_LIGHT_COUNT];
	PointLight gPointLight[POINT_LIGHT_COUNT];
	SpotLight gSpotLight[SPOT_LIGHT_COUNT];

	float3 gEyePosW;				// CameraPos

	int gDirLightCnt;
	int gPointLightCnt;
	int gSpotLightCnt;
}

cbuffer cbLightViewProj : register(b3)
{
	float4x4 lightViewProj;
};

cbuffer cbTexture : register(b4)
{
	float4 textureInfo;
}

cbuffer cbToneMapping : register(b5)
{
	float exposure;

	float3 pad4;
}