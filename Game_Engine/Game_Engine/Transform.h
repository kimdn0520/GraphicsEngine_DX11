#pragma once
#include "GameEngineDLL.h"
#include "AlignedAllocationPolicy.h"
#include "Component.h"

class Transform : public Component, public AlignedAllocationPolicy<16>
{
public:
	GameEngine_DLL Transform(GameObject* gameObject);
	virtual ~Transform();

private:
	Vector3 _localPosition = { 0.f, 0.f, 0.f };
	Vector3 _localRotation = { 0.f, 0.f, 0.f };
	Vector3 _localScale = { 1.f, 1.f, 1.f };
	Vector3 _worldPosition = { 0.f, 0.f, 0.f };
	Vector3 _worldRotation = { 0.f, 0.f, 0.f };
	Vector3 _worldScale = { 1.f, 1.f, 1.f };

	Matrix _nodeMatrix;
	Matrix _localMatrix;
	Matrix _worldMatrix;

	Transform* _parent;

	Vector3 _targetPos;
	Vector3 _right = { 1.f, 0.f, 0.f };
	Vector3 _up = { 0.f, 1.f, 0.f };
	Vector3 _look = { 0.f, 0.f, 1.f };
	float _mousePosX;
	float _mousePosY;

	Vector3 _oriScale;
	Vector3 _oriRot;
	Vector3 _oriTrans;

	const float _rad2Deg = 360 / (3.141592f * 2);

public:
	GameEngine_DLL Vector3 GetRight() { return _right; }
	GameEngine_DLL Vector3 GetUp() { return _up; }
	GameEngine_DLL Vector3 GetLook() { return _look; }
	GameEngine_DLL void LookAt(Vector3 target);

	GameEngine_DLL Transform* GetParent() { return _parent; }
	GameEngine_DLL void SetParent(Transform* parent) { _parent = parent; }

	GameEngine_DLL const Matrix& GetWorldMatrix() { return _worldMatrix; }
	const Matrix& GetNodeMatrix() { return _nodeMatrix; }
	GameEngine_DLL const Vector3& GetLocalPosition() { return _localPosition; }
	GameEngine_DLL const Vector3& GetLocalRotation() { return _localRotation; }
	GameEngine_DLL const Vector3& GetLocalScale() { return _localScale; }
	GameEngine_DLL const Vector3& GetWorldPosition();
	GameEngine_DLL Vector3& GetWorldRotation() { return _worldRotation; }
	GameEngine_DLL const Vector3& GetWorldScale();

	GameEngine_DLL void SetLocalPosition(const Vector3& position) { _localPosition = position; }
	GameEngine_DLL void SetLocalRotation(const Vector3& rotation);
	GameEngine_DLL void SetLocalScale(const Vector3& scale) { _localScale = scale; }

	void SetNodeTM(Matrix nodeTM) { _nodeMatrix = nodeTM; }

	GameEngine_DLL Vector3 QuatToEuler(Vector4 quat);

	GameEngine_DLL Vector4 EulerToQuat(Vector3& euler);

	// Look 벡터 방향으로 움직인다.
	GameEngine_DLL void MoveLook(float speed);
	// Right 벡터 방향으로 움직인다.
	GameEngine_DLL void MoveRight(float speed);
	// Up 벡터 방향으로 움직인다.
	GameEngine_DLL void MoveUp(float speed);
	// 회전
	GameEngine_DLL void Rotate(Vector3 rotation);

	GameEngine_DLL void RotateXAxis(float angle);
	GameEngine_DLL void RotateYAxis(float angle);

	// 마우스 위치 저장
	GameEngine_DLL void OnMouseDown(int x, int y);
	// 마우스 움직임에따라 회전
	GameEngine_DLL void OnMouseMove(int x, int y, float speed);

	// ASE파서에서 Decompose해서 받았던 original scale,rot,trans
	const Vector3& GetOriScale() { return _oriScale; }
	const Vector3& GetOriRot() { return _oriRot; }
	const Vector3& GetOriTrans() { return _oriTrans; }
	void SetOriScale(Vector3 oriScale) { _oriScale = oriScale; }
	void SetOriRot(Vector3 oriRot) { _oriRot = oriRot; }
	void SetOriTrans(Vector3 oriTrans) { _oriTrans = oriTrans; }

	static void DecomposeMatrix(XMMATRIX worldTM, XMFLOAT3& scale, XMFLOAT3& rotation, XMFLOAT3& trans);

	void FixedUpdate() override;
};

