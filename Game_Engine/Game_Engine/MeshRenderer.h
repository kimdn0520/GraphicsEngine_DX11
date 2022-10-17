#pragma once
#include "GameEngineDLL.h"
#include "Component.h"
#include "Graphics_RenderingData.h"

class Transform;
class GameObject;
class SkinAnimator;

struct FBXBoneInfo;

class MeshRenderer : public Component
{
public:
	GameEngine_DLL MeshRenderer(std::shared_ptr<GameObject> gameObject);
	virtual ~MeshRenderer();

private:
	std::shared_ptr<Transform> _transform;

	shared_ptr<ObjectInfo> _objectInfo;

public:
	vector<shared_ptr<GameObject>> boneObjList;

	bool isBone = false;

public:
	void SetBoneObject(shared_ptr<GameObject> boneObj) { boneObjList.push_back(boneObj); }
	
	shared_ptr<ObjectInfo> GetObjectInfo() { return _objectInfo; }

	GameEngine_DLL void SetMeshID(std::size_t meshID) { _objectInfo->meshID = meshID; }

	GameEngine_DLL void SetMaterial(std::string name);

	// 스킨메시인지
	void IsSkinnedMesh(bool value) { _objectInfo->isSkinned = value; }

	void IsShadow(bool value) { _objectInfo->isShadow = value; }

	void Update() override;

	void Render() override;
};

