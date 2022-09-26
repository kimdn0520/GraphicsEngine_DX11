#pragma once
#include "GameEngineDLL.h"

/// <summary>
/// Update문을 1프레임마다 한번 씩 수행함.
/// Timer를 사용하여 이전 프레임에서 현재 프레임까지 경과된 시간(deltaTime)을
/// 곱해줌으로써 1초간 정해진 만큼의 움직임을 구현할 수 있다.
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
	float	_deltaTime = 0.f;				// 이전 프레임에서 현재 프레임까지 경과된 시간

private:
	unsigned __int32	_frameCount = 0;	// Update문에 몇번 실행되었는지 게산
	float	_frameTime = 0.f;				// 누적 시간
	unsigned __int32	_fps = 0;			// framePerSecond 초마다 평균적으로 몇프레임 실행됐는지 계산
};

