#pragma once
#include "GameEngineDLL.h"
#include "Component.h"
#include "Graphics_RenderingData.h"

class Transform;
class GameObject;

class MeshRenderer : public Component
{
public:
	GameEngine_DLL MeshRenderer(GameObject* gameObject);
	virtual ~MeshRenderer();

private:
	Transform* _transform;

	ObjectInfo* _objectInfo;

	bool _isBone = false;				// 본인지 아닌지(본은 안 그릴거라서)

public:
	std::vector<GameObject*> boneObjList;

	std::vector<std::string> boneNameList;

public:
	ObjectInfo* GetObjectInfo() { return _objectInfo; }

	GameEngine_DLL void SetMeshName(std::string meshName) { _objectInfo->name = meshName; }

	GameEngine_DLL void SetMaterial(Material* material);

	// 스킨메시인지
	void IsSkinnedMesh(bool value) { _objectInfo->isSkinned = value; }

	// 본인지 아닌지
	void SetIsBone(bool val) { _isBone = val; }

	void Update() override;

	void Render() override;
};

