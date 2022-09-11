#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

SceneManager* SceneManager::sceneManager = nullptr;

SceneManager* SceneManager::Get()
{
	if (sceneManager == nullptr)
		sceneManager = new SceneManager();

	return sceneManager;
}

void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;
	
	_activeScene->FixedUpdate();
	_activeScene->Update();
	_activeScene->LateUpdate();
}

void SceneManager::Render()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Render();
}

void SceneManager::LoadScene(string sceneName)
{
	isReservedLoadScene = true;

	_nextScene = _sceneList[sceneName];
}

void SceneManager::ReservedSceneLoad()
{
	if (_activeScene != nullptr)
	{
		_activeScene->AllClearGameObject();
	}

	_activeScene = _nextScene;
	_activeScene->Initialize();

	_activeScene->Awake();
	_activeScene->Start();

	isReservedLoadScene = false;
}

void SceneManager::SetRemoveGameObject(GameObject* gameObj)
{
	_destroyObjList.push(gameObj);
}

void SceneManager::ReservedRemoveGameObject()
{
	if (_activeScene != nullptr)
	{
		while (!_destroyObjList.empty())
		{
			GameObject* destroyGameObj = _destroyObjList.front();

			// 현재 씬에 있는 해당 게임오브젝트를 삭제한다
			_activeScene->RemoveGameObject(destroyGameObj);

			_destroyObjList.pop();

			delete destroyGameObj;
		}
	}
}

void SceneManager::SetInstantiateGameObject(GameObject* gameObj)
{
	_instantiateGameObjectList.push(gameObj);
}

void SceneManager::ReservedInstantiateGameObject()
{
	if (_activeScene != nullptr)
	{
		if (!_instantiateGameObjectList.empty())
		{
			_activeScene->AddGameObject(_instantiateGameObjectList.front());

			_instantiateGameObjectList.front()->Awake();

			_instantiateGameObjectList.front()->Start();

			_instantiateGameObjectList.pop();
		}
	}
}

void SceneManager::Release()
{
	for (auto& scene : _sceneList)
	{
		scene.second->Release();
	}

	_sceneList.clear();
}
