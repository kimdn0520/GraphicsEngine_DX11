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
	// �׷��Ƚ� ������ ���´�.
	graphicsEngine = Graphics_Interface::Get();

	// �׷��Ƚ� ������ Init ���ش�.
	graphicsEngine->Initialize(hWnd, screenWidth, screenHeight);
}
