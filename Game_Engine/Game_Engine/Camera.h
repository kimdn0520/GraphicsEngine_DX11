#pragma once
#include "GameEngineDLL.h"
#include "AlignedAllocationPolicy.h"
#include "Component.h"
#include "Graphics_PerFrameData.h"

class Transform;

enum class PROJECTION_TYPE
{
	PERSPECTIVE,			 // ���� ����
	ORTHOGRAPHIC,			 // ���� ����
};

class Camera : public Component, public AlignedAllocationPolicy<16>
{
public:
	GameEngine_DLL Camera(std::shared_ptr<GameObject> gameObject);
	GameEngine_DLL virtual ~Camera();

private:
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	Vector3 _targetPos;

	std::shared_ptr<Transform> _cameraTransform;

	// z�� �þ߷κ����� �Ÿ��� ��� ������ x, y��ǥ�� �Ÿ��� ����Ͽ� �����Ѵ�.
	// near�� far �� 1 ~ 1000 ���� ������ ���ָ� ���������� ������ ��ġ�鼭 0 ~ 1 �� ��ȯ�̵ȴ�.
	float _nearZ;
	float _farZ;
	float _fov;
	float _width;
	float _height;

	Matrix _viewTM = {};
	Matrix _projTM = {};
	Matrix _orthoProjTM = {};
	Matrix _reflectionTM = {};

	shared_ptr<CameraInfo> _cameraInfo;

public:
	Matrix GetViewTM() { return _viewTM; }
	Matrix GetProjTM() { return _projTM; }
	Matrix GetReflectionViewMatrix() { return _reflectionTM; }

	void SetNear(float value) { _nearZ = value; }
	void SetFar(float value) { _farZ = value; }
	void SetFOV(float value) { _fov = value; }

	void UpdateViewMatrix();

	void UpdatePerspectiveMatrix();

	virtual void Update() override;
	
	// ���콺�� ���� ��ġ�� ��ǥ���� WorldSpace�� ��ȯ���ؼ�
	// Ray�� ���������� ������ �����Ѵ�.
	// GameEngine_DLL NoobRay ScreenPointToRay(POINT mousePos);
};