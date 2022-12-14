#pragma once
#include "GameEngineDLL.h"

class GameObject;
class Camera;
class UI;
class Text;

class Scene
{
private:
	std::vector<std::shared_ptr<GameObject>> _gameObjects;

	// 게임오브젝트중 Camera만 모아놓은 vector
	std::vector<std::shared_ptr<Camera>> _cameras;

	// 게임오브젝트중 UI만 모아놓은 vector
	std::vector<std::shared_ptr<UI>> _ui;

	// 게임오브젝트중 Text만 모아놓은 vector
	std::vector<std::shared_ptr<Text>> _text;

public:
	static bool isMainCamera;

public:
	friend class SceneManager;

public:
	GameEngine_DLL std::shared_ptr<Camera> GetMainCamera();

	const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() { return _gameObjects; }

	const std::vector<std::shared_ptr<UI>>& GetUI() { return _ui; }

	const std::vector<std::shared_ptr<Text>>& GetText() { return _text; }

	// 현재 씬에 게임오브젝트를 넣을거얌
	GameEngine_DLL void AddGameObject(std::shared_ptr<GameObject> gameObject);

	// 게임오브젝트를 지울거야
	GameEngine_DLL void RemoveGameObject(std::shared_ptr<GameObject> gameObject);

	// 현재 씬에 모든 게임오브젝트를 지울거야
	GameEngine_DLL void AllClearGameObject();

	// 종료시
	void Release();

	// 씬에서 오브젝트들을 만들거나 할때
	virtual void Initialize() {};

	void Awake();				// 가장 먼저 초기화 하는 함수
	void Start();				// 시작 함수
	void FixedUpdate();			// 물리 업데이트(이동 -> 충돌 -> 물리?)
	void Update();				// 업데이트
	void LateUpdate();			// 카메라 같은거 업데이트

	void Render();				// 렌더는 그래픽스로 넘겨주쟈
};

