#include "pch.h"
#include "GraphicsManager.h"

GraphicsManager* GraphicsManager::graphicsManager = nullptr;

GraphicsManager* GraphicsManager::Get()
{
	if (graphicsManager == nullptr)
		graphicsManager = GraphicsManager::Get();

	return graphicsManager;
}

void GraphicsManager::Initialize(HWND hWnd, int screenWidth, int screenHeight)
{
	// 그래픽스 엔진을 얻어온다.
	graphicsEngine = Graphics_Interface::Get();

	// 그래픽스 엔진을 Init 해준다.
	graphicsEngine->Initialize(hWnd, screenWidth, screenHeight);
}
