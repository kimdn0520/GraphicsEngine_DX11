#include "pch.h"
#include "GameObject.h"
#include "MonoBehaviour.h"

GameObject::GameObject()
	: _name("")
{}

GameObject::~GameObject()
{}

void GameObject::Awake()
{
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