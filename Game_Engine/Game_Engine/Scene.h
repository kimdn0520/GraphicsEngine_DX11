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

	// ���ӿ�����Ʈ�� Camera�� ��Ƴ��� vector
	std::vector<std::shared_ptr<Camera>> _cameras;

	// ���ӿ�����Ʈ�� UI�� ��Ƴ��� vector
	std::vector<std::shared_ptr<UI>> _ui;

	// ���ӿ�����Ʈ�� Text�� ��Ƴ��� vector
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

	// ���� ���� ���ӿ�����Ʈ�� �����ž�
	GameEngine_DLL void AddGameObject(std::shared_ptr<GameObject> gameObject);

	// ���ӿ�����Ʈ�� ����ž�
	GameEngine_DLL void RemoveGameObject(std::shared_ptr<GameObject> gameObject);

	// ���� ���� ��� ���ӿ�����Ʈ�� ����ž�
	GameEngine_DLL void AllClearGameObject();

	// �����
	void Release();

	// ������ ������Ʈ���� ����ų� �Ҷ�
	virtual void Initialize() {};

	void Awake();				// ���� ���� �ʱ�ȭ �ϴ� �Լ�
	void Start();				// ���� �Լ�
	void FixedUpdate();			// ���� ������Ʈ(�̵� -> �浹 -> ����?)
	void Update();				// ������Ʈ
	void LateUpdate();			// ī�޶� ������ ������Ʈ

	void Render();				// ������ �׷��Ƚ��� �Ѱ�����
};

