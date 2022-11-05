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
		DirectX::SimpleMath::Vector4 paColor = { 0.0f, 0.0f, 0.0f, 1.0f },
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

	std::wstring albedoMap;
	std::wstring normalMap;
	std::wstring metallicMap;
	std::wstring roughnessMap;
	std::wstring AOMap;
	std::wstring emissiveMap;

	DirectX::SimpleMath::Vector4 material_Ambient;
	DirectX::SimpleMath::Vector4 material_Diffuse;
	DirectX::SimpleMath::Vector4 material_Specular;
	DirectX::SimpleMath::Vector4 material_Emissive;

	float material_Transparency;	// ����
	float material_Reflectivity;	// �ݻ���

	float metallic;					// �ݼӼ�
	float roughness;				// ��ĥ��
};

struct FBXMeshInfo
{
	std::string						meshName;			// mesh �̸�
	std::string						parentName;			// �θ� �̸�
	std::string						materialName;		// �� mesh�� �ش��ϴ� material �̸�!
	std::vector<Vertex>				meshVertexList;
	std::vector<unsigned int>		indices;

	DirectX::SimpleMath::Matrix nodeTM;					// Node TransformMatrix

	bool isSkinned = false;		// ��Ű�׿�����Ʈ ���� �ƴ���..
};

struct FBXBoneInfo
{
	std::string				boneName;

	int						parentIndex;

	DirectX::SimpleMath::Matrix offsetMatrix;				// Bone's OffsetMatrix
	DirectX::SimpleMath::Matrix nodeMatrix;					// Bone's NodeMatrix
};

struct FBXKeyFrameInfo
{
	float		time;

	DirectX::SimpleMath::Vector3 localTransform;
	DirectX::SimpleMath::Quaternion localRotation;
	DirectX::SimpleMath::Vector3 localScale;
};

struct FBXAnimClipInfo
{
	std::string	animationName;

	float frameRate = 0.0f;

	float tickPerFrame = 0.0f;					
	
	int totalKeyFrame = 0;							

	int startKeyFrame = 0;
	
	int endKeyFrame = 0;

	std::vector<std::vector<std::shared_ptr<FBXKeyFrameInfo>>> keyFrameList;	 //	Ű������ ����Ʈ
};

class FBXModel
{
public:
	std::vector<std::shared_ptr<FBXMeshInfo>> fbxMeshInfoList;		

	std::vector<std::shared_ptr<FBXMaterialInfo>>	materialList;

	std::vector<std::shared_ptr<FBXBoneInfo>> fbxBoneInfoList;

	std::vector<std::shared_ptr<FBXAnimClipInfo>> animationClipList;

	bool isSkinnedAnimation;		// ��Ű�� �ִϸ��̼� ���� ����
};

