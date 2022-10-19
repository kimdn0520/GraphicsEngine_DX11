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

	std::string parentName;		// 부모 이름
	std::shared_ptr<FBXMeshInfo> parentMesh;	// 부모의 메시

	std::vector<std::shared_ptr<FBXMeshInfo>> childMeshList;	// 자식 메시들

	bool isParent;				// 부모가 존재하는지

	bool isSkinned = false;		// 스키닝오브젝트 인지 아닌지..
};

struct FBXBoneInfo
{
	std::string				boneName;
	int						boneID;

	std::string				parentBoneName;
	int						parentIndex;

	int						depth = 0;

	std::vector<std::shared_ptr<FBXBoneInfo>> childBoneList;	// 자식 노드 리스트

	DirectX::SimpleMath::Matrix offsetMatrix;							// Bone's OffsetMatrix
	
	DirectX::SimpleMath::Matrix worldTM = DirectX::XMMatrixIdentity();	// Bone's World Transform Matrix
};

struct FBXSkeletonInfo
{
	std::vector<std::shared_ptr<FBXBoneInfo>> fbxBoneInfoList;

	/*void AddBone(std::shared_ptr<FBXBoneInfo> bone)
	{
		fbxBoneInfoList.push_back(bone);
	}*/
};

struct FBXKeyFrameInfo
{
	float		time;

	DirectX::SimpleMath::Vector3 transform;
	DirectX::SimpleMath::Vector4 rotation;
	DirectX::SimpleMath::Vector3 scale;
};

struct FBXAnimClipInfo
{
	std::string			animationName;
	
	float	ticksPerFrame;	// 틱 값
	int		totalKeyFrame;	// 총 프레임
	int		startKeyFrame;	// 시작 프레임
	int		endKeyFrame;	// 마지막 프레임

	float	duration;		// 속도

	std::vector<std::shared_ptr<FBXKeyFrameInfo>> keyFrameList;	 //	키프레임 리스트
};

class FBXModel
{
public:
	std::vector<std::shared_ptr<FBXMeshInfo>> fbxMeshInfoList;		

	std::vector<std::shared_ptr<FBXMaterialInfo>>	materialList;
	
	std::shared_ptr<FBXSkeletonInfo> fbxSkeletonInfo;
	
	std::vector<std::shared_ptr<FBXAnimClipInfo>> animationClipList;

	bool isSkinnedAnimation;		// 스키닝 애니메이션 존재 여부
};

