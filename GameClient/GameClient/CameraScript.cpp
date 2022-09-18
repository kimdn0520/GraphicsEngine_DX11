#include "pch.h"
#include "CameraScript.h"
#include "GameObject.h"
#include "Transform.h"
#include "InputManager.h"
#include "TimeManager.h"

CameraScript::CameraScript(GameObject* gameObject)
	: MonoBehaviour(gameObject),
	_transform(gameObject->GetTransform()),
	_speed(20.f)
{}

CameraScript::~CameraScript()
{}

void CameraScript::LateUpdate()
{
	// 카메라의 현재 look 벡터에서 앞으로 이동
	if (InputManager::Get()->GetButton(KEY_TYPE::W))
		_transform->MoveLook(_speed * TimeManager::Get()->GetDeltaTime());

	// 카메라의 현재 look 벡터에서 뒤로 이동
	if (InputManager::Get()->GetButton(KEY_TYPE::S))
		_transform->MoveLook(-_speed * TimeManager::Get()->GetDeltaTime());

	// 카메라의 현재 right 벡터에서 왼쪽으로 이동
	if (InputManager::Get()->GetButton(KEY_TYPE::A))
		_transform->MoveRight(-_speed * TimeManager::Get()->GetDeltaTime());

	// 카메라의 현재 right 벡터에서 오른쪽으로 이동
	if (InputManager::Get()->GetButton(KEY_TYPE::D))
		_transform->MoveRight(_speed * TimeManager::Get()->GetDeltaTime());

	// 카메라의 현재 up 벡터에서 위로 이동
	if (InputManager::Get()->GetButton(KEY_TYPE::E))
		_transform->MoveUp(_speed * TimeManager::Get()->GetDeltaTime());

	// 카메라의 현재 up 벡터에서 아래로 이동
	if (InputManager::Get()->GetButton(KEY_TYPE::Q))
		_transform->MoveUp(-_speed * TimeManager::Get()->GetDeltaTime());

	if (InputManager::Get()->GetButtonDown(KEY_TYPE::RBUTTON))
	{
		_transform->OnMouseDown(InputManager::Get()->GetMousePos().x,
								InputManager::Get()->GetMousePos().y);
	}
	else if (InputManager::Get()->GetButton(KEY_TYPE::RBUTTON))
	{
		_transform->OnMouseMove(InputManager::Get()->GetMousePos().x,
								InputManager::Get()->GetMousePos().y, 0.25f);
	}
}
