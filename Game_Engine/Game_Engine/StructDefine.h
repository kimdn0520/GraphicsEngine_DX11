#pragma once
#include <string>
#include <vector>
#include <memory>

#include "SimpleMath.h"

struct AnimKeyFrame
{
	float time;

	DirectX::SimpleMath::Vector3 localTransform;
	DirectX::SimpleMath::Quaternion localRotation;
	DirectX::SimpleMath::Vector3 localScale;
};

struct AnimationClip
{
	std::string		animationName;

	std::vector<std::shared_ptr<AnimKeyFrame>>	keyFrame;

	// 프레임 Rate
	float frameRate;

	// 애니메이션 첫 번째 프레임
	int startKeyFrame;

	// 애니메이션 총 프레임 수
	int totalKeyFrame;

	// 애니메이션 마지막 프레임
	int endKeyFrame;

	// 프레임당 몇 Tick이 지나가는지
	float tickPerFrame;
};