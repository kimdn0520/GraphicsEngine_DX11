#include "pch.h"
#include "Graphics_Interface.h"
#include "GraphicsEngine_DX11.h"
#include "Device.h"


std::shared_ptr<Graphics_Interface> Graphics_Interface::graphicsEngine = nullptr;

// GraphicsEngineInterface를 상속받고있는 GraphicsEngine_DX11의 객체 생성
std::shared_ptr<Graphics_Interface> Graphics_Interface::Get()
{
	if (graphicsEngine == nullptr)
		graphicsEngine = std::make_shared<GraphicsEngine_DX11>();

	return graphicsEngine;
}