#pragma once
#include "pch.h"

namespace YAMLBinaryData
{
	struct Float2
	{
		friend boost::serialization::access;

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
		friend boost::serialization::access;

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
		friend boost::serialization::access;

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

	class Transform
	{
	public:
		friend boost::serialization::access;

		Transform(
			Float3 _localPosition = { 0.f, 0.f, 0.f }, 
			Float3 _localRotation = { 0.f, 0.f, 0.f },
			Float3 _localScale = { 1.f, 1.f, 1.f }
			)
			: localPosition(std::move(_localPosition))
			, localRotation(std::move(_localRotation))
			, localScale(std::move(_localScale))
		{}

	public:
		Float3 localPosition;
		Float3 localRotation;
		Float3 localScale;

	private:
		template<typename Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar& localPosition;
			ar& localRotation;
			ar& localScale;
		}
		
	};

	class BoxCollider
	{
	public:
		friend boost::serialization::access;

		BoxCollider() = default;

		BoxCollider(
			Float3 _size = { 1.f, 1.f, 1.f },
			Float3 _center = { 0.f, 0.f, 0.f }
		)
			: size(std::move(_size))
			, center(std::move(_center))
		{}

	public:
		Float3 size;
		Float3 center;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& size;
			ar& center;
		}

	};

	class GameObject
	{
	public:
		friend boost::serialization::access;

		GameObject() = default;

		GameObject(std::string _name, Transform _transform)
			: name(std::move(_name))
			, transform(std::move(_transform))
		{}

	public:
		std::string name = "";
		Transform transform;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& name;
			ar& transform;
		}
	};

	class Prefab
	{
	public:
		friend boost::serialization::access;

		Prefab() = default;

		Prefab(std::string _name, Transform _transform)
			: name(std::move(_name))
			, transform(std::move(_transform))
		{}

	public:
		std::string name = "";
		Transform transform;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& name;
			ar& transform;
		}
	};

	class Scene
	{
	public:
		friend boost::serialization::access;

		Scene() = default;

		Scene(
			vector<GameObject> _gameObjects,
			vector<Prefab> _prefabs
		)
			: gameObjects(std::move(_gameObjects))
			, prefabs(std::move(_prefabs))
		{}

	public:
		// 근데 거의 카메라 정도가 끝일듯?
		vector<GameObject> gameObjects;

		vector<Prefab> prefabs;
		
	private:
		template<typename Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar& gameObjects;
			ar& prefabs;
		}
	};
}

