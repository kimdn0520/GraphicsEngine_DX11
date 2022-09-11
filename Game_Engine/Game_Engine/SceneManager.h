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
	map<string, Scene*> _sceneList;

	string _nextSceneName;

	std::queue<GameObject*> _destroyObjList;

	queue<GameObject*> _instantiateGameObjectList;

public:
	bool isReservedLoadScene = false;

	bool isPause = false;

public:
	// 현재 씬을 업데이트 하쟈~
	void Update();

	// 현재 씬을 렌더 하쟈~
	void Render();

	// 현재 씬을 겟또다제~
	GameEngine_DLL const Scene* GetActiveScene() { return _activeScene; }

	// 예약된 씬이 있는지 없는지~
	//const bool& IsReservedLoadScene() { return _isReservedLoadScene; }

	// 저장되어있는 씬리스트들 겟또다제~
	const map<string, Scene*> GetSceneList() { return _sceneList; }

	// 씬을 넣어주는 함수~
	template <typename TScene>
	void InputScene(string sceneName);

	// 씬을 불러옵니다~
	GameEngine_DLL void LoadScene(string sceneName);

	// 예약된 씬을 불러올거야~
	void ReservedSceneLoad();

	// 제거할 게임오브젝트를 destroyObjList에 넣어준다.
	GameEngine_DLL void SetRemoveGameObject(GameObject* gameObj);

	// 제거 예약된 오브젝트를 제거합니다.
	void ReservedRemoveGameObject();

	// 생성할 게임오브젝트를 instantiateGameObjectList에 넣어준다.
	GameEngine_DLL void SetInstantiateGameObject(GameObject* gameObj);

	// 생성 예약된 오브젝트를 생성합니다.
	void ReservedInstantiateGameObject();

	GameEngine_DLL string& GetLoadSceneName() { return _nextSceneName; }

	GameEngine_DLL void SetLoadSceneName(string name) { _nextSceneName = name; }

	void Release();
};

template<typename TScene>
inline void SceneManager::InputScene(string sceneName)
{
	_sceneList[sceneName] = new TScene();
}
