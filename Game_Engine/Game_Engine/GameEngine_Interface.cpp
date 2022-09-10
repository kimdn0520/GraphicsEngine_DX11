#include "pch.h"
#include "GameEngine_Interface.h"
#include "GameEngine.h"
#include "GraphicsManager.h"

GameEngine* GEngine;

void EngineInit(HWND hwnd, int width, int height)
{
	// ���� ���� ��ü ����
	GEngine = new GameEngine();

	// ���� �׷��Ƚ� �Ŵ��� init -> �ȿ��� �׷��Ƚ� init ���ش�.
	GraphicsManager::Get()->Initialize(hwnd, width, height);

	GEngine->Initialize(hwnd, width, height);
}

void EngineUpdate()
{
	GEngine->Update();
}

void EngineRender()
{
	GraphicsManager::Get()->MainBackBufferRender();

	GraphicsManager::Get()->RenderingDataRender();

	GraphicsManager::Get()->Present();
}

void EngineShutdown()
{
	GraphicsManager::Get()->Release();

	GEngine->Release();

	delete GEngine;
}

void ResizeWindow(int width, int height)
{
	if (GEngine != nullptr)
	{
		GraphicsManager::Get()->OnResize(width, height);
	}
}
