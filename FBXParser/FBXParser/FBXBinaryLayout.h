#pragma once
#include "pch.h"

namespace FBXBinaryData
{
	struct Float2
	{
		Float2() = default;

		Float2(float _x, float _y)
			: x(_x), y(_y)
		{}

		float x;
		float y;

		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& x;
			ar& y;
		}
	};

	struct Float3
	{
		Float3() = default;

		Float3(float _x, float _y, float _z)
			: x(_x), y(_y), z(_z)
		{}

		float x;
		float y;
		float z;

		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& x;
			ar& y;
			ar& z;
		}
	};

	struct Float4
	{
		Float4() = default;

		Float4(float _x, float _y, float _z, float _w)
			: x(_x), y(_y), z(_z), w(_w)
		{}

		float x;
		float y;
		float z;
		float w;

		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& x;
			ar& y;
			ar& z;
			ar& w;
		}
	};

	struct Float4x4
	{
		union
		{
			struct
			{
				float m00, m01, m02, m03;
				float m10, m11, m12, m13;
				float m20, m21, m22, m23;
				float m30, m31, m32, m33;
			};

			float m[4][4];
		};

		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& m;
		}
	};

	struct VertexData
	{
		VertexData() = default;

		VertexData(
			Float3 _position = { 0.f, 0.f, 0.f },
			Float4 _color = { 1.0f, 1.0f, 1.0f, 1.0f },
			Float2 _uv = { 0.f, 0.f },
			Float3 _normal = { 0.f, 0.f, 0.f },
			Float3 _tangent = { 0.f, 0.f, 0.f })
			: position(_position), normal(_normal), uv(_uv), tangent(_tangent), color(_color)
		{
			for (int i = 0; i < 8; i++)
			{
				weights[i] = 0.f;
				boneIndices[i] = -1;
			}
		}

		Float3 position;
		Float4 color;
		Float2 uv;
		Float3 normal;
		Float3 tangent;

		// Skinned Data
		float weights[8];
		int boneIndices[8];

		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& position;
			ar& color;
			ar& uv;
			ar& normal;
			ar& tangent;
			ar& weights;
			ar& boneIndices;
		}
	};

	class MaterialData
	{
	public:
		friend boost::serialization::access;

		MaterialData() = default;

		MaterialData(std::string _materialName, std::wstring _albedoMap, std::wstring _normalMap, std::wstring _metallicMap, std::wstring _roughnessMap
			, std::wstring _AOMap, std::wstring _emissiveMap, Float4 _material_Ambient, Float4 _material_Diffuse, Float4 _material_Specular, Float4 _material_Emissive
			, float _material_Transparency, float _material_Reflectivity, float _metallic, float _roughness)
			: materialName(std::move(_materialName))
			, albedoMap(std::move(_albedoMap))
			, normalMap(std::move(_normalMap))
			, metallicMap(std::move(_metallicMap))
			, roughnessMap(std::move(_roughnessMap))
			, AOMap(std::move(_AOMap))
			, emissiveMap(std::move(_emissiveMap))
			, material_Ambient(std::move(_material_Ambient))
			, material_Diffuse(std::move(_material_Diffuse))
			, material_Specular(std::move(_material_Specular))
			, material_Emissive(std::move(_material_Emissive))
			, material_Transparency(std::move(_material_Transparency))
			, material_Reflectivity(std::move(_material_Reflectivity))
			, metallic(std::move(_metallic))
			, roughness(std::move(_roughness))
		{}

	private:
		std::string materialName;

		std::wstring albedoMap;
		std::wstring normalMap;
		std::wstring metallicMap;
		std::wstring roughnessMap;
		std::wstring AOMap;
		std::wstring emissiveMap;

		Float4 material_Ambient;
		Float4 material_Diffuse;
		Float4 material_Specular;
		Float4 material_Emissive;

		float material_Transparency;
		float material_Reflectivity;

		float metallic;
		float roughness;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& materialName;

			ar& albedoMap;
			ar& normalMap;
			ar& metallicMap;
			ar& roughnessMap;
			ar& AOMap;
			ar& emissiveMap;

			ar& material_Ambient;
			ar& material_Diffuse;
			ar& material_Specular;
			ar& material_Emissive;

			ar& material_Transparency;
			ar& material_Reflectivity;

			ar& metallic;
			ar& roughness;
		}
	};

	class MeshData
	{
	public:
		friend boost::serialization::access;

		MeshData() = default;

		MeshData(std::string _meshName, std::string _parentName, std::string _materialName, std::vector<VertexData> _meshVertexList
			, std::vector<unsigned int> _indices, Float4x4 _nodeTM, bool _isSkinned = false)
			: meshName(std::move(_meshName))
			, parentName(std::move(_parentName))
			, materialName(std::move(_materialName))
			, meshVertexList(std::move(_meshVertexList))
			, indices(std::move(_indices))
			, nodeTM(std::move(_nodeTM))
			, isSkinned(std::move(_isSkinned))
		{}

	private:
		std::string						meshName;
		std::string						parentName;
		std::string						materialName;
		std::vector<VertexData>			meshVertexList;
		std::vector<unsigned int>		indices;

		Float4x4 nodeTM;

		bool isSkinned = false;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& meshName;
			ar& parentName;
			ar& materialName;
			ar& meshVertexList;
			ar& indices;
			ar& nodeTM;
			ar& isSkinned;
		}
	};

	class BoneData
	{
	public:
		friend boost::serialization::access;

		BoneData() = default;

		BoneData(std::string _boneName, int _parentIndex, Float4x4 _offsetMatrix, Float4x4 _nodeMatrix)
			: boneName(std::move(_boneName))
			, parentIndex(std::move(_parentIndex))
			, offsetMatrix(std::move(_offsetMatrix))
			, nodeMatrix(std::move(_nodeMatrix))
		{}

	private:
		std::string	boneName;

		int			parentIndex;

		Float4x4	offsetMatrix;
		Float4x4	nodeMatrix;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boneName;
			ar& parentIndex;
			ar& offsetMatrix;
			ar& nodeMatrix;
		}
	};

	class KeyFrameInfoData
	{
	public:
		friend boost::serialization::access;

		KeyFrameInfoData() = default;

		KeyFrameInfoData(float _time, Float3 _localTransform, Float4 _localRotation, Float3 _localScale)
			: time(std::move(_time))
			, localTransform(std::move(_localTransform))
			, localRotation(std::move(_localRotation))
			, localScale(std::move(_localScale))
		{}

	private:
		float  time;

		Float3 localTransform;
		Float4 localRotation;
		Float3 localScale;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& time;
			ar& localTransform;
			ar& localRotation;
			ar& localScale;
		}
	};

	class AnimationClipData
	{
	public:
		friend boost::serialization::access;

		AnimationClipData() = default;

		AnimationClipData(std::string _animationName, float _frameRate, float _tickPerFrame, int _totalKeyFrame, int _startKeyFrame, int _endKeyFrame
			, std::vector<std::vector<std::shared_ptr<KeyFrameInfoData>>> _keyFrameList)
			: animationName(std::move(_animationName))
			, frameRate(std::move(_frameRate))
			, tickPerFrame(std::move(_tickPerFrame))
			, totalKeyFrame(std::move(_totalKeyFrame))
			, startKeyFrame(std::move(_startKeyFrame))
			, endKeyFrame(std::move(_endKeyFrame))
			, keyFrameList(std::move(_keyFrameList))
		{}

	private:
		std::string	animationName;

		float frameRate = 0.0f;

		float tickPerFrame = 0.0f;

		int totalKeyFrame = 0;

		int startKeyFrame = 0;

		int endKeyFrame = 0;

		std::vector<std::vector<std::shared_ptr<KeyFrameInfoData>>> keyFrameList;	 

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& animationName;
			ar& frameRate;
			ar& tickPerFrame;
			ar& totalKeyFrame;
			ar& startKeyFrame;
			ar& endKeyFrame;
			ar& keyFrameList;
		}
	};

	class ModelData
	{
	public:
		friend boost::serialization::access;

		ModelData() = default;

		ModelData(std::vector<std::shared_ptr<MeshData>> _meshInfoList, std::vector<std::shared_ptr<MaterialData>> _materialList
		, std::vector<std::shared_ptr<BoneData>> _boneInfoList, std::vector<std::shared_ptr<AnimationClipData>> _animationList
		, bool isSkinnedAnimation = false)
			: meshInfoList(std::move(_meshInfoList))
			, materialList(std::move(_materialList))
			, boneInfoList(std::move(_boneInfoList))
			, animationClipList(std::move(_animationList))
			, isSkinnedAnimation(std::move(isSkinnedAnimation))
		{}

	private:
		std::vector<std::shared_ptr<MeshData>> meshInfoList;

		std::vector<std::shared_ptr<MaterialData>>	materialList;

		std::vector<std::shared_ptr<BoneData>> boneInfoList;

		std::vector<std::shared_ptr<AnimationClipData>> animationClipList;

		bool isSkinnedAnimation;		

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& meshInfoList;
			ar& materialList;
			ar& boneInfoList;
			ar& animationClipList;
			ar& isSkinnedAnimation;
		}
	};
}

