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
	short boneIndices[8];
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

	DirectX::SimpleMath::Vector4 material_Ambient = { 0.f, 0.f, 0.f, 0.f };
	DirectX::SimpleMath::Vector4 material_Diffuse = { 0.f, 0.f, 0.f, 0.f };
	DirectX::SimpleMath::Vector4 material_Specular = { 0.f, 0.f, 0.f, 0.f };
	DirectX::SimpleMath::Vector4 material_Emissive = { 0.f, 0.f, 0.f, 0.f };

	float emissionFactor = 0.f;			// ���� ũ��
	float material_Transparency = 0.f;	// ����
	float material_Reflectivity = 0.f;	// �ݻ���

	float metallic = 0.f;				// �ݼӼ�
	float roughness = 0.f;				// ��ĥ��
};

struct FBXMeshInfo
{
	std::string						meshName;			// mesh �̸�
	std::string						parentName;			// �θ� �̸�
	std::vector<std::string>		materials;			// �� mesh�� �ش��ϴ� material �̸�!
	std::vector<Vertex>				meshVertexList;
	std::vector<std::vector<unsigned int>>	indices;

	DirectX::SimpleMath::Matrix nodeTM = DirectX::XMMatrixIdentity();			// Node TransformMatrix

	bool isSkinned = false;		// ��Ű�׿�����Ʈ ���� �ƴ���..
};

struct FBXBoneInfo
{
	std::string				boneName;

	int						parentIndex;

	DirectX::SimpleMath::Matrix offsetMatrix = DirectX::XMMatrixIdentity();		// Bone's OffsetMatrix
	DirectX::SimpleMath::Matrix nodeMatrix = DirectX::XMMatrixIdentity();		// Bone's NodeMatrix
};

struct FBXKeyFrameInfo
{
	float		time;

	DirectX::SimpleMath::Vector3 localTransform = { 0.f, 0.f, 0.f };
	DirectX::SimpleMath::Quaternion localRotation = { 0.f, 0.f, 0.f, 0.f };
	DirectX::SimpleMath::Vector3 localScale = { 0.f, 0.f, 0.f };
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

