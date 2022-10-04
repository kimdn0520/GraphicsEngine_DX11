#pragma once
#include "SimpleMath.h"
using namespace DirectX;
using namespace SimpleMath;

#define DIRECTION_LIGHT_COUNT 1
#define POINT_LIGHT_COUNT 30
#define SPOT_LIGHT_COUNT 30

//---------------------------------------------------------------------------------------------------------
// ���̴��� �ʿ��� struct
//---------------------------------------------------------------------------------------------------------
struct DirectionalLight
{
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	Vector3 color;
	float power;

	Vector3 direction;
	float pad;

	Matrix LightViewProj;
};

struct PointLight
{
	PointLight() { ZeroMemory(this, sizeof(this)); }

	Vector3 color;
	float power;

	Vector3 position;
	float range;

	Matrix LightViewProj;
};

struct SpotLight
{
	SpotLight() { ZeroMemory(this, sizeof(this)); }

	Vector3 color;
	float power;

	Vector3 direction;
	float halfAngle;

	Vector3 position;
	float range;

	Matrix LightViewProj;
};

//---------------------------------------------------------------------------------------------------------
// ���̴��� �ʿ��� ConstantBuffer�� �̸��̶� ������ �����־ƾ��Ѵ�.
//---------------------------------------------------------------------------------------------------------
__declspec(align(16)) struct cbPerObject
{
	Matrix gWorld;
	Matrix gWorldViewProj;
	Matrix gWorldInvTranspose;

	Matrix gBoneTransforms[96];
};

__declspec(align(16)) struct cbLight
{
	DirectionalLight gDirLight[DIRECTION_LIGHT_COUNT];
	PointLight gPointLight[POINT_LIGHT_COUNT];
	SpotLight gSpotLight[SPOT_LIGHT_COUNT];

	Vector3 gEyePosW;	// CameraPos

	int gDirLightCnt;
	int gPointLightCnt;
	int gSpotLightCnt;
};

__declspec(align(16)) struct cbMaterial
{
	float metallic;
	float roughness;
	Vector2 pad;

	BOOL isAlbedo;			// hlsl���� boolŸ���� 4byte �̹Ƿ� �Ϸ� ����ü �Ѱ��ٶ� BOOL�� �Ѱ�����Ѵ�
	BOOL isNormal;
	BOOL isMetallic;
	BOOL isRoughness;
	BOOL isAO;
	BOOL isEmissive;

	BOOL isLight = true;
};

__declspec(align(16)) struct cbTexture
{
	Vector4 textureInfo;
};

__declspec(align(16)) struct cbLightViewProj
{
	Matrix lightViewProj;
};

__declspec(align(16)) struct cbToneMapping
{
	float exposure;

	Vector3 pad;
};