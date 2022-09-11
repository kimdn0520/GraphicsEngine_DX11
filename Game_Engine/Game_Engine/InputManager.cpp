#include "pch.h"
#include "InputManager.h"

InputManager* InputManager::inputManager = nullptr;

InputManager* InputManager::Get()
{
	if (inputManager == nullptr)
		inputManager = new InputManager();

	return inputManager;
}

void InputManager::Initialize(HWND hwnd)
{
	_hwnd = hwnd;

	_states.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
}

void InputManager::Update()
{
	HWND hwnd = ::GetActiveWindow();
	if (_hwnd != hwnd)
	{
		for (unsigned __int64 key = 0; key < KEY_TYPE_COUNT; key++)
			_states[key] = KEY_STATE::NONE;

		return;
	}

	BYTE asciiKeys[KEY_TYPE_COUNT] = {};
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	for (unsigned __int64 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		if (key == 25)
			continue;

		// 키가 눌려 있으면 true
		if (asciiKeys[key] & 0x80)
		{
			KEY_STATE& state = _states[key];

			// 이전 프레임에 키를 누른 상태라면 PRESS
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::PRESS;
			else
			{
				_isAnykey = true;
				state = KEY_STATE::DOWN;
			}
		}
		else
		{
			KEY_STATE& state = _states[key];

			// 이전 프레임에 키를 누른 상태라면 UP
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
			{
				_isAnykey = false;
				state = KEY_STATE::UP;
			}
			else
				state = KEY_STATE::NONE;
		}
	}

	::GetCursorPos(&_mousePos);
	::ScreenToClient(_hwnd, &_mousePos);
}

float InputManager::GetAxisRawHorizontal()
{
	if (GetButton(KEY_TYPE::LEFT))
		return -1.f;
	else if (GetButton(KEY_TYPE::RIGHT))
		return 1.f;

	if (GetButtonUp(KEY_TYPE::LEFT))
		return 0.f;
	else if (GetButtonUp(KEY_TYPE::RIGHT))
		return 0.f;

	return 0.f;
}

float InputManager::GetAxisRawVertical()
{
	if (GetButton(KEY_TYPE::UP))
		return 1.f;
	else if (GetButton(KEY_TYPE::DOWN))
		return -1.f;

	if (GetButtonUp(KEY_TYPE::UP))
		return 0.f;
	else if (GetButtonUp(KEY_TYPE::DOWN))
		return 0.f;

	return 0.f;
}