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

	// ���� ƽ�� ���Ѵ�.
	// �����ϴ� frame ���ٰ� ticksperFrame(������ ������ ƽ) �� �����ش�.
	float currentTick = currentFrame * currentClip->tickPerFrame;		
	int preIndex = 0;
	int postIndex = 0;
	float t = 0.f;				// ���� ���

	for (int key = 0; key < currentClip->keyFrame.size(); key++)
	{
		// ���� ƽ�� �� ó�� �ִϸ��̼Ǻ��� �۰ų� ���� ���
		// ��ó�� �ִϸ��̼��� ����Ѵ�.
		if (currentTick <= currentClip->keyFrame[0]->time)
		{
			preIndex = 0;

			postIndex = 1;

			// tick�� �Ѱ��ϰ��..
			if (currentClip->keyFrame.size() == 1)
				postIndex = 0;

			t = 0.f;

			break;
		}

		// ���� ƽ�� ��� tick ���� ū ���
		// ������ �ִϸ��̼��� ����Ѵ�.
		if (currentTick >= currentClip->keyFrame[currentClip->keyFrame.size() - 1]->time)
		{
			// tick�� �Ѱ��� ���..
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

		// ���� ƽ�� ���� ���
		// �� ���� ���� ����� ƽ���� �Ѵ�.
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

	// ������ �ٽ� ó������
	if (isLoop)
	{
		if (currentFrame > currentClip->totalKeyFrame)
			currentFrame = currentClip->startKeyFrame;
	}
	// ���� �ƴϸ� ����
	else
	{
		if (currentFrame > currentClip->totalKeyFrame)
			isPlay = false;
	}
}
