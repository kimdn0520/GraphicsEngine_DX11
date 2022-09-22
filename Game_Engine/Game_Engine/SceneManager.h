#pragma once
#include "GameEngineDLL.h"

class Scene;
class GameObject;

class SceneManager
{
public:
	SceneManager() {};
	~SceneManager() {};

private:
	static SceneManager* sceneManager;

public:
	static GameEngine_DLL SceneManager* Get();

private:
	Scene* _activeScene;
	Scene* _nextScene;
	map<std::string, Scene*> _sceneList;

	std::string _nextSceneName;

	std::queue<GameObject*> _destroyObjList;

	queue<GameObject*> _instantiateGameObjectList;

public:
	bool isReservedLoadScene = false;

	bool isPause = false;
	//
public:
	// ���� ���� ������Ʈ ����~
	void Update();

	// ���� ���� ���� ����~
	void Render();

	// ���� ���� �ٶǴ���~
	GameEngine_DLL const Scene* GetActiveScene() { return _activeScene; }

	// ����� ���� �ִ��� ������~
	//const bool& IsReservedLoadScene() { return _isReservedLoadScene; }

	// ����Ǿ��ִ� ������Ʈ�� �ٶǴ���~
	const map<std::string, Scene*> GetSceneList() { return _sceneList; }

	// ���� �־��ִ� �Լ�~
	template <typename TScene>
	void InputScene(std::string sceneName);

	// ���� �ҷ��ɴϴ�~
	GameEngine_DLL void LoadScene(std::string sceneName);

	// ����� ���� �ҷ��ðž�~
	void ReservedSceneLoad();

	// ������ ���ӿ�����Ʈ�� destroyObjList�� �־��ش�.
	GameEngine_DLL void SetRemoveGameObject(GameObject* gameObj);

	// ���� ����� ������Ʈ�� �����մϴ�.
	void ReservedRemoveGameObject();

	// ������ ���ӿ�����Ʈ�� instantiateGameObjectList�� �־��ش�.
	GameEngine_DLL void SetInstantiateGameObject(GameObject* gameObj);

	// ���� ����� ������Ʈ�� �����մϴ�.
	void ReservedInstantiateGameObject();

	GameEngine_DLL std::string& GetLoadSceneName() { return _nextSceneName; }

	GameEngine_DLL void SetLoadSceneName(std::string name) { _nextSceneName = name; }

	void Release();
};

template<typename TScene>
inline void SceneManager::InputScene(std::string sceneName)
{
	_sceneList[sceneName] = new TScene();
}
