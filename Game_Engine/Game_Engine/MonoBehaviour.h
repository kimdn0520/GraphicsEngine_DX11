#pragma once
#include "GameEngineDLL.h"
#include "Component.h"

class ColliderBase;
class GameObject;

class MonoBehaviour : public Component
{
public:
	GameEngine_DLL MonoBehaviour(GameObject* gameObject);
	GameEngine_DLL virtual ~MonoBehaviour();

public:
	// 오브젝트가 켜질때
	virtual void OnEnable() {};

	// 오브젝트가 꺼질때
	virtual void OnDisable() {};

	GameEngine_DLL void Instantiate(shared_ptr<GameObject> prefab, Vector3 pos);

	// 프로그램 실행중에 게임오브젝트를 삭제할 수 있도록 해준다.
	GameEngine_DLL void Destroy(shared_ptr<GameObject> gameObject);

	virtual void OnTriggerEnter(ColliderBase* other) {};
	virtual void OnTriggerStay(ColliderBase* other) {};
	virtual void OnTriggerExit(ColliderBase* other) {};
};

