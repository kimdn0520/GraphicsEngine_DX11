#include "pch.h"
#include "Graphics_Interface.h"
#include "GraphicsEngine_DX11.h"

Graphics_Interface* Graphics_Interface::graphicsEngine = nullptr;

// GraphicsEngineInterface를 상속받고있는 GraphicsEngine_DX11의 객체 생성
Graphics_Interface* Graphics_Interface::Get()
{
	if (graphicsEngine == nullptr)
		graphicsEngine = new GraphicsEngine::GraphicsEngine_DX11();

	return graphicsEngine;
}