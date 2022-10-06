#pragma once
#include <algorithm>
#include "SimpleMath.h"

struct Vertex
{
	Vertex()
	{
		for (int i = 0; i < 8; i++)
		{
			weights[i] = 0.f;
			boneIndices[i] = 0;
		}
	}
	Vertex(DirectX::SimpleMath::Vector3 paPos,
		DirectX::SimpleMath::Vector4 paColor = { 1.0f, 1.0f, 1.0f, 1.0f },
		DirectX::SimpleMath::Vector2 paUV = { 0.f, 0.f },
		DirectX::SimpleMath::Vector3 paNormal = { 0.f, 0.f, 0.f },
		DirectX::SimpleMath::Vector3 paTangent = { 0.f, 0.f, 0.f })
		: pos(paPos), normal(paNormal), uv(paUV), tangent(paTangent), color(paColor)
	{
		for (int i = 0; i < 8; i++)
		{
			weights[i] = 0.f;
			boneIndices[i] = 0;
		}
	}

	DirectX::SimpleMath::Vector3 pos;
	DirectX::SimpleMath::Vector4 color;
	DirectX::SimpleMath::Vector2 uv;
	DirectX::SimpleMath::Vector3 normal;
	DirectX::SimpleMath::Vector3 tangent;

	// Skinned Data
	float weights[8];
	unsigned __int32 boneIndices[8];
};

struct FBXFace
{
	// Face를 이루는 3개의 Vertex 정보
	int vertexIndex[3];								// 이 Face를 이루는 Vertex의 인덱스
	DirectX::SimpleMath::Vector2 vertexUV[2];		// 이 Face를 이루는 Vertex의 uv
	DirectX::SimpleMath::Vector3 vertexNormal[3];	// 이 Face를 이루는 Vertex의 Normal
	DirectX::SimpleMath::Vector3 vertexTangent[3];	// 이 Face를 이루는 Vertex의 Tangent
};

struct FBXMaterialInfo
{
	std::wstring materialName;

	std::wstring albedoMap;
	std::wstring normalMap;
	std::wstring metallicMap;
	std::wstring roughnessMap;
	std::wstring AOMap;
	std::wstring emissiveMap;

	float metallic;
	float roughness;
};

struct BoneWeight
{
	using Pair = std::pair<int, double>;
	std::vector<Pair> boneWeights;

	void AddWeights(int index, double weight)
	{
		if (weight <= 0.f)
			return;

		auto findIt = std::find_if(boneWeights.begin(), boneWeights.end(),
			[=](const Pair& p) { return p.second < weight; });

		if (findIt != boneWeights.end())
			boneWeights.insert(findIt, Pair(index, weight));
		else
			boneWeights.push_back(Pair(index, weight));

		// 가중치는 최대 8개
		if (boneWeights.size() > 8)
			boneWeights.pop_back();
	}

	void Normalize()
	{
		double sum = 0.f;
		std::for_each(boneWeights.begin(), boneWeights.end(), [&](Pair& p) { sum += p.second; });
		std::for_each(boneWeights.begin(), boneWeights.end(), [=](Pair& p) { p.second = p.second / sum; });
	}
};

struct FBXMeshInfo
{
	std::string						meshName;			// mesh 이름!
	std::string						materialName;		// 이 mesh에 해당하는 material 이름!
	std::vector<Vertex>				meshVertexList;
	std::vector<Vertex>				meshFaceList;

	std::vector<unsigned int>		indices;

	bool isSkinned;			// 스키닝오브젝트 인지 아닌지..
};

struct FBXKeyFrameInfo
{
	DirectX::SimpleMath::Matrix matTransform;

	double		time;
};

struct FBXBoneInfo
{
	std::string				boneName;
	int						boneIndex;

	std::string				parentBoneName;
	int						parentIndex;

	bool isParent;			// 본의 부모가 있는지 없는지..

	std::vector<std::shared_ptr<FBXBoneInfo>> childBoneList;	// 자식 노드 리스트

	DirectX::SimpleMath::Matrix nodeTM;		// Bone's Node Transform Matrix
	DirectX::SimpleMath::Matrix worldTM;	// Bone's World Transform Matrix
	DirectX::SimpleMath::Matrix localTM;	// Bone's Local Transform Matrix
};

// 본들을 갖고있는 스키닝 오브젝트!
struct FBXSkeletonInfo
{
	std::vector<std::shared_ptr<FBXBoneInfo>> fbxBoneInfoList;

	//std::vector<DirectX::SimpleMath::Matrix> boneOffset;

	void AddBone(std::shared_ptr<FBXBoneInfo> bone)
	{
		fbxBoneInfoList.push_back(bone);
	}
};


struct FBXAnimClipInfo
{
	std::string			animationName;
	
	float	ticksPerFrame;
	int		totalKeyFrame;
	int		startKeyFrame;
	int		endKeyFrame;

	std::vector<std::vector<std::shared_ptr<FBXKeyFrameInfo>>> keyFrames;
};

class FBXModel
{
public:
	std::vector<std::shared_ptr<FBXMeshInfo>> fbxMeshInfoList;		

	std::vector<std::shared_ptr<FBXMaterialInfo>>	materialList;
	
	std::shared_ptr<FBXSkeletonInfo> fbxSkeletionInfo;
	
	std::vector<std::shared_ptr<FBXAnimClipInfo>> animationClipList;

	bool isSkinnedAnimation;		// 스키닝 애니메이션 존재 여부
};

