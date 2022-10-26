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

	// ������ Rate
	float frameRate;

	// �ִϸ��̼� ù ��° ������
	int startKeyFrame;

	// �ִϸ��̼� �� ������ ��
	int totalKeyFrame;

	// �ִϸ��̼� ������ ������
	int endKeyFrame;

	// �����Ӵ� �� Tick�� ����������
	float tickPerFrame;
};