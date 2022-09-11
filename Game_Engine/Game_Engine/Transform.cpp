#include "pch.h"
#include "Transform.h"

Transform::Transform(GameObject* gameObject)
	: Component(gameObject, COMPONENT_TYPE::TRANSFORM),
	_mousePosX(0.f), _mousePosY(0.f), _up(Vector3(0.f, 1.f, 0.f))
{}

Transform::~Transform()
{}

void Transform::LookAt(Vector3 target)
{
	_targetPos = target;

	_look = target - _localPosition;
	_look.Normalize();

	_up = Vector3(0.f, 1.f, 0.f);
	_right = _up.Cross(_look);
	_right.Normalize();

	if (_right.Length() <= 0.009f)
	{
		_right = Vector3(0.f, 1.f, 2.f).Cross(_look);
	}

	_up = _look.Cross(_right);

	/*Matrix rotMat;
	rotMat._11 = _right.x; rotMat._12 = _right.y; rotMat._13 = _right.z;
	rotMat._21 = _up.x;     rotMat._22 = _up.y,     rotMat._23 = _up.z;
	rotMat._31 = _look.x,  rotMat._32 = _look.y;  rotMat._33 = _look.z;

	float pitch = (float)asin(-rotMat._32);
	float yaw = (float)atan2(rotMat._31, rotMat._33);
	float roll = (float)atan2(rotMat._12, rotMat._22);*/

	float pitch = atan2f(_up.Dot(Vector3(0, 0, 1)), _up.Dot(Vector3(0, 1, 0))) * _rad2Deg;
	float yaw = atan2f(_look.Dot(Vector3(1, 0, 0)), _look.Dot(Vector3(0, 0, 1))) * _rad2Deg;
	// 앞에거 말구 뒤에 _right.Dot(Vector3(0, 0, 0)) 이 원래 1, 0, 0 이였는데 0, 0, 0 으로 하니까 잘되네..? 
	float roll = atan2f(_right.Dot(Vector3(0, 1, 0)), _right.Dot(Vector3(0, 0, 0))) * _rad2Deg;

	SetLocalRotation({ pitch, yaw, roll });
}

const Vector3& Transform::GetWorldPosition()
{
	FixedUpdate();

	_worldPosition = Vector3(_worldMatrix._41, _worldMatrix._42, _worldMatrix._43);

	return _worldPosition;
}

const Vector3& Transform::GetWorldScale()
{
	FixedUpdate();

	_worldScale.x = sqrtf(powf(_worldMatrix._11, 2.f) + powf(_worldMatrix._12, 2.f) + powf(_worldMatrix._13, 2.f));
	_worldScale.y = sqrtf(powf(_worldMatrix._21, 2.f) + powf(_worldMatrix._22, 2.f) + powf(_worldMatrix._23, 2.f));
	_worldScale.y = sqrtf(powf(_worldMatrix._31, 2.f) + powf(_worldMatrix._32, 2.f) + powf(_worldMatrix._33, 2.f));

	return _worldScale;
}

void Transform::SetLocalRotation(const Vector3& rotation)
{
	_localRotation = rotation;

	if (_parent != nullptr)
	{
		_worldRotation = _localRotation - _parent->_worldRotation;
	}
	else
	{
		_worldRotation = _localRotation;
	}
}

/// <summary>
/// quaternion -> euler -> degree
/// </summary>
Vector3 Transform::QuatToEuler(Vector4 quat)
{
	Vector3 rotation;

	double x = quat.x;
	double y = quat.y;
	double z = quat.z;
	double w = quat.w;
	double sqx = x * x;
	double sqy = y * y;
	double sqz = z * z;
	double sqw = w * w;
	float exceptCheck = 2.0f * (w * x - y * z);
	float eulerX = 0.f;

	if (abs(exceptCheck) >= 1.f)
		eulerX = copysign(3.141592f / 2, exceptCheck);
	else
		eulerX = asin(2.0f * (w * x - y * z));

	float eulerY = atan2(2.0f * (x * z + w * y), (-sqx - sqy + sqz + sqw));
	float eulerZ = atan2(2.0f * (x * y + z * w), (-sqx + sqy - sqz + sqw));
	rotation = { eulerX, eulerY, eulerZ };

	rotation = { XMConvertToDegrees(rotation.x),
						XMConvertToDegrees(rotation.y),
						XMConvertToDegrees(rotation.z) };

	return rotation;
}

/// <summary>
/// Euler -> Quat
/// </summary>
Vector4 Transform::EulerToQuat(Vector3& euler)
{
	Vector4 result;

	float x = XMConvertToRadians(euler.x) * 0.5f;
	float y = XMConvertToRadians(euler.y) * 0.5f;
	float z = XMConvertToRadians(euler.z) * 0.5f;

	float sinX = sinf(x);
	float cosX = cosf(x);
	float sinY = sinf(y);
	float cosY = cosf(y);
	float sinZ = sinf(z);
	float cosZ = cosf(z);

	result.x = (cosY * sinX * cosZ) + (sinY * cosX * sinZ);
	result.y = (sinY * cosX * cosZ) - (cosY * sinX * sinZ);
	result.z = (cosY * cosX * sinZ) - (sinY * sinX * cosZ);
	result.w = (cosY * cosX * cosZ) + (sinY * sinX * sinZ);

	return result;
}

void Transform::MoveLook(float speed)
{
	SetLocalPosition(_localPosition + _look * speed);
}

void Transform::MoveRight(float speed)
{
	SetLocalPosition(_localPosition + _right * speed);
}

void Transform::MoveUp(float speed)
{
	SetLocalPosition(_localPosition + _up * speed);
}

void Transform::Rotate(Vector3 rotation)
{
	SetLocalRotation(_localRotation + rotation);
}

/// <summary>
/// X축(트랜스폼 right벡터) 기준으로 회전 (일단 카메라용..)
/// </summary>
void Transform::RotateXAxis(float angle)
{
	XMFLOAT3 tmpRight = XMFLOAT3(_right.x, _right.y, _right.z);
	XMFLOAT3 tmpUp = XMFLOAT3(_up.x, _up.y, _up.z);
	XMFLOAT3 tmpLook = XMFLOAT3(_look.x, _look.y, _look.z);

	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&tmpRight), angle);

	XMStoreFloat3(&tmpUp, XMVector3TransformNormal(XMLoadFloat3(&tmpUp), R));
	XMStoreFloat3(&tmpLook, XMVector3TransformNormal(XMLoadFloat3(&tmpLook), R));

	_up = { tmpUp.x, tmpUp.y, tmpUp.z };
	_look = { tmpLook.x, tmpLook.y, tmpLook.z };

	XMMATRIX rotMat;

	rotMat.r[2] = { _look.x, _look.y, _look.z };
	rotMat.r[1] = { _up.x, _up.y, _up.z };
	rotMat.r[0] = { _right.x, _right.y, _right.z };

	float pitch = (float)asin(-rotMat.r[2].m128_f32[1]);
	float yaw = (float)atan2(rotMat.r[2].m128_f32[0], rotMat.r[2].m128_f32[2]);
	float roll = (float)atan2(rotMat.r[0].m128_f32[1], rotMat.r[1].m128_f32[1]);

	SetLocalRotation({ pitch, yaw, roll });
}

/// <summary>
///	카메라를 Y축(트랜스폼 up벡터) 기준으로 회전
/// </summary>
void Transform::RotateYAxis(float angle)
{
	XMFLOAT3 tmpRight = XMFLOAT3(_right.x, _right.y, _right.z);
	XMFLOAT3 tmpUp = XMFLOAT3(_up.x, _up.y, _up.z);
	XMFLOAT3 tmpLook = XMFLOAT3(_look.x, _look.y, _look.z);

	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&tmpRight, XMVector3TransformNormal(XMLoadFloat3(&tmpRight), R));
	XMStoreFloat3(&tmpUp, XMVector3TransformNormal(XMLoadFloat3(&tmpUp), R));
	XMStoreFloat3(&tmpLook, XMVector3TransformNormal(XMLoadFloat3(&tmpLook), R));

	_right = { tmpRight.x, tmpRight.y, tmpRight.z };
	_up = { tmpUp.x, tmpUp.y, tmpUp.z };
	_look = { tmpLook.x, tmpLook.y, tmpLook.z };

	XMMATRIX rotMat;

	rotMat.r[2] = { _look.x, _look.y, _look.z };
	rotMat.r[1] = { _up.x, _up.y, _up.z };
	rotMat.r[0] = { _right.x, _right.y, _right.z };

	float pitch = (float)asin(-rotMat.r[2].m128_f32[1]);
	float yaw = (float)atan2(rotMat.r[2].m128_f32[0], rotMat.r[2].m128_f32[2]);
	float roll = (float)atan2(rotMat.r[0].m128_f32[1], rotMat.r[1].m128_f32[1]);

	SetLocalRotation({ pitch, yaw, roll });
}

void Transform::OnMouseDown(int x, int y)
{
	_mousePosX = x;
	_mousePosY = y;
}

void Transform::OnMouseMove(int x, int y, float speed)
{
	// XMConvertToRadians 안해줘서 큐브맵이 안나오는 각도가 있었다
	float dx = XMConvertToRadians(speed * static_cast<float>(x - _mousePosX));
	float dy = XMConvertToRadians(speed * static_cast<float>(y - _mousePosY));

	RotateXAxis(dy);
	RotateYAxis(dx);

	_mousePosX = x;
	_mousePosY = y;
}

void Transform::DecomposeMatrix(XMMATRIX worldTM, XMFLOAT3& scale, XMFLOAT3& rotation, XMFLOAT3& trans)
{
	XMVECTOR localScaleVec;
	XMVECTOR localRotQuatVec;
	XMVECTOR localTransVec;
	XMVECTOR localRotationVec;
	XMMatrixDecompose(&localScaleVec, &localRotQuatVec, &localTransVec, worldTM);

	// 쿼터니언을 오일러각으로 바꾸는 과정..
	double x = localRotQuatVec.m128_f32[0];
	double y = localRotQuatVec.m128_f32[1];
	double z = localRotQuatVec.m128_f32[2];
	double w = localRotQuatVec.m128_f32[3];
	double sqx = x * x;
	double sqy = y * y;
	double sqz = z * z;
	double sqw = w * w;
	float eulerX = asin(2.0f * (w * x - y * z));
	float eulerY = atan2(2.0f * (x * z + w * y), (-sqx - sqy + sqz + sqw));
	float eulerZ = atan2(2.0f * (x * y + z * w), (-sqx + sqy - sqz + sqw));
	rotation = { eulerX, eulerY, eulerZ };

	XMStoreFloat3(&scale, localScaleVec);
	XMStoreFloat3(&trans, localTransVec);
	rotation = { XMConvertToDegrees(rotation.x),
						XMConvertToDegrees(rotation.y),
						XMConvertToDegrees(rotation.z) };
}



void Transform::FixedUpdate()
{
	Matrix scaleMatrix = XMMatrixScaling(_localScale.x, _localScale.y, _localScale.z);

	// 오일러각 사용
	/*XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(_localRotation.x),
		XMConvertToRadians(_localRotation.y),
		XMConvertToRadians(_localRotation.z));*/

		// 쿼터니언을 로테이션으로 빌드 
	Matrix rotationMatrix = XMMatrixRotationQuaternion(EulerToQuat(_localRotation));

	Matrix positionMatrix = XMMatrixTranslation(_localPosition.x, _localPosition.y, _localPosition.z);

	_localMatrix = scaleMatrix * rotationMatrix * positionMatrix;
	_worldMatrix = _localMatrix;

	if (_parent != nullptr)
	{
		_worldMatrix *= _parent->GetWorldMatrix();
	}
}
