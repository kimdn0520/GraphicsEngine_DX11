#include "pch.h"
#include "Text.h"
#include "GameObject.h"

Text::Text(std::shared_ptr<GameObject> gameObject)
	: Component(gameObject, COMPONENT_TYPE::TEXT),
	_textInfo(std::make_shared<TextInfo>())
{
	_textInfo->scale = 1.0f;		// 일단 기본 크기 1.0f으로 해놓았습니다.
}

Text::~Text()
{}

void Text::SetLocation(int x, int y)
{
	_textInfo->x = x;
	_textInfo->y = y;
}

void Text::Render()
{
	_textInfo->x = _x;
	_textInfo->y = _y;

	//GraphicsManager::GetInstance()->SetTextInfo(_textInfo);
}
