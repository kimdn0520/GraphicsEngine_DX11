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
	// 현재 씬을 업데이트 하쟈~
	void Update();

	// 현재 씬을 렌더 하쟈~
	void Render();

	// 현재 씬을 겟또다제~
	GameEngine_DLL const std::shared_ptr<Scene> GetActiveScene() { return _activeScene; }

	// 예약된 씬이 있는지 없는지~
	//const bool& IsReservedLoadScene() { return _isReservedLoadScene; }

	// 저장되어있는 씬리스트들 겟또다제~
	const map<std::string, std::shared_ptr<Scene>> GetSceneList() { return _sceneList; }

	// 씬을 넣어주는 함수~
	template <typename TScene>
	void InputScene(std::string sceneName);

	// 씬을 불러옵니다~
	GameEngine_DLL void LoadScene(std::string sceneName);

	// 예약된 씬을 불러올거야~
	void ReservedSceneLoad();

	// 제거할 게임오브젝트를 destroyObjList에 넣어준다.
	GameEngine_DLL void SetRemoveGameObject(std::shared_ptr<GameObject> gameObj);

	// 제거 예약된 오브젝트를 제거합니다.
	void ReservedRemoveGameObject();

	// 생성할 게임오브젝트를 instantiateGameObjectList에 넣어준다.
	GameEngine_DLL void SetInstantiateGameObject(std::shared_ptr<GameObject> gameObj);

	// 생성 예약된 오브젝트를 생성합니다.
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
