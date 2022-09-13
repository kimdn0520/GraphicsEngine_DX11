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
	// ������Ʈ�� ������
	virtual void OnEnable() {};

	// ������Ʈ�� ������
	virtual void OnDisable() {};

	GameEngine_DLL void Instantiate(GameObject* prefab, DirectX::SimpleMath::Vector3 pos);

	// ���α׷� �����߿� ���ӿ�����Ʈ�� ������ �� �ֵ��� ���ش�.
	GameEngine_DLL void Destroy(GameObject* gameObject);

	virtual void OnTriggerEnter(ColliderBase* other) {};
	virtual void OnTriggerStay(ColliderBase* other) {};
	virtual void OnTriggerExit(ColliderBase* other) {};
};

