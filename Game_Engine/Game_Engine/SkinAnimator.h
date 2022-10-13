#pragma once
#include "Component.h"

class Transform;

class SkinAnimator : public Component
{
public:
	SkinAnimator(shared_ptr<GameObject> gameObject);
	virtual ~SkinAnimator();

private:
	shared_ptr<Transform> _transform;

public:
	Matrix _finalBoneListMatrix[96];

	vector<shared_ptr<GameObject>> boneObjList;

	vector<size_t> boneNameList;

public:
	void SetBoneObject(shared_ptr<GameObject>& boneObj) { boneObjList.push_back(boneObj); }

	void Update() override;
};

