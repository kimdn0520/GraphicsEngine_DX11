#include "pch.h"
#include "Component.h"
#include "GameObject.h"

Component::Component(std::shared_ptr<GameObject> gameObject, COMPONENT_TYPE type)
	: _gameObject(gameObject), _type(type)
{}

Component::~Component()
{}
