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
}

void EngineUpdate()
{

}

void EngineRender()
{

}

void EngineShutdown()
{

}

void ResizeWindow(int width, int height)
{

}
