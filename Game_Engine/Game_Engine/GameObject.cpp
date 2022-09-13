#include "pch.h"
#include "GameObject.h"
#include "MonoBehaviour.h"
#include "Component.h"
#include "Camera.h"
#include "Transform.h"
#include "UI.h"
#include "Text.h"

GameObject::GameObject()
	: _name("")
{}

GameObject::~GameObject()
{}

GameEngine_DLL Camera* GameObject::GetCamera()
{
	Component* component = _components[(int)COMPONENT_TYPE::CAMERA];

	return dynamic_cast<Camera*>(component);
}

GameEngine_DLL Transform* GameObject::GetTransform()
{
	Component* component = _components[(int)COMPONENT_TYPE::TRANSFORM];

	return dynamic_cast<Transform*>(component);
}

GameEngine_DLL UI* GameObject::GetUI()
{
	Component* component = _components[(int)COMPONENT_TYPE::UI];

	return dynamic_cast<UI*>(component);
}

GameEngine_DLL Text* GameObject::GetText()
{
	Component* component = _components[(int)COMPONENT_TYPE::TEXT];

	return dynamic_cast<Text*>(component);
}

void GameObject::Awake()
{
	static int objectCount = 0;

	_objectID = objectCount++;

	if (_script != nullptr)
		_script->Awake();

	for (Component* component : _components)
	{
		if (component)
			component->Awake();
	}
}

void GameObject::Start()
{
	if (_script != nullptr)
		_script->Start();

	for (Component* component : _components)
	{
		if (component)
			component->Start();
	}
}

void GameObject::FixedUpdate()
{
	if (_script != nullptr)
		_script->FixedUpdate();

	for (Component* component : _components)
	{
		if (component)
			component->FixedUpdate();
	}
}

void GameObject::Update()
{
	if (_script != nullptr)
		_script->Update();

	for (Component* component : _components)
	{
		if (component)
			component->Update();
	}
}

void GameObject::LateUpdate()
{
	if (_script != nullptr)
		_script->LateUpdate();

	for (Component* component : _components)
	{
		if (component)
			component->LateUpdate();
	}
}

void GameObject::Render()
{
	for (Component* component : _components)
	{
		if (component)
			component->Render();
	}
}

void GameObject::SetActive(bool value)
{
	// 나자신 value 넣구
	activeSelf = value;

	// 하위 오브젝트 value 재귀로 넣주구~
	for (auto& child : _childs)
	{
		child->SetActive(value);
	}
}