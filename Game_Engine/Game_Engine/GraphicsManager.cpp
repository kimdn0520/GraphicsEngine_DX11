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
	// 그래픽스 엔진을 얻어온다.
	graphicsEngine = Graphics_Interface::Get();

	// 그래픽스 엔진을 Init 해준다.
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
