#pragma once
#include "GameEngineDLL.h"
#include "Component.h"

class MonoBehaviour;
class Camera;
class Transform;
class UI;
class Text;

enum class Tag
{
	Default,
	Player,
	Enemy,
};

class GameObject
{
public:
	GameEngine_DLL GameObject();
	GameEngine_DLL virtual ~GameObject();

private:
	Tag _tag = Tag::Default;
	
	std::string _name;

	std::array<Component*, COMPONENT_COUNT> _components;	// 게임오브젝트가 갖고있는 컴포넌트들
	
	MonoBehaviour* _script;									// 게임오브젝트당 스크립트는 하나만 들어간다.

	int _objectID = 0;

public:
	bool isCheckFrustum = false;	// 프러스텀 체크해야할 오브젝트인가?

	bool activeSelf = true;			// 게임오브젝트가 켜져있는가 꺼져있는가

public:
	// 게임오브젝트에 달린 컴포넌트를 얻어올수있어욤
	template <typename TComponent>
	TComponent* GetComponent();

	// 게임오브젝트에 달린 스크립트를 얻어올거에여~
	template <typename TComponent>
	TComponent* GetScript();

	// 컴포넌트를 추가할수있어요!
	template <typename TComponent>
	void AddComponent();

	GameEngine_DLL Camera* GetCamera();

	GameEngine_DLL Transform* GetTransform();

	GameEngine_DLL UI* GetUI();

	GameEngine_DLL Text* GetText();

	std::string GetName() { return _name; }

	GameEngine_DLL void SetName(std::string name)
	{
		_name = name;
	};

	GameEngine_DLL Tag GetTag() { return _tag; }

	GameEngine_DLL void SetTag(Tag tag) { _tag = tag; }

	int GetObjectID() { return _objectID; }

	std::vector<GameObject*> _childs;

	std::vector<GameObject*>& GetChilds() { return _childs; }

	GameEngine_DLL void PlayAnim(GameObject* gameObject, std::string name, bool isLoop);

	GameEngine_DLL void SetChild(GameObject* child)
	{
		_childs.push_back(child);
	};

	GameEngine_DLL void SetActive(bool value);

public:
	/*void OnTriggerEnter(ColliderBase* other);

	void OnTriggerStay(ColliderBase* other);

	void OnTriggerExit(ColliderBase* other);*/

public:
	void Awake();
	void Start();
	void FixedUpdate();
	void Update();
	void LateUpdate();

	void Render();
};

template<typename TComponent>
inline TComponent* GameObject::GetComponent()
{
	for (int i = 0; i < COMPONENT_COUNT; i++)
	{
		TComponent* castedComponent = dynamic_cast<TComponent*>(_components[i]);

		if (castedComponent != nullptr)
			return castedComponent;
	}

	return nullptr;
}

template<typename TComponent>
inline TComponent* GameObject::GetScript()
{
	TComponent* castedComponent = dynamic_cast<TComponent*>(_script);

	if (castedComponent != nullptr)
		return castedComponent;

	return nullptr;
}

template<typename TComponent>
inline void GameObject::AddComponent()
{
	TComponent* newComponent = new TComponent(this);	// this로 게임오브젝트도 컴포넌트에 넘겨준다.

	int typeIndex = (int)newComponent->GetType();

	if (typeIndex < COMPONENT_COUNT)
	{
		_components[typeIndex] = newComponent;
	}
	else
	{
		_script = dynamic_cast<MonoBehaviour*>(newComponent);
	}
}
