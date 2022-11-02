#pragma once
#include <string>
#include "SimpleMath.h"

enum class OBJECT_TYPE
{
	DEFAULT,
	SKY_BOX,
	PARTICLE,
};

struct Material
{
	std::string name;

	bool isLight = true;

	std::wstring albedoMap;
	std::wstring normalMap;
	std::wstring metallicMap;
	std::wstring roughnessMap;
	std::wstring AOMap;
	std::wstring emissiveMap;

	std::wstring cubeMap;

	DirectX::SimpleMath::Vector4 material_Ambient;
	DirectX::SimpleMath::Vector4 material_Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::SimpleMath::Vector4 material_Specular;
	DirectX::SimpleMath::Vector4 material_Emissive;

	float material_Transparency;	// 투명도
	float material_Reflectivity;	// 반사율

	float metallic;
	float roughness;
};

struct ObjectInfo
{
	OBJECT_TYPE type = OBJECT_TYPE::DEFAULT;
	
	unsigned int objectID;

	size_t meshID;
	
	DirectX::SimpleMath::Matrix worldTM;
	DirectX::SimpleMath::Vector3 worldPos;
	DirectX::SimpleMath::Matrix finalBoneListMatrix[96];
	
	std::string materialName;

	std::wstring vsName;

	std::wstring psName;

	bool isSkinned = false;

	bool isShadow = true;
};

struct TextInfo
{
	int x;
	int y;

	std::string text;
	
	Vector4 color;
	
	float scale;
};