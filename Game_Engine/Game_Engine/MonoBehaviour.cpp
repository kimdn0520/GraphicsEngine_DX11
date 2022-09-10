#include "pch.h"
#include "MonoBehaviour.h"

MonoBehaviour::MonoBehaviour(GameObject* gameObject)
	: Component(gameObject, COMPONENT_TYPE::MONO_BEHAVIOUR)
{}

MonoBehaviour::~MonoBehaviour()
{}