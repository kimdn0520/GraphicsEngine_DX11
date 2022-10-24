#pragma once
#include "GameEngineDLL.h"
#include "Component.h"

class MonoBehaviour;
class Camera;
class Transform;
class UI;
class Text;
class Animator;

enum class Tag
{
	Default,
	Player,
	Enemy,
};

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	GameEngine_DLL GameObject();
	GameEngine_DLL virtual ~GameObject();

private:
	Tag _tag = Tag::Default;
	
	std::string _name;

	std::array<std::shared_ptr<Component>, COMPONENT_COUNT> _components;	// ���ӿ�����Ʈ�� �����ִ� ������Ʈ��
	
	std::shared_ptr<MonoBehaviour> _script;									// ���ӿ�����Ʈ�� ��ũ��Ʈ�� �ϳ��� ����.

	int _objectID = 0;

public:
	bool isCheckFrustum = false;	// �������� üũ�ؾ��� ������Ʈ�ΰ�?

	bool activeSelf = true;			// ���ӿ�����Ʈ�� �����ִ°� �����ִ°�

public:
	// ���ӿ�����Ʈ�� �޸� ������Ʈ�� ���ü��־��
	template <typename TComponent>
	std::shared_ptr<TComponent> GetComponent();

	// ���ӿ�����Ʈ�� �޸� ��ũ��Ʈ�� ���ðſ���~
	template <typename TComponent>
	std::shared_ptr<TComponent> GetScript();

	// ������Ʈ�� �߰��Ҽ��־��!
	template <typename TComponent>
	void AddComponent();

	GameEngine_DLL std::shared_ptr<Camera> GetCamera();

	GameEngine_DLL std::shared_ptr<Transform> GetTransform();

	GameEngine_DLL std::shared_ptr<UI> GetUI();

	GameEngine_DLL std::shared_ptr<Text> GetText();

	GameEngine_DLL std::shared_ptr<Animator> GetAnimator();

	std::string GetName() { return _name; }

	GameEngine_DLL void SetName(std::string name)
	{
		_name = name;
	};

	GameEngine_DLL Tag GetTag() { return _tag; }

	GameEngine_DLL void SetTag(Tag tag) { _tag = tag; }

	int GetObjectID() { return _objectID; }

	std::vector<std::shared_ptr<GameObject>> _childs;

	std::vector<std::shared_ptr<GameObject>>& GetChilds() { return _childs; }

	GameEngine_DLL void PlayAnim(std::shared_ptr<GameObject> gameObject, std::string name, bool isLoop);

	GameEngine_DLL void SetChild(std::shared_ptr<GameObject> child)
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
inline std::shared_ptr<TComponent> GameObject::GetComponent()
{
	for (int i = 0; i < COMPONENT_COUNT; i++)
	{
		std::shared_ptr<TComponent> castedComponent = dynamic_pointer_cast<TComponent>(_components[i]);

		if (castedComponent != nullptr)
			return castedComponent;
	}

	return nullptr;
}

template<typename TComponent>
inline std::shared_ptr<TComponent> GameObject::GetScript()
{
	std::shared_ptr<TComponent> castedComponent = dynamic_pointer_cast<TComponent>(_script);

	if (castedComponent != nullptr)
		return castedComponent;

	return nullptr;
}

template<typename TComponent>
inline void GameObject::AddComponent()
{
	shared_ptr<TComponent> newComponent = make_shared<TComponent>(this->shared_from_this());	// this�� ���ӿ�����Ʈ�� ������Ʈ�� �Ѱ��ش�.

	int typeIndex = (int)newComponent->GetType();

	if (typeIndex < COMPONENT_COUNT)
	{
		_components[typeIndex] = newComponent;
	}
	else
	{
		_script = dynamic_pointer_cast<MonoBehaviour>(newComponent);
	}
}
