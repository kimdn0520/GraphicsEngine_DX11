#include "pch.h"
#include "GameEngine.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "Resources.h"

void GameEngine::Initialize(HWND hwnd, int width, int height)
{
	InputManager::Get()->Initialize(hwnd);
	TimeManager::Get()->Initialize();
	Resources::Get()->Initialize();
}

void GameEngine::Update()
{
	InputManager::Get()->Update();
	TimeManager::Get()->Update();

	// ����� �ε���� �ִٸ� �ҷ��´�.
	if (SceneManager::Get()->isReservedLoadScene)
	{
		SceneManager::Get()->ReservedSceneLoad();
	}
	else
	{
		SceneManager::Get()->Update();
		SceneManager::Get()->Render();
	}
}

void GameEngine::Release()
{
	SceneManager::Get()->Release();
}


