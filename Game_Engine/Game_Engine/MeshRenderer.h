#pragma once
#include "GameEngineDLL.h"
#include "Component.h"
#include "Graphics_RenderingData.h"

class Transform;
class GameObject;

class MeshRenderer : public Component
{
public:
	GameEngine_DLL MeshRenderer(std::shared_ptr<GameObject> gameObject);
	virtual ~MeshRenderer();

private:
	std::shared_ptr<Transform> _transform;

	shared_ptr<ObjectInfo> _objectInfo;

	bool _isBone = false;				// ������ �ƴ���(���� �� �׸��Ŷ�)

public:
	std::vector<std::shared_ptr<GameObject>> boneObjList;

	std::vector<std::string> boneNameList;

public:
	shared_ptr<ObjectInfo> GetObjectInfo() { return _objectInfo; }

	GameEngine_DLL void SetMeshID(std::size_t meshID) { _objectInfo->meshID = meshID; }

	GameEngine_DLL void SetMaterial(std::shared_ptr<Material> material);

	// ��Ų�޽�����
	void IsSkinnedMesh(bool value) { _objectInfo->isSkinned = value; }

	void IsShadow(bool value) { _objectInfo->isShadow = value; }

	// ������ �ƴ���
	void SetIsBone(bool val) { _isBone = val; }

	void Update() override;

	void Render() override;
};

