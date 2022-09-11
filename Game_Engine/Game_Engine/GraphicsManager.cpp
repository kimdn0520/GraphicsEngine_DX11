#include "pch.h"
#include "GraphicsManager.h"

GraphicsManager* GraphicsManager::graphicsManager = nullptr;

GraphicsManager* GraphicsManager::Get()
{
	if (graphicsManager == nullptr)
		graphicsManager = new GraphicsManager();

	return graphicsManager;
}

void GraphicsManager::Initialize(HWND hWnd, int screenWidth, int screenHeight)
{
	// �׷��Ƚ� ������ ���´�.
	graphicsEngine = Graphics_Interface::Get();

	// �׷��Ƚ� ������ Init ���ش�.
	graphicsEngine->Initialize(hWnd, screenWidth, screenHeight);
}

void GraphicsManager::Release()
{
	graphicsEngine->Release();
}

void GraphicsManager::OnResize(int width, int height)
{
	graphicsEngine->OnResize(width, height);
}

void GraphicsManager::MainBackBufferRender()
{
	graphicsEngine->MainBackBufferRender();
}

void GraphicsManager::RenderingDataRender()
{
	graphicsEngine->RenderingDataRender();
}

void GraphicsManager::Present()
{
	graphicsEngine->Present();
}

int GraphicsManager::GetScreenWidth()
{
	return graphicsEngine->GetScreenWidth();
}

int GraphicsManager::GetScreenHeight()
{
	return graphicsEngine->GetScreenHeight();
}
