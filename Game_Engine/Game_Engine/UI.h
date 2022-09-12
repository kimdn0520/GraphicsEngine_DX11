#pragma once
#include "GameEngineDLL.h"
#include "Component.h"

class Transform;

class UI : public Component
{
public:
	GameEngine_DLL UI(GameObject* gameObject);
	GameEngine_DLL virtual ~UI();

protected:
	int _sortLayer = 0;

public:
	void SetSortLayer(int layer) { _sortLayer = layer; }
};

