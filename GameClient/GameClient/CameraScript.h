#pragma once
#include "MonoBehaviour.h"

class Camera;
class Transform;

class CameraScript : public MonoBehaviour
{
public:
	CameraScript(GameObject* gameObject);
	virtual ~CameraScript();

private:
	float _speed;

public:
	Camera* _camera;
	Transform* _transform;

	Transform* _targetTransfom;

	int _mousePosX = 0;
	int _mousePosY = 0;

	virtual void LateUpdate() override;
};

