#pragma once
#include "GameEngineDLL.h"

/// <summary>
/// Update���� 1�����Ӹ��� �ѹ� �� ������.
/// Timer�� ����Ͽ� ���� �����ӿ��� ���� �����ӱ��� ����� �ð�(deltaTime)��
/// ���������ν� 1�ʰ� ������ ��ŭ�� �������� ������ �� �ִ�.
/// </summary>
class TimeManager
{
public:
	TimeManager() = default;
	~TimeManager() = default;

private:
	static std::shared_ptr<TimeManager> timeManager;

public:
	static GameEngine_DLL std::shared_ptr<TimeManager> Get();

public:
	void Initialize();
	void Update();

	unsigned __int32 GetFps() { return _fps; }
	GameEngine_DLL float GetDeltaTime() { return _deltaTime; }

private:
	unsigned __int64	_frequency = 0;
	unsigned __int64	_prevCount = 0;
	float	_deltaTime = 0.f;				// ���� �����ӿ��� ���� �����ӱ��� ����� �ð�

private:
	unsigned __int32	_frameCount = 0;	// Update���� ��� ����Ǿ����� �Ի�
	float	_frameTime = 0.f;				// ���� �ð�
	unsigned __int32	_fps = 0;			// framePerSecond �ʸ��� ��������� �������� ����ƴ��� ���
};

