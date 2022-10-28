#include "pch.h"
#include "Player.h"
#include "GameObject.h"
#include "Transform.h"
#include "Animator.h"

#include "InputManager.h"
#include "TimeManager.h"

Player::Player(std::shared_ptr<GameObject> gameObject)
	: MonoBehaviour(gameObject),
	transform(gameObject->GetTransform())
{}

Player::~Player()
{}

void Player::FixedUpdate()
{
	
}

void Player::Update()
{
	if (InputManager::Get()->GetButtonDown(KEY_TYPE::KEY_1))
	{
		GetGameObject()->PlayAnim(GetGameObject(), "Armature|Armature|mixamo.com|Layer0", true);
	}
	else if (InputManager::Get()->GetButton(KEY_TYPE::KEY_2))
	{
		GetGameObject()->PlayAnim(GetGameObject(), "Armature|Armature|mixamo.com|Layer0", true);
	}
	else if (InputManager::Get()->GetButton(KEY_TYPE::KEY_3))
	{
		GetGameObject()->PlayAnim(GetGameObject(), "Armature|Armature|mixamo.com|Layer0", true);
	}
	else if (InputManager::Get()->GetButton(KEY_TYPE::KEY_4))
	{
		GetGameObject()->PlayAnim(GetGameObject(), "Armature|Armature|mixamo.com|Layer0", true);
	}
}
