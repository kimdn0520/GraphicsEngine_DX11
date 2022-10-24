#pragma once
#include "MonoBehaviour.h"

class Transform;
class Animator;

class Player : public MonoBehaviour
{
public:
	Player(std::shared_ptr<GameObject> gameObject);
	virtual ~Player();

private:

public:
	std::shared_ptr<Transform> transform;

	std::shared_ptr<Animator> animator;

	virtual void FixedUpdate() override;

	virtual void Update() override;
};

