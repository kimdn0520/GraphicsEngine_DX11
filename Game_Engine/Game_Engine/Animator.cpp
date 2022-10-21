#include "pch.h"
#include "Animator.h"
#include "GameObject.h"
#include "StructDefine.h"

Animator::Animator(shared_ptr<GameObject> gameObject)
	: Component(gameObject, COMPONENT_TYPE::ANIMATOR),
	transform(gameObject->GetTransform())
{}

Animator::~Animator()
{}

void Animator::SetAnimClip(std::string animName, shared_ptr<AnimationClip> animClip)
{
	
}

void Animator::Play(std::string animName, bool loop)
{

}

void Animator::Update()
{

}
