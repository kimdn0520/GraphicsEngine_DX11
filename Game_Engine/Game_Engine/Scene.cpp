#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"

std::shared_ptr<Camera> Scene::GetMainCamera()
{
	if (_cameras.empty())
		return nullptr;

	return _cameras[0];
}

void Scene::AddGameObject(std::shared_ptr<GameObject> gameObject)
{
	// Camera ������Ʈ�� ���� _cameras ���� ����ش�.(Camera ���·� �־��ش�.)
	if (gameObject->GetCamera() != nullptr)
	{
		_cameras.push_back(gameObject->GetCamera());
	}

	// UI�� �������ִ� ���ӿ�����Ʈ��� UI�� ���� ����ش�.
	if (gameObject->GetUI() != nullptr)
	{
		_ui.push_back(gameObject->GetUI());
	}

	// Text�� �������ִ� ���ӿ�����Ʈ��� UI�� ���� ����ش�.
	if (gameObject->GetText() != nullptr)
	{
		_text.push_back(gameObject->GetText());
	}

	_gameObjects.push_back(gameObject);

	// �־��� ���ӿ�����Ʈ�� �ڽĿ�����Ʈ�� ������ �ִٸ� ã�ư��� ���� �־��ش�.
	if (gameObject->GetChilds().size() != 0)
	{
		for (int i = 0; i < gameObject->GetChilds().size(); i++)
		{
			AddGameObject(gameObject->GetChilds()[i]);
		}
	}
}

void Scene::RemoveGameObject(std::shared_ptr<GameObject> gameObject)
{
	if (gameObject->GetCamera())
	{
		auto findIt = std::find(_cameras.begin(), _cameras.end(), gameObject->GetCamera());
		if (findIt != _cameras.end())
		{
			_cameras.erase(findIt);
			findIt->reset();
		}
	}

	if (gameObject->GetUI())
	{
		auto findIt = std::find(_ui.begin(), _ui.end(), gameObject->GetUI());
		if (findIt != _ui.end())
		{
			_ui.erase(findIt);
			findIt->reset();
		}
	}

	auto findIt = std::find(_gameObjects.begin(), _gameObjects.end(), gameObject);
	if (findIt != _gameObjects.end())
	{
		_gameObjects.erase(findIt);
		findIt->reset();
	}

	if (gameObject->GetChilds().size() != 0)
	{
		for (int i = 0; i < gameObject->GetChilds().size(); i++)
		{
			RemoveGameObject(gameObject->GetChilds()[i]);
		}
	}
}

void Scene::AllClearGameObject()
{
	for (auto& gameObject : _gameObjects)
		gameObject.reset();

	_gameObjects.clear();
	_cameras.clear();
	_ui.clear();
	_text.clear();
}

void Scene::Release()
{
	for (auto& gameObject : _gameObjects)
		gameObject.reset();

	_gameObjects.clear();
	_cameras.clear();
	_ui.clear();
	_text.clear();
}

void Scene::Awake()
{
	for (auto& gameObject : _gameObjects)
	{
		gameObject->Awake();
	}
}

void Scene::Start()
{
	for (auto& gameObject : _gameObjects)
	{
		gameObject->Start();
	}
}

void Scene::FixedUpdate()
{
	for (auto& gameObject : _gameObjects)
	{
		gameObject->FixedUpdate();
	}
}

void Scene::Update()
{
	for (auto& gameObject : _gameObjects)
	{
		gameObject->Update();
	}
}

void Scene::LateUpdate()
{
	for (auto& gameObject : _gameObjects)
	{
		gameObject->LateUpdate();
	}
}

void Scene::Render()
{
	for (auto& gameObject : _gameObjects)
	{
		// ���ӿ�����Ʈ activeslef true �ΰ͸� �����ʿ� �Ѱ��ٰž�
		if (gameObject->activeSelf)
			gameObject->Render();
	}
}
