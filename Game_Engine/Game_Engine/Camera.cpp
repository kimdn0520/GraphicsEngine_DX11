#include "pch.h"
#include "GraphicsManager.h"
#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"

Camera::Camera(GameObject* gameObject)
	: Component(gameObject, COMPONENT_TYPE::CAMERA),
	_cameraTransform(gameObject->GetComponent<Transform>()),
	_nearZ(1.0f), _farZ(500.0f), _fov(XM_PI / 4.f),
	_width(static_cast<float>(GraphicsManager::Get()->GetScreenWidth())),
	_height(static_cast<float>(GraphicsManager::Get()->GetScreenHeight()))
{}

Camera::~Camera()
{}

// ������ǥ��� ǥ���� �������� ī�޶� ��ǥ��� ��ȯ�� �ؾ���.
// ���� ��ǥ ��ȯ�� ���������� ������ ��ġ�� �Ǵµ�
// ī�޶� ���� ���忡 �����ϱ� ���÷� ���� ����� ���ϸ�� �׷��� �׳� ī�޶��������� ������� ���ϸ�ȴ�.
// ���� [ī�޶��� ���� ��ǥ ��ȯ ����� �����]�� ī�޶� ��ȯ ���(�� ���) �̴�
// �̰� ������Ʈ�� ������Ŀ� �����ָ� �ȴ�.
void Camera::UpdateViewMatrix()
{
	Vector3 axisX = _cameraTransform->GetRight();			// ī�޶� x��
	Vector3 axisY = _cameraTransform->GetUp();				// ī�޶� y��
	Vector3 axisZ = _cameraTransform->GetLook();			// ī�޶� z�� (look���͸� z�� ��´�.) - ī�޶� �ٶ󺸴� ����
	Vector3 camPos = _cameraTransform->GetLocalPosition();

	axisZ.Normalize();										// z normalize
	axisY = axisZ.Cross(axisX);								// x���̶� �����ؼ� y ���ϰ�
	axisY.Normalize();
	axisX = axisY.Cross(axisZ);								// y���ѰŸ� z�� �����ؼ� x���ϰ�

	float x = -camPos.Dot(axisX);
	float y = -camPos.Dot(axisY);
	float z = -camPos.Dot(axisZ);

	Matrix V
	{
		axisX.x, axisY.x, axisZ.x, 0.f,
		axisX.y, axisY.y, axisZ.y, 0.f,
		axisX.z, axisY.z, axisZ.z, 0.f,
		x, y, z, 1.0f
	};

	_viewTM = V;
}

void Camera::UpdatePerspectiveMatrix()
{
	_width = (static_cast<float>(GraphicsManager::Get()->GetScreenWidth()));
	_height = (static_cast<float>(GraphicsManager::Get()->GetScreenHeight()));

	_projTM = XMMatrixPerspectiveFovLH(_fov, _width / _height, _nearZ, _farZ);

	_orthoProjTM = XMMatrixOrthographicLH(_width, _height, 0.f, 1.0f);
}

void Camera::Update()
{
	// ���, projection �̶� ��� ������Ʈ���ش�.
	UpdateViewMatrix();
	UpdatePerspectiveMatrix();

	_cameraInfo.worldPos = _cameraTransform->GetWorldPosition();
	_cameraInfo.viewTM = _viewTM;
	_cameraInfo.projTM = _projTM;
	_cameraInfo.orthoProjTM = _orthoProjTM;

	// GraphicsEngineManager::GetInstance()->SetCameraInfo(_cameraInfo);
}