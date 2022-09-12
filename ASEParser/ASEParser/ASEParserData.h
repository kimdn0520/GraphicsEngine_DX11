#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include "SimpleMath.h"

namespace ASEData
{
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
}

namespace ASEData
{
	struct ASEFrameData
	{
		std::string			fileName;
		int					firstFrame;
		int					lastFrame;
		int					frameSpeed;
		int					ticksPerFrame;
	};

	struct ASEMaterialInfo
	{
		bool isDiffuse = false;
		bool isNormal = false;
		bool isSpecular = false;

		DirectX::SimpleMath::Vector4			diffuse;
		DirectX::SimpleMath::Vector4			ambient;
		DirectX::SimpleMath::Vector4			specular;

		std::string		name;
		std::wstring	diffuseTexName;
		std::wstring	normalTexName;
		std::wstring	specularTexName;
	};

	struct Animation_pos
	{
		int			tick;
		DirectX::SimpleMath::Vector4		pos;
	};

	struct Animation_rot
	{
		int			tick;
		DirectX::SimpleMath::Vector4		rotQT;		// 상대변량이기 때문에 이전 값들과 누적해서 가지고 있는다 (즉 지금까지의 절대량으로 변환한다)
	};

	struct Animation_scl
	{
		int			time;
		DirectX::SimpleMath::Vector3		scale;
		DirectX::SimpleMath::Vector3		scaleaxis;
		float		scaleaxisang;
		//Quaternion	m_scalerotQT_accumulation;
	};

	///--------------------------------------------------
	/// 위 클래스를 조합한 Animation 클래스
	///--------------------------------------------------
	class ASEAnimation
	{

	public:
		std::string		nodename;		// 그냥 오브젝트 이름
		std::string		animName;		// 애니메이션 이름
		std::vector<Animation_pos*>		position;
		std::vector<Animation_rot*>		rotation;
		std::vector<Animation_scl*>		scale;

		//// 애니메이션 firstframe
		//int firstFrame;

		//// 애니메이션 총 프레임 수
		//int totalFrame;

		//// 애니메이션 초당 프레임 수
		//int frameSpeed;

		//// 프레임당 몇 Tick이 지나가는지
		//int ticksPerFrame;
	};

	struct ASEBoneInfo
	{
		std::string						boneName;
	};

	struct ASEMeshInfo
	{
		std::string						meshName;
		std::string						parentName;

		std::vector<Vertex>				vertices;
		std::vector<unsigned int>		indices;

		unsigned int materialID;

		DirectX::SimpleMath::Matrix nodeTM;

		bool isAnimation;	// 애니메이션 보유 여부

		std::vector<ASEAnimation*> animationClips;

		std::vector<ASEBoneInfo*> boneInfos;
	};

}

namespace ASEData
{
	class ASEModel
	{
	public:
		ASEFrameData* frameData;

		std::vector<ASEMeshInfo*> meshInfos;

		std::vector<ASEMaterialInfo*> materials;
	};
}