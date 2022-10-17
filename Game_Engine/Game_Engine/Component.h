#pragma once

class GameObject;

enum class COMPONENT_TYPE
{
	// 이동 행렬 -> 물리(움직이고 충돌을 했으면 물리작용?) 
	TRANSFORM,					// FIXED UPDATE 
	COLLIDER,					// FIXED UPDATE

	ANIMATOR,					// UPDATE
	CAMERA,						// UPDATE, script 에서는 LateUpdate
	LIGHT,						// UPDATE

	UI,							// UPDATE, LATEUPDATE, RENDER

	TEXT,						// Render
	MESHRENDERER,				// Render

	MONO_BEHAVIOUR,				// script
	END,
};

enum
{
	// 모노비헤비이어는 포함 안한다.
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
	// 어떤 타입인지 알아오깅.. 
	COMPONENT_TYPE GetType() { return _type; }

	// 이 컴포넌트가 달린 게임오브젝트를 얻어올수 있다.
	std::shared_ptr<GameObject> GetGameObject() { return _gameObject; }

	virtual void Awake() { }
	virtual void Start() { }
	virtual void FixedUpdate() { }
	virtual void Update() { }
	virtual void LateUpdate() { }

	virtual void Render() { }
};

