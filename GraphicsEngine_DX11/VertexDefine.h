#pragma once
#include "SimpleMath.h"

namespace Color
{
	extern const __declspec(selectany) DirectX::SimpleMath::Vector4 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	extern const __declspec(selectany) DirectX::SimpleMath::Vector4 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	extern const __declspec(selectany) DirectX::SimpleMath::Vector4 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
}

struct StaticMeshVertex
{
	StaticMeshVertex(
		DirectX::SimpleMath::Vector3 paPos = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f),
		DirectX::SimpleMath::Vector4 paColor = ::Color::Black,
		DirectX::SimpleMath::Vector2 paUV = DirectX::SimpleMath::Vector2(0.f, 1.f),
		DirectX::SimpleMath::Vector3 paNormal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f),
		DirectX::SimpleMath::Vector3 paTangent = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f)) :
		position(paPos), uv(paUV), normal(paNormal), tangent(paTangent), color(paColor) {}

	DirectX::SimpleMath::Vector3 position;

	DirectX::SimpleMath::Vector4 color;
	
	DirectX::SimpleMath::Vector2 uv;

	DirectX::SimpleMath::Vector3 normal;

	DirectX::SimpleMath::Vector3 tangent;
};

struct SkinnedMeshVertex
{
	/*SkinnedMeshVertex()
	{
		for (int i = 0; i < 8; i++)
		{
			weights[i] = 0.f;
			boneIndices[i] = 0;
		}
	}*/

	SkinnedMeshVertex(
		DirectX::SimpleMath::Vector3 paPos = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f),
		DirectX::SimpleMath::Vector4 paColor = ::Color::Black,
		DirectX::SimpleMath::Vector2 paUV = { 0.f, 1.f },
		DirectX::SimpleMath::Vector3 paNormal = { 1.f, 0.f, 0.f },
		DirectX::SimpleMath::Vector3 paTangent = { 0.f, 0.f, 0.f })
	{
		position = paPos; color = paColor; uv = paUV; normal = paNormal; tangent = paTangent;

		for (int i = 0; i < 8; i++)
		{
			weights[i] = 0.f;
			boneIndices[i] = 0;
		}
	}

	DirectX::SimpleMath::Vector3 position;
	
	DirectX::SimpleMath::Vector4 color;
	
	DirectX::SimpleMath::Vector2 uv;
	
	DirectX::SimpleMath::Vector3 normal;
	
	DirectX::SimpleMath::Vector3 tangent;

	// Skinned Data
	float weights[8];
	unsigned __int32 boneIndices[8];
};