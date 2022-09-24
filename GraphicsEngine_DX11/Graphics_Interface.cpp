#include "pch.h"
#include "Graphics_Interface.h"
#include "GraphicsEngine_DX11.h"
#include "Device.h"


std::shared_ptr<Graphics_Interface> Graphics_Interface::graphicsEngine = nullptr;

// GraphicsEngineInterface�� ��ӹް��ִ� GraphicsEngine_DX11�� ��ü ����
std::shared_ptr<Graphics_Interface> Graphics_Interface::Get()
{
	if (graphicsEngine == nullptr)
		graphicsEngine = std::make_shared<GraphicsEngine_DX11>();

	return graphicsEngine;
}