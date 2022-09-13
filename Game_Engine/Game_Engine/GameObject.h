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

	std::array<Component*, COMPONENT_COUNT> _components;	// ���ӿ�����Ʈ�� �����ִ� ������Ʈ��
	
	MonoBehaviour* _script;									// ���ӿ�����Ʈ�� ��ũ��Ʈ�� �ϳ��� ����.

	int _objectID = 0;

public:
	bool isCheckFrustum = false;	// �������� üũ�ؾ��� ������Ʈ�ΰ�?

	bool activeSelf = true;			// ���ӿ�����Ʈ�� �����ִ°� �����ִ°�

public:
	// ���ӿ�����Ʈ�� �޸� ������Ʈ�� ���ü��־��
	template <typename TComponent>
	TComponent* GetComponent();

	// ���ӿ�����Ʈ�� �޸� ��ũ��Ʈ�� ���ðſ���~
	template <typename TComponent>
	TComponent* GetScript();

	// ������Ʈ�� �߰��Ҽ��־��!
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
	TComponent* newComponent = new TComponent(this);	// this�� ���ӿ�����Ʈ�� ������Ʈ�� �Ѱ��ش�.

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
