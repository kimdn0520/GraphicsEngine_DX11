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

// 월드좌표계로 표현된 정점들을 카메라 좌표계로 변환을 해야함.
// 월드 좌표 변환의 역방향으로 과정을 거치면 되는데
// 카메라가 지금 월드에 있으니까 로컬로 가는 행렬을 구하면댐 그래서 그냥 카메라월드행렬의 역행렬을 구하면된다.
// 따라서 [카메라의 월드 좌표 변환 행렬의 역행렬]이 카메라 변환 행렬(뷰 행렬) 이다
// 이걸 오브젝트의 월드행렬에 곱해주면 된다.
void Camera::UpdateViewMatrix()
{
	Vector3 axisX = _cameraTransform->GetRight();			// 카메라 x축
	Vector3 axisY = _cameraTransform->GetUp();				// 카메라 y축
	Vector3 axisZ = _cameraTransform->GetLook();			// 카메라 z축 (look벡터를 z축 삼는다.) - 카메라가 바라보는 지점
	Vector3 camPos = _cameraTransform->GetLocalPosition();

	axisZ.Normalize();										// z normalize
	axisY = axisZ.Cross(axisX);								// x축이랑 외적해서 y 구하고
	axisY.Normalize();
	axisX = axisY.Cross(axisZ);								// y구한거를 z랑 외적해서 x구하고

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
	// 뷰랑, projection 이랑 계속 업데이트해준다.
	UpdateViewMatrix();
	UpdatePerspectiveMatrix();

	_cameraInfo.worldPos = _cameraTransform->GetWorldPosition();
	_cameraInfo.viewTM = _viewTM;
	_cameraInfo.projTM = _projTM;
	_cameraInfo.orthoProjTM = _orthoProjTM;

	// GraphicsEngineManager::GetInstance()->SetCameraInfo(_cameraInfo);
}