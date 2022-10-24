#include "pch.h"
#include "GameObject.h"
#include "MonoBehaviour.h"
#include "Component.h"
#include "Camera.h"
#include "Transform.h"
#include "Animator.h"
#include "UI.h"
#include "Text.h"

GameObject::GameObject()
	: _name("")
{}

GameObject::~GameObject()
{}

shared_ptr<Camera> GameObject::GetCamera()
{
	shared_ptr<Component> component = _components[(int)COMPONENT_TYPE::CAMERA];

	return dynamic_pointer_cast<Camera>(component);
}

shared_ptr<Transform> GameObject::GetTransform()
{
	shared_ptr<Component> component = _components[(int)COMPONENT_TYPE::TRANSFORM];

	return dynamic_pointer_cast<Transform>(component);
}

shared_ptr<UI> GameObject::GetUI()
{
	shared_ptr<Component> component = _components[(int)COMPONENT_TYPE::UI];

	return dynamic_pointer_cast<UI>(component);
}

shared_ptr<Text> GameObject::GetText()
{
	shared_ptr<Component> component = _components[(int)COMPONENT_TYPE::TEXT];

	return dynamic_pointer_cast<Text>(component);
}

shared_ptr<Animator> GameObject::GetAnimator()
{
	shared_ptr<Component> component = _components[(int)COMPONENT_TYPE::ANIMATOR];

	return dynamic_pointer_cast<Animator>(component);
}

void GameObject::Awake()
{
	static int objectCount = 0;

	_objectID = objectCount++;

	if (_script != nullptr)
		_script->Awake();

	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Awake();
	}
}

void GameObject::Start()
{
	if (_script != nullptr)
		_script->Start();

	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Start();
	}
}

void GameObject::FixedUpdate()
{
	if (_script != nullptr)
		_script->FixedUpdate();

	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->FixedUpdate();
	}
}

void GameObject::Update()
{
	if (_script != nullptr)
		_script->Update();

	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Update();
	}
}

void GameObject::LateUpdate()
{
	if (_script != nullptr)
		_script->LateUpdate();

	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->LateUpdate();
	}
}

void GameObject::Render()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Render();
	}
}

void GameObject::PlayAnim(std::shared_ptr<GameObject> gameObject, std::string name, bool isLoop)
{
	if (gameObject->GetAnimator() != nullptr)
	{
		gameObject->GetAnimator()->Play(name, isLoop);
	}

	if (gameObject->GetChilds().size() != 0)
	{
		for (auto& gameObj : gameObject->GetChilds())
		{
			if (gameObj->GetAnimator() != nullptr)
			{
				gameObj->GetAnimator()->Play(name, isLoop);
			}

			PlayAnim(gameObj, name, isLoop);
		}
	}
}

void GameObject::SetActive(bool value)
{
	// ���ڽ� value �ֱ�
	activeSelf = value;

	// ���� ������Ʈ value ��ͷ� ���ֱ�~
	for (auto& child : _childs)
	{
		child->SetActive(value);
	}
}