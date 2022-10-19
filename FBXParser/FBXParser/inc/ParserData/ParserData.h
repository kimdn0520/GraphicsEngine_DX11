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
	std::string						meshName;			// mesh �̸�!
	std::string						materialName;		// �� mesh�� �ش��ϴ� material �̸�!
	std::vector<Vertex>				meshVertexList;
	std::vector<unsigned int>		indices;

	DirectX::SimpleMath::Matrix nodeTM;					// Node TransformMatrix

	std::string parentName;		// �θ� �̸�
	std::shared_ptr<FBXMeshInfo> parentMesh;	// �θ��� �޽�

	std::vector<std::shared_ptr<FBXMeshInfo>> childMeshList;	// �ڽ� �޽õ�

	bool isParent;				// �θ� �����ϴ���

	bool isSkinned = false;		// ��Ű�׿�����Ʈ ���� �ƴ���..
};

struct FBXBoneInfo
{
	std::string				boneName;
	int						boneID;

	std::string				parentBoneName;
	int						parentIndex;

	int						depth = 0;

	std::vector<std::shared_ptr<FBXBoneInfo>> childBoneList;	// �ڽ� ��� ����Ʈ

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
	
	float	ticksPerFrame;	// ƽ ��
	int		totalKeyFrame;	// �� ������
	int		startKeyFrame;	// ���� ������
	int		endKeyFrame;	// ������ ������

	float	duration;		// �ӵ�

	std::vector<std::shared_ptr<FBXKeyFrameInfo>> keyFrameList;	 //	Ű������ ����Ʈ
};

class FBXModel
{
public:
	std::vector<std::shared_ptr<FBXMeshInfo>> fbxMeshInfoList;		

	std::vector<std::shared_ptr<FBXMaterialInfo>>	materialList;
	
	std::shared_ptr<FBXSkeletonInfo> fbxSkeletonInfo;
	
	std::vector<std::shared_ptr<FBXAnimClipInfo>> animationClipList;

	bool isSkinnedAnimation;		// ��Ű�� �ִϸ��̼� ���� ����
};

