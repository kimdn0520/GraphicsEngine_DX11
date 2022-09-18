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
	// ī�޶��� ���� look ���Ϳ��� ������ �̵�
	if (InputManager::Get()->GetButton(KEY_TYPE::W))
		_transform->MoveLook(_speed * TimeManager::Get()->GetDeltaTime());

	// ī�޶��� ���� look ���Ϳ��� �ڷ� �̵�
	if (InputManager::Get()->GetButton(KEY_TYPE::S))
		_transform->MoveLook(-_speed * TimeManager::Get()->GetDeltaTime());

	// ī�޶��� ���� right ���Ϳ��� �������� �̵�
	if (InputManager::Get()->GetButton(KEY_TYPE::A))
		_transform->MoveRight(-_speed * TimeManager::Get()->GetDeltaTime());

	// ī�޶��� ���� right ���Ϳ��� ���������� �̵�
	if (InputManager::Get()->GetButton(KEY_TYPE::D))
		_transform->MoveRight(_speed * TimeManager::Get()->GetDeltaTime());

	// ī�޶��� ���� up ���Ϳ��� ���� �̵�
	if (InputManager::Get()->GetButton(KEY_TYPE::E))
		_transform->MoveUp(_speed * TimeManager::Get()->GetDeltaTime());

	// ī�޶��� ���� up ���Ϳ��� �Ʒ��� �̵�
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
