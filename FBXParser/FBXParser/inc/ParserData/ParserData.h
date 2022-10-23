#pragma once
#include <algorithm>
#include "SimpleMath.h"

struct Vertex
{
	/*Vertex()
	{
		for (int i = 0; i < 8; i++)
		{
			weights[i] = 0.f;
			boneIndices[i] = -1;
		}
	}*/
	Vertex(DirectX::SimpleMath::Vector3 paPos = { 0.f, 0.f, 0.f },
		DirectX::SimpleMath::Vector4 paColor = { 1.0f, 1.0f, 1.0f, 1.0f },
		DirectX::SimpleMath::Vector2 paUV = { 0.f, 0.f },
		DirectX::SimpleMath::Vector3 paNormal = { 0.f, 0.f, 0.f },
		DirectX::SimpleMath::Vector3 paTangent = { 0.f, 0.f, 0.f })
		: position(paPos), normal(paNormal), uv(paUV), tangent(paTangent), color(paColor)
	{
		for (int i = 0; i < 8; i++)
		{
			weights[i] = 0.f;
			boneIndices[i] = -1;
		}
	}

	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector4 color;
	DirectX::SimpleMath::Vector2 uv;
	DirectX::SimpleMath::Vector3 normal;
	DirectX::SimpleMath::Vector3 tangent;

	// Skinned Data
	float weights[8];
	int boneIndices[8];
};

struct FBXMaterialInfo
{
	std::string materialName;

	bool isAlbedo = false;
	bool isNormal = false;
	bool isMetallic = false;
	bool isRoughness = false;
	bool isAO = false;
	bool isEmissive = false;

	std::wstring albedoMap;
	std::wstring normalMap;
	std::wstring metallicMap;
	std::wstring roughnessMap;
	std::wstring AOMap;
	std::wstring emissiveMap;

	float metallic;
	float roughness;
};

struct FBXMeshInfo
{
	std::string						meshName;			// mesh 이름!
	std::string						materialName;		// 이 mesh에 해당하는 material 이름!
	std::vector<Vertex>				meshVertexList;
	std::vector<unsigned int>		indices;

	DirectX::SimpleMath::Matrix nodeTM;					// Node TransformMatrix

	bool isSkinned = false;		// 스키닝오브젝트 인지 아닌지..
};

struct FBXBoneInfo
{
	std::string				boneName;

	int						parentIndex;

	DirectX::SimpleMath::Matrix offsetMatrix;							// Bone's OffsetMatrix
};

struct FBXKeyFrameInfo
{
	float		time;

	DirectX::SimpleMath::Vector3 localTransform;
	DirectX::SimpleMath::Vector4 localRotation;
	DirectX::SimpleMath::Vector3 localScale;

	DirectX::SimpleMath::Vector3 worldTransform;
	DirectX::SimpleMath::Vector4 worldRotation;
	DirectX::SimpleMath::Vector3 worldScale;
};

struct FBXAnimClipInfo
{
	std::string	animationName;

	float frameRate = 0.0f;

	float tickPerFrame = 0.0f;					
	
	int totalKeyFrame = 0;							

	int startKeyFrame = 0;
	
	int endKeyFrame = 0;

	std::vector<std::vector<std::shared_ptr<FBXKeyFrameInfo>>> keyFrameList;	 //	키프레임 리스트
};

class FBXModel
{
public:
	std::vector<std::shared_ptr<FBXMeshInfo>> fbxMeshInfoList;		

	std::vector<std::shared_ptr<FBXMaterialInfo>>	materialList;
	
	//std::shared_ptr<FBXSkeletonInfo> fbxSkeletonInfo;

	std::vector<std::shared_ptr<FBXBoneInfo>> fbxBoneInfoList;

	std::vector<std::shared_ptr<FBXAnimClipInfo>> animationClipList;

	bool isSkinnedAnimation;		// 스키닝 애니메이션 존재 여부
};

