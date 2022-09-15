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

	bool _isBone = false;				// ������ �ƴ���(���� �� �׸��Ŷ�)

public:
	std::vector<GameObject*> boneObjList;

	std::vector<std::string> boneNameList;

public:
	ObjectInfo* GetObjectInfo() { return _objectInfo; }

	GameEngine_DLL void SetMeshID(std::size_t meshID) { _objectInfo->meshID = meshID; }

	GameEngine_DLL void SetMaterial(Material* material);

	// ��Ų�޽�����
	void IsSkinnedMesh(bool value) { _objectInfo->isSkinned = value; }

	// ������ �ƴ���
	void SetIsBone(bool val) { _isBone = val; }

	void Update() override;

	void Render() override;
};

