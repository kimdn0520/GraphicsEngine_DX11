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
		DirectX::SimpleMath::Vector4		rotQT;		// ��뺯���̱� ������ ���� ����� �����ؼ� ������ �ִ´� (�� ���ݱ����� ���뷮���� ��ȯ�Ѵ�)
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
	/// �� Ŭ������ ������ Animation Ŭ����
	///--------------------------------------------------
	class ASEAnimation
	{

	public:
		std::string		nodename;		// �׳� ������Ʈ �̸�
		std::string		animName;		// �ִϸ��̼� �̸�
		std::vector<Animation_pos*>		position;
		std::vector<Animation_rot*>		rotation;
		std::vector<Animation_scl*>		scale;

		//// �ִϸ��̼� firstframe
		//int firstFrame;

		//// �ִϸ��̼� �� ������ ��
		//int totalFrame;

		//// �ִϸ��̼� �ʴ� ������ ��
		//int frameSpeed;

		//// �����Ӵ� �� Tick�� ����������
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

		bool isAnimation;	// �ִϸ��̼� ���� ����

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