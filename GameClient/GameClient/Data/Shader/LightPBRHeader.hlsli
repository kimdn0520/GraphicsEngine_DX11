// ���۷��� : https://github.com/Nadrin/PBR/blob/master/data/shaders/hlsl/pbr.hlsl

// Light Informations For PBR
struct DirectionalLight
{
    float3 color;          // ���� ��
    float lumen;           // ���� ����

    float3 direction;      // ����
    float pad;
};

struct PointLight
{
    float3 color;
    float lumen;

    float3 position;       // ��ġ
    float range;           // ����
};

struct SpotLight
{
    float3 color;
    float lumen;

    float3 direction;      // ����
    float halfAngle;       // Spot Half Angle (== FOV) => ����Ʈ ����Ʈ�� ������ ����

    float3 position;       // ��ġ
    float range;           // ������ ����
};

// Physically Based shading model: Lambetrtian diffuse BRDF + Cook-Torrance microfacet specular BRDF + IBL for ambient.

// This implementation is based on "Real Shading in Unreal Engine 4" SIGGRAPH 2013 course notes by Epic Games.
// See: http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf

static const float PI = 3.141592f;
static const float Epsilon = 0.00001f;

// Constant normal incidence Fresnel factor for all dielectrics.
static const float3 Fdielectric = 0.04f;

// �̼��� �����Լ� �ΰͰ����� GGX�� ���� ã�� �� ������..
// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;

    float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
    return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

//Smith�� ����� ����� �������� ���� �Լ��� Schlick-GGX �ٻ�
// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
    return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// ������ ������ Shlick �ٻ�.
// Shlick's approximation of the Fresnel factor.
float3 fresnelSchlick(float3 F0, float cosTheta)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// Returns number of mipmap levels for specular IBL environment map.
uint querySpecularTextureLevels(TextureCube specularTexture)
{
    uint width, height, levels;

    specularTexture.GetDimensions(0, width, height, levels);

    return levels;
}

// ���౤���Դ� �������� ����.
float CalcAttenuation(float distance, float range)
{
    float distance2 = pow(distance, 2.f);

    float range2 = pow(range, 2.f);

    float distanceAttenuation = 1.f / (distance2 + 1);

    float lightRadiusMask = pow(saturate(1 - pow(distance2 / range2, 2.f)), 2.f);

    return distanceAttenuation * lightRadiusMask;
}

float3 CalcDirectionalLight(in DirectionalLight light, float3 albedo, float3 F0, float3 Lo, float cosLo,
    float3 N, float metallic, float roughness)
{
    float3 Li = -normalize(light._direction);

    float3 Lradiance = light.color * light.lumen;

    // Half-Vector Li and Lo
    float3 Lh = normalize(Li + Lo);

    float cosLi = max(0.f, dot(N, Li));
    float cosLh = max(0.f, dot(N, Lh));

    float3 F = fresnelSchlick(F0, max(0.f, dot(Lh, Lo)));

    float D = ndfGGX(cosLh, roughness);

    float G = gaSchlickGGX(cosLi, cosLo, roughness);

    float3 kd = lerp(float3(1.f, 1.f, 1.f) - F, float3(0.f, 0.f, 0.f), metallic);

    float3 diffuseBRDF = kd * albedo;

    float3 specularBRDF = (F * D * G) / max(Epsilon, 4.f * cosLi * cosLo);      // ������ ��𰬾�

    return (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
}

//float3 CalcSpotLight(in SpotLightInfo lightInfo, float3 pixelPosition, float3 albedo, float F0, float3 Lo, float cosLo,
//    float3 N, float metallic, float roughness)
//{
//    float3 Li = -normalize(lightInfo._direction);

//    // 1. Spot Light�� Half Angle �ȿ� �����°�
//    float3 PixelToLight = normalize(lightInfo._position - pixelPosition);


//    // 2. PBR ���
//    float3 Li = -lightInfo._direction

//    // 3. ����
//    float distance = length(lightInfo._position - pixelPosition);

//    float attenuation = CalcAttenuation(distance, lightInfo._range);

//}

//float3 CalcPointLight()
//{

//    // Attenuation
//}