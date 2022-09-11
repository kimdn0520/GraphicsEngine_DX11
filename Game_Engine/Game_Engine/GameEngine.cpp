#include "pch.h"
#include "GameEngine.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "SceneManager.h"

void GameEngine::Initialize(HWND hwnd, int width, int height)
{
	InputManager::Get()->Initialize(hwnd);
	TimeManager::Get()->Initialize();
}

void GameEngine::Update()
{
	InputManager::Get()->Update();
	TimeManager::Get()->Update();

	SceneManager::Get()->Update();
	SceneManager::Get()->Render();
}

void GameEngine::Release()
{
	SceneManager::Get()->Release();
}


