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

	Vector4 Ambient;
	Vector4 Diffuse;
	Vector4 Specular;
	Vector3 Direction;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.

	Matrix LightViewProj;
};

struct PointLight
{
	PointLight() { ZeroMemory(this, sizeof(this)); }

	Vector4 Ambient;
	Vector4 Diffuse;
	Vector4 Specular;

	// Packed into 4D vector: (Position, Range)
	// ���̴��� ���� 4���� ���Ϳ� �ϳ��� ���������� 331�� ����
	Vector3 Position;
	float Range;

	// Packed into 4D vector: (A0, A1, A2, Pad)
	Vector3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.

	Matrix LightViewProj;
};

struct SpotLight
{
	SpotLight() { ZeroMemory(this, sizeof(this)); }

	Vector4 Ambient;
	Vector4 Diffuse;
	Vector4 Specular;

	// Packed into 4D vector: (Position, Range)
	Vector3 Position;
	float Range;

	// Packed into 4D vector: (Direction, Spot)
	Vector3 Direction;
	float Spot;

	// Packed into 4D vector: (Att, Pad)
	Vector3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
	// Vector3�� �� pad�� �־���

	Matrix LightViewProj;
};

//---------------------------------------------------------------------------------------------------------
// ���̴��� �ʿ��� ConstantBuffer�� �̸��̶� ������ �����־ƾ��Ѵ�.
//---------------------------------------------------------------------------------------------------------
__declspec(align(16)) struct cbMesh
{
	Matrix gWorld;
	Matrix gProj;
	Matrix gWorldViewProj;
	Matrix gWorldInvTranspose;

	unsigned int objectID;
};

__declspec(align(16)) struct cbSkinned
{
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
	Vector4 gMaterialAmbient;
	Vector4 gMaterialDiffuse;
	Vector4 gMaterialSpecular;
	Vector4 gMaterialReflection;

	BOOL isDiffuseTexture;			// hlsl���� boolŸ���� 4byte �̹Ƿ� �Ϸ� ����ü �Ѱ��ٶ� BOOL�� �Ѱ�����Ѵ�
	BOOL isNormalTexture;
	BOOL isSpecularTexture;
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