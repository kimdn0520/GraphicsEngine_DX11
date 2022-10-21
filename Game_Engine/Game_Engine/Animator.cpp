#include "pch.h"
#include "Animator.h"
#include "GameObject.h"
#include "StructDefine.h"

#include "TimeManager.h"

Animator::Animator(shared_ptr<GameObject> gameObject)
	: Component(gameObject, COMPONENT_TYPE::ANIMATOR),
	transform(gameObject->GetTransform()),
	currentFrame(0),
	preFrame(0),
	updateTime(0.f),
	currentClip(nullptr),
	isPlay(true),
	isLoop(true)
{}

Animator::~Animator()
{}

void Animator::SetAnimClip(std::string animName, shared_ptr<AnimationClip> animClip)
{
	animClips.insert(make_pair(animName, animClip));
}

void Animator::Play(std::string animName, bool loop)
{
	currentClip = animClips[animName];

	isLoop = loop;
}

void Animator::Update()
{
	if (!isPlay || currentClip == nullptr)
		return;

	updateTime += TimeManager::Get()->GetDeltaTime();

	currentFrame = (int)(updateTime / currentClip->tickPerFrame);

	if (preFrame != currentFrame)
	{
		preFrame = currentFrame;


	}

	updateTime = 0.f;
}
