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
	bool isAlbedo = false;
	bool isNormal = false;
	bool isMetallic = false;
	bool isRoughness = false;
	bool isAO = false;
	bool isEmissive = false;
	bool isLight = true;

	std::wstring albedoMap;
	std::wstring normalMap;
	std::wstring metallicMap;
	std::wstring roughnessMap;
	std::wstring AOMap;
	std::wstring emissiveMap;

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
	
	std::vector<std::shared_ptr<Material>> materials;

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