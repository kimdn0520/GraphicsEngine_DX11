#pragma once
#include "MonoBehaviour.h"

class Camera;
class Transform;

class CameraScript : public MonoBehaviour
{
public:
	CameraScript(std::shared_ptr<GameObject> gameObject);
	virtual ~CameraScript();

private:
	float _speed;

public:
	std::shared_ptr<Camera> _camera;
	std::shared_ptr<Transform> _transform;

	std::shared_ptr<Transform> _targetTransfom;

	int _mousePosX = 0;
	int _mousePosY = 0;

	virtual void LateUpdate() override;
};

