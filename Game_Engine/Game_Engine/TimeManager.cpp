#include "pch.h"
#include "TimeManager.h"

std::shared_ptr<TimeManager> TimeManager::timeManager = nullptr;

std::shared_ptr<TimeManager> TimeManager::Get()
{
	if (timeManager == nullptr)
		timeManager = std::make_shared<TimeManager>();

	return timeManager;
}

void TimeManager::Initialize()
{
	// 성능 타이머의 주파수(초당 개수)를 얻을 때
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));

	// 성능 타이머로부터 개수 단위의 현재 시간을 얻을 때
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount)); // CPU 클럭
}

/// <summary>
/// 시작점과 끝지점에서 QueryPerformanceCounter 함수를 호출하면 
/// 수행시간동안 발생한 진동수를 얻을 수 있음
/// 그러나 이 값은 시간 단위가 아니기 때문에
/// QueryPerformanceFrequency함수로 얻는 주기로 나누어 시간 단위로 변환해야 함.
/// </summary>
void TimeManager::Update()
{
	unsigned __int64 currentCount;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	// 현재 Count(currentCount)에서 전 Count(preCount)를 뺀 값을
	// frequency로 나누어주면 경과된 시간(초)이 나온다.
	_deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);
	_prevCount = currentCount;

	_frameCount++;
	_frameTime += _deltaTime;

	if (_frameTime > 1.f)	// 1초를 넘어가는 순간
	{
		_fps = static_cast<unsigned __int32>(_frameCount / _frameTime);

		_frameTime = 0.f;
		_frameCount = 0;
	}
}

