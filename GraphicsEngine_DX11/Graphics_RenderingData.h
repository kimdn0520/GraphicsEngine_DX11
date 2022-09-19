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
	bool isDiffuse = false;
	bool isNormal = false;
	bool isSpecular = false;
	bool isReflection = false;
	bool isLight = true;

	std::wstring diffuseTexture;
	std::wstring normalTexture;
	std::wstring specularTexture;
	std::wstring cubeMapTexture;

	DirectX::SimpleMath::Vector4 ambient;
	DirectX::SimpleMath::Vector4 diffuse;
	DirectX::SimpleMath::Vector4 specular;
	DirectX::SimpleMath::Vector4 reflection;
};

struct ObjectInfo
{
	OBJECT_TYPE type = OBJECT_TYPE::DEFAULT;
	
	unsigned int objectID;

	size_t meshID;
	
	DirectX::SimpleMath::Matrix worldTM;
	DirectX::SimpleMath::Vector3 worldPos;
	DirectX::SimpleMath::Matrix finalBoneListMatrix[96];
	
	std::vector<Material*> materials;

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