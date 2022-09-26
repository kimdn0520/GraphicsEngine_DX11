#pragma once
#include "GameEngineDLL.h"
#include "Component.h"
#include "Graphics_RenderingData.h"

class Text : public Component
{
public:
	GameEngine_DLL Text(std::shared_ptr<GameObject> gameObject);
	GameEngine_DLL virtual ~Text();

private:
	std::shared_ptr<TextInfo> _textInfo;

	int _x = 0;
	int _y = 0;

public:
	GameEngine_DLL std::shared_ptr<TextInfo> GetTextInfo() { return _textInfo; }

	GameEngine_DLL void SetLocation(int x, int y);

public:

	virtual void Render() override;
};

