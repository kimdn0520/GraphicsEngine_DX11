#include "pch.h"
#include "MonoBehaviour.h"

MonoBehaviour::MonoBehaviour(std::shared_ptr<GameObject> gameObject)
	: Component(gameObject, COMPONENT_TYPE::MONO_BEHAVIOUR)
{}

MonoBehaviour::~MonoBehaviour()
{}