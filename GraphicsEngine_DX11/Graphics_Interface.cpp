#include "pch.h"
#include "Graphics_Interface.h"
#include "GraphicsEngine_DX11.h"

Graphics_Interface* Graphics_Interface::graphicsEngine = nullptr;

// GraphicsEngineInterface�� ��ӹް��ִ� GraphicsEngine_DX11�� ��ü ����
Graphics_Interface* Graphics_Interface::Get()
{
	if (graphicsEngine == nullptr)
		graphicsEngine = new GraphicsEngine::GraphicsEngine_DX11();

	return graphicsEngine;
}