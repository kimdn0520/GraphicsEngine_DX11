#pragma once
#include "GameEngineDLL.h"

class GameObject;
class Camera;
class UI;
class Text;

class Scene
{
private:
	std::vector<GameObject*> _gameObjects;

	// ���ӿ�����Ʈ�� Camera�� ��Ƴ��� vector
	std::vector<Camera*> _cameras;

	// ���ӿ�����Ʈ�� UI�� ��Ƴ��� vector
	std::vector<UI*> _ui;

	// ���ӿ�����Ʈ�� Text�� ��Ƴ��� vector
	std::vector<Text*> _text;

public:
	static bool isMainCamera;

public:
	friend class SceneManager;

public:
	GameEngine_DLL Camera* GetMainCamera();

	const vector<GameObject*>& GetGameObjects() { return _gameObjects; }

	const vector<UI*>& GetUI() { return _ui; }

	const vector<Text*>& GetText() { return _text; }

	// ���� ���� ���ӿ�����Ʈ�� �����ž�
	GameEngine_DLL void AddGameObject(GameObject* gameObject);

	// ���ӿ�����Ʈ�� ����ž�
	GameEngine_DLL void RemoveGameObject(GameObject* gameObject);

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

