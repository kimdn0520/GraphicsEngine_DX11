#include "pch.h"
#include "GameEngine_Interface.h"
#include "GameEngine.h"
#include "GraphicsManager.h"

unique_ptr<GameEngine> GEngine;

void EngineInit(HWND hwnd, int width, int height)
{
	// ���� ���� ��ü ����
	GEngine = std::make_unique<GameEngine>();

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
	GraphicsManager::Get()->RenderingDataRender();

	GraphicsManager::Get()->Present();
}

void EngineShutdown()
{
	GraphicsManager::Get()->Release();

	GEngine->Release();

	GEngine.reset();
}

void ResizeWindow(int width, int height)
{
	if (GEngine != nullptr)
	{
		GraphicsManager::Get()->OnResize(width, height);
	}
}
