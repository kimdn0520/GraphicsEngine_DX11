#pragma once
#include "GameEngineDLL.h"
#include "Component.h"

struct AnimationClip;

class Transform;

class Animator : public Component
{
private:
	shared_ptr<Transform> transform;

	unordered_map<std::string, shared_ptr<AnimationClip>> animClips;

public:
	GameEngine_DLL Animator(shared_ptr<GameObject> gameObject);
	GameEngine_DLL virtual ~Animator();

public:
	GameEngine_DLL void SetAnimClip(std::string animName, shared_ptr<AnimationClip> animClip);

	GameEngine_DLL void Play(std::string animName, bool loop);

public:
	virtual void Update() override;
};

