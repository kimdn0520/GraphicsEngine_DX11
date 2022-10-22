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

		//for (int key = 0; key < currentClip->keyFrame.size(); key++)
		//{
		//	// ���� ƽ�� �� ó�� �ִϸ��̼Ǻ��� �۰ų� ���� ���
		//	// ��ó�� �ִϸ��̼��� ����Ѵ�.
		//	if (currentPosTick <= _tickNumberPosList[_clipIndex][0])
		//	{
		//		preIndex = 0;
		//		postIndex = 1;

		//		// tickPos �� �Ѱ��ϰ��..?!
		//		if (_tickNumberPosList[_clipIndex].size() == 1)
		//			postIndex = 0;

		//		t = 0.f;

		//		break;
		//	}

		//	// ���� ƽ�� ��� posTick ���� ū ���
		//	// ������ �ִϸ��̼��� ����Ѵ�.
		//	if (currentPosTick >= _tickNumberPosList[_clipIndex][_tickNumberPosList[_clipIndex].size() - 1])
		//	{
		//		// tickPos �� �Ѱ��� ���?
		//		if (_tickNumberPosList[_clipIndex].size() == 1)
		//		{
		//			preIndex = 0;
		//			postIndex = 0;
		//		}
		//		else
		//		{
		//			preIndex = _tickNumberPosList[_clipIndex].size() - 2;
		//			postIndex = _tickNumberPosList[_clipIndex].size() - 1;
		//		}

		//		t = 1.0f;

		//		break;
		//	}

		//	if (currentPosTick > _tickNumberPosList[_clipIndex][i])
		//		continue;

		//	// ���� ƽ�� ���� ���
		//	// �� ���� ���� ����� ƽ���� �Ѵ�.
		//	preIndex = i - 1;
		//	postIndex = i;

		//	t = static_cast<float>(currentPosTick - _tickNumberPosList[_clipIndex][preIndex]) / (_tickNumberPosList[_clipIndex][postIndex] - _tickNumberPosList[_clipIndex][preIndex]);
		//	break;
		//}

		//Vector3 pos = XMVectorLerp(_currentClip->position[preIndex]->pos, _currentClip->position[postIndex]->pos, t);

		//_transform->SetLocalPosition(pos);
	}

	updateTime = 0.f;
}
