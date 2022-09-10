#include "pch.h"
#include "GameEngine_Interface.h"
#include "GameEngine.h"
#include "GraphicsManager.h"

GameEngine* GEngine;

void EngineInit(HWND hwnd, int width, int height)
{
	// 게임 엔진 객체 생성
	GEngine = new GameEngine();

	// 게임 그래픽스 매니저 init -> 안에서 그래픽스 init 해준다.
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
