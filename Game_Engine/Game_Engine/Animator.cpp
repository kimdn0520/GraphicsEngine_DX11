#include "pch.h"
#include "Animator.h"
#include "GameObject.h"
#include "StructDefine.h"
#include "Transform.h"

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

	currentFrame = currentClip->startKeyFrame;
}

void Animator::Update()
{
	if (!isPlay || currentClip == nullptr)
		return;

	updateTime += TimeManager::Get()->GetDeltaTime();

	if (updateTime < 1.0f / currentClip->frameRate)
		return;

	updateTime = 0.f;

	// 현재 틱을 구한당.
	// 증가하는 frame 에다가 ticksperFrame(프레임 마다의 틱) 을 곱해준다.
	float currentTick = currentFrame * currentClip->tickPerFrame;		
	int preIndex = 0;
	int postIndex = 0;
	float t = 0.f;				// 보간 계수

	for (int key = 0; key < currentClip->keyFrame.size(); key++)
	{
		// 현재 틱이 맨 처음 애니메이션보다 작거나 같은 경우
		// 맨처음 애니메이션을 재생한다.
		if (currentTick <= currentClip->keyFrame[0]->time)
		{
			preIndex = 0;

			postIndex = 1;

			// tick이 한개일경우..
			if (currentClip->keyFrame.size() == 1)
				postIndex = 0;

			t = 0.f;

			break;
		}

		// 현재 틱이 모든 tick 보다 큰 경우
		// 마지막 애니메이션을 재생한다.
		if (currentTick >= currentClip->keyFrame[currentClip->keyFrame.size() - 1]->time)
		{
			// tick이 한개일 경우..
			if (currentClip->keyFrame.size() == 1)
			{
				preIndex = 0;
				postIndex = 0;
			}
			else
			{
				preIndex = currentClip->keyFrame.size() - 2;
				postIndex = currentClip->keyFrame.size() - 1;
			}

			t = 1.0f;

			break;
		}

		if (currentTick > currentClip->keyFrame[key]->time)
			continue;

		// 현재 틱이 작은 경우
		// 그 전의 가장 가까운 틱으로 한다.
		preIndex = key - 1;
		postIndex = key;

		t = static_cast<float>(currentTick - currentClip->keyFrame[preIndex]->time) / (currentClip->keyFrame[postIndex]->time - currentClip->keyFrame[preIndex]->time);
		
		break;
	}

	Vector3 pos = XMVectorLerp(currentClip->keyFrame[preIndex]->worldTransform, currentClip->keyFrame[postIndex]->worldTransform, t);

	Vector4 rotQT = XMQuaternionSlerp(currentClip->keyFrame[preIndex]->worldRotation, currentClip->keyFrame[postIndex]->worldRotation, t);
	
	Vector3 rotation = transform->QuatToEuler(rotQT);

	//Vector3 scale = XMVectorLerp(currentClip->keyFrame[preIndex]->localScale, currentClip->keyFrame[postIndex]->localScale, t);

	transform->SetLocalPosition(pos);

	transform->SetLocalRotation(Vector3(rotation.x, rotation.y, rotation.z));

	//transform->SetLocalScale(Vector3(scale.x, scale.y, scale.z));

	currentFrame++;

	// 루프면 다시 처음으로
	if (isLoop)
	{
		if (currentFrame > currentClip->totalKeyFrame)
			currentFrame = currentClip->startKeyFrame;
	}
	// 루프 아니면 멈춤
	else
	{
		if (currentFrame > currentClip->totalKeyFrame)
			isPlay = false;
	}
}
