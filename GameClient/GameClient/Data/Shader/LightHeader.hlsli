
#define DIRECTION_LIGHT_COUNT	1
#define POINT_LIGHT_COUNT		30
#define SPOT_LIGHT_COUNT		30

struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.

	float4x4 LightViewProj;
};

struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	// Packed into 4D vector: (Position, Range)
	float3 Position;
	float Range;

	// Packed into 4D vector: (A0, A1, A2, Pad)
	float3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.

	float4x4 LightViewProj;
};

struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	// Packed into 4D vector: (Position, Range)
	float3 Position;
	float Range;

	// Packed into 4D vector: (Direction, Spot)
	float3 Direction;
	float Spot;

	// Packed into 4D vector: (Att, Pad)
	float3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.

	float4x4 LightViewProj;
};

//---------------------------------------------------------------------------------------
// 다음의 HLSL 함수는 주어진 재질과 지향광원, 표면 법선, 그리고 시점 벡터(빛을 받는 표면
// 점에서 시점을 향한 단위 벡터)에 기초해서 표면 점의 조명 값(반사된 빛의 색상)을
// 계산한다.
//---------------------------------------------------------------------------------------
void ComputeDirectionalLight(float4 Ambient, float4 Diffuse, float4 Specular, float4 Reflect,
	DirectionalLight L,
	float3 normal, float3 toEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The light vector aims opposite the direction the light rays travel.
	// 빛 벡터는 들어오는 빛과 반대 방향으로 해준다.
	float3 lightVec = -L.Direction;

	// 주변광 재질 색상 * 입사 주변광
	ambient = Ambient * L.Ambient;

	// L*n 해주는 부분 [빛의 세기]!! 
	float diffuseFactor = dot(lightVec, normal);

	// 그래서 그 내적해준게 0보다 크면 (양수이면) 빛의 세기가 들어간다.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 r = reflect(-lightVec, normal);			// specular를 위한~  반사 벡터?
		// 반사벡터랑 시점벡터랑 내적하고 반영 지수 p(Specular.w)를 조정하여 거듭 제곱..
		float specFactor = pow(max(dot(r, toEye), 0.0f), Specular.w);

		// 아 ~ 람베르트코사인 법칙 * 분산광 재질 색상 * 입사 분산광 해주는 부분인것같아~
		diffuse = diffuseFactor * Diffuse * L.Diffuse;

		spec = specFactor * Specular * L.Specular;
	}
}

//---------------------------------------------------------------------------------------
// 다음의 HLSL 함수는 주어진 재질과 점광원, 표면 점 위치, 표면 법선, 그리고 시점 벡터(빛
// 을 받는 표면 점에서 시점을 향한 단위 벡터)에 기초해서 표면 점의 조명 값(반사된 빛의 색
// 상)을 계산한다.
//---------------------------------------------------------------------------------------
void ComputePointLight(float4 Ambient, float4 Diffuse, float4 Specular, float4 Reflection, PointLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// 출력 성분들을 초기화한다.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// 빛 벡터(표면 점에서 광원으로의 벡터)
	float3 lightVec = L.Position - pos;

	// 표면 점과 광원 사이의 거리
	float d = length(lightVec);

	// 범위 판정
	if (d > L.Range)
		return;

	// 빛 벡터를 정규화 한다.
	lightVec /= d;

	// 주변광 항
	ambient = Ambient * L.Ambient;

	// 빛이 막히지 않고 표면에 도달한다는 가정 하에서
	// 분상광 항과 반영광 항을 더한다.

	float diffuseFactor = dot(lightVec, normal);

	// 동적 분기를 피하기 위해 조건문을 펼친다.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 r = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(r, toEye), 0.0f), Specular.w);

		diffuse = diffuseFactor * Diffuse * L.Diffuse;
		spec = specFactor * Specular * L.Specular;
	}

	// 감쇠
	float att = 1.0f / dot(L.Att, float3(1.0f, d, d * d));

	diffuse *= att;
	spec *= att;
}

//---------------------------------------------------------------------------------------
// 다음의 HLSL 함수는 주어진 재질과 점적광원, 표면 점 위치, 표면 법선, 그리고 시점 벡터
// (빛을 받는 표면 점에서 시점을 향한 단위벡터)에 기초해서 표면 점의 조명 값(반사된 빛의
// 색상)을 계산한다.
//---------------------------------------------------------------------------------------
void ComputeSpotLight(float4 Ambient, float4 Diffuse, float4 Specular, float4 Reflection, SpotLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// 출력 성분들을 초기화한다.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// 빛 벡터(표면 점에서 광원으로의 벡터)
	float3 lightVec = L.Position - pos;

	// 표면 점과 광원 사이의 거리
	float d = length(lightVec);

	// 범위 판정
	if (d > L.Range)
		return;

	// 빛 벡터를 정규화한다.
	lightVec /= d;

	// 주변광 항
	ambient = Ambient * L.Ambient;

	// 빛이 막히지 않고 표면에 도달한다는 가정 하에서
	// 분산광 항과 반영광 항을 더한다.

	float diffuseFactor = dot(lightVec, normal);

	// 동적 분기를 피하기 위해 조건문을 펼친다.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 r = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(r, toEye), 0.0f), Specular.w);

		diffuse = diffuseFactor * Diffuse * L.Diffuse;
		spec = specFactor * Specular * L.Specular;
	}

	// 점적광 계수를 계산한다.
	// L.Spot로 점적광 원뿔을 키우거나 줄일 수 있다.
	float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);

	// 빛의 세기를 점적광 계수로 비례하고 거리에 따라 감쇠시킨다.
	float att = spot / dot(L.Att, float3(1.0f, d, d * d));

	ambient *= spot;
	diffuse *= att;
	spec *= att;
}

//---------------------------------------------------------------------------------------
// Transforms a normal map sample to world space.
//---------------------------------------------------------------------------------------
float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	// Uncompress each component from [0,1] to [-1,1].
	float3 normalT = 2.0f * normalMapSample - 1.0f;

	// Build orthonormal basis.
	float3 N = unitNormalW;
	float3 T = normalize(tangentW - dot(tangentW, N) * N);
	//float3 T = tangentW;
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	// Transform from tangent space to world space.
	float3 bumpedNormalW = mul(normalT, TBN);

	return bumpedNormalW;
}

float CalcShadowFactor(SamplerComparisonState samShadow,
	Texture2D shadowMap,
	float3 shadowPosNDC, float4 textureInfo)
{
	// Depth in NDC space.
	float depth = shadowPosNDC.z;

	// 쉐도우 맵의 텍셀 크기
	const float dx = textureInfo.z;

	const float dy = textureInfo.w;

	float percentLit = 0.0f;

	const float2 offset[9] =
	{
		float2(-dx, -dy), float2(0.0f, -dy), float2(dx, -dy),
		float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
		float2(-dx, +dy), float2(0.0f, +dy), float2(dx, +dy)
	};

	[unroll]
	for (int i = 0; i < 9; ++i)
	{
		percentLit += shadowMap.SampleCmpLevelZero(samShadow, shadowPosNDC.xy + offset[i], depth).r;
	}

	return percentLit /= 9.0f;
}