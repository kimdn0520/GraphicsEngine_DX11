#pragma once
#include "Component.h"

class Transform;

struct FBXBoneInfo;

class SkinAnimator : public Component
{
public:
	SkinAnimator(shared_ptr<GameObject> gameObject);
	virtual ~SkinAnimator();

private:
	shared_ptr<Transform> _transform;

public:
	Matrix _finalBoneListMatrix[96];

	vector<shared_ptr<FBXBoneInfo>> boneObjList;

public:
	//void SetBoneObject(shared_ptr<GameObject>& boneObj) { boneObjList.push_back(boneObj); }

	void Update() override;
};

