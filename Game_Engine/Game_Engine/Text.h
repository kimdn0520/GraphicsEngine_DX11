#pragma once
#include "GameEngineDLL.h"
#include "Component.h"
#include "Graphics_RenderingData.h"

class Text : public Component
{
public:
	GameEngine_DLL Text(GameObject* gameObject);
	GameEngine_DLL virtual ~Text();

private:
	TextInfo* _textInfo;

	int _x = 0;
	int _y = 0;

public:
	GameEngine_DLL TextInfo* GetTextInfo() { return _textInfo; }

	GameEngine_DLL void SetLocation(int x, int y);

public:

	virtual void Render() override;
};

