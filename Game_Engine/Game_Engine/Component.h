#pragma once

class GameObject;

enum class COMPONENT_TYPE
{
	// �̵� ��� -> ����(�����̰� �浹�� ������ �����ۿ�?) 
	TRANSFORM,					// FIXED UPDATE 
	COLLIDER,					// FIXED UPDATE

	ANIMATOR,					// UPDATE
	CAMERA,						// UPDATE, script ������ LateUpdate
	LIGHT,						// UPDATE

	UI,							// UPDATE, LATEUPDATE, RENDER

	TEXT,						// Render
	MESHRENDERER,				// Render

	MONO_BEHAVIOUR,				// script
	END,
};

enum
{
	// ��������̾�� ���� ���Ѵ�.
	COMPONENT_COUNT = (int)(COMPONENT_TYPE::END)-1
};

class Component
{
public:
	Component(std::shared_ptr<GameObject> gameObject, COMPONENT_TYPE type);
	virtual ~Component();

private:
	friend class GameObject;

	void SetGameObject(std::shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

protected:
	COMPONENT_TYPE _type;
	
	std::shared_ptr<GameObject> _gameObject;

public:
	// � Ÿ������ �˾ƿ���.. 
	COMPONENT_TYPE GetType() { return _type; }

	// �� ������Ʈ�� �޸� ���ӿ�����Ʈ�� ���ü� �ִ�.
	std::shared_ptr<GameObject> GetGameObject() { return _gameObject; }

	virtual void Awake() { }
	virtual void Start() { }
	virtual void FixedUpdate() { }
	virtual void Update() { }
	virtual void LateUpdate() { }

	virtual void Render() { }
};

