#pragma once
#include "GameEngineDLL.h"

class Scene;
class GameObject;

class SceneManager
{
public:
	SceneManager() = default;
	~SceneManager() = default;

private:
	static std::shared_ptr<SceneManager> sceneManager;

public:
	static GameEngine_DLL std::shared_ptr<SceneManager> Get();

private:
	std::shared_ptr<Scene> _activeScene;
	std::shared_ptr<Scene> _nextScene;
	map<std::string, std::shared_ptr<Scene>> _sceneList;

	std::string _nextSceneName;

	std::queue<std::shared_ptr<GameObject>> _destroyObjList;

	queue<std::shared_ptr<GameObject>> _instantiateGameObjectList;

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
	GameEngine_DLL const std::shared_ptr<Scene> GetActiveScene() { return _activeScene; }

	// ����� ���� �ִ��� ������~
	//const bool& IsReservedLoadScene() { return _isReservedLoadScene; }

	// ����Ǿ��ִ� ������Ʈ�� �ٶǴ���~
	const map<std::string, std::shared_ptr<Scene>> GetSceneList() { return _sceneList; }

	// ���� �־��ִ� �Լ�~
	template <typename TScene>
	void InputScene(std::string sceneName);

	// ���� �ҷ��ɴϴ�~
	GameEngine_DLL void LoadScene(std::string sceneName);

	// ����� ���� �ҷ��ðž�~
	void ReservedSceneLoad();

	// ������ ���ӿ�����Ʈ�� destroyObjList�� �־��ش�.
	GameEngine_DLL void SetRemoveGameObject(std::shared_ptr<GameObject> gameObj);

	// ���� ����� ������Ʈ�� �����մϴ�.
	void ReservedRemoveGameObject();

	// ������ ���ӿ�����Ʈ�� instantiateGameObjectList�� �־��ش�.
	GameEngine_DLL void SetInstantiateGameObject(std::shared_ptr<GameObject> gameObj);

	// ���� ����� ������Ʈ�� �����մϴ�.
	void ReservedInstantiateGameObject();

	GameEngine_DLL std::string& GetLoadSceneName() { return _nextSceneName; }

	GameEngine_DLL void SetLoadSceneName(std::string name) { _nextSceneName = name; }

	void Release();
};

template<typename TScene>
inline void SceneManager::InputScene(std::string sceneName)
{
	_sceneList[sceneName] = std::make_shared<TScene>();
}
