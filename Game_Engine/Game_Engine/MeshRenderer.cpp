#include "pch.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "GameObject.h"
#include "GraphicsManager.h"

MeshRenderer::MeshRenderer(std::shared_ptr<GameObject> gameObject)
	: Component(gameObject, COMPONENT_TYPE::MESHRENDERER),
	_transform(gameObject->GetComponent<Transform>()),
	_objectInfo(new ObjectInfo())
{
	ZeroMemory(_objectInfo->finalBoneListMatrix, sizeof(XMMATRIX) * 96);

	_objectInfo->objectID = GetGameObject()->GetObjectID();
}

MeshRenderer::~MeshRenderer()
{}

void MeshRenderer::SetMaterial(std::shared_ptr<Material> material)
{
	_objectInfo->materials.push_back(material);
}

void MeshRenderer::Update()
{
	Matrix boneWorldTM;
	Matrix boneNodeTM;
	Matrix boneNodeTM_Inverse;
	Matrix boneOffsetTM;
	Matrix boneOffsetTM_Inverse;
	Matrix skinWorldTM;
	Matrix skinWorldTM_Inverse;
	Matrix finalBoneTM;

	for (int i = 0; i < boneObjList.size(); i++)
	{
		std::shared_ptr<GameObject> tmpBone = boneObjList[i];

		boneWorldTM = tmpBone->GetTransform()->GetWorldMatrix();
		boneNodeTM = tmpBone->GetTransform()->GetNodeMatrix();

		Matrix skinWorldTM = _transform->GetWorldMatrix();
		Matrix skinNodeTM = _transform->GetNodeMatrix();
		skinWorldTM_Inverse = XMMatrixInverse(nullptr, skinWorldTM);

		boneNodeTM_Inverse = XMMatrixInverse(nullptr, boneNodeTM);

		// skinNode�� boneNode ������� ���ϸ� boneNode���� skinNode�� ����Ű�� ����� ������
		// boneWorld�� ���ؼ� world�� ������ ��Ű�׼��̴����� �ڽ���world�� �ι������ְ� �־����..
		// skinWorldTM_Inverse�� ����ٰ� �����ְų� ��Ű�׼��̴����� ���� �ϳ� ����.
		finalBoneTM = skinNodeTM * boneNodeTM_Inverse * boneWorldTM * skinWorldTM_Inverse;

		_objectInfo->finalBoneListMatrix[i] = finalBoneTM;
	}
}

void MeshRenderer::Render()
{
	_objectInfo->worldTM = _transform->GetWorldMatrix();
	_objectInfo->worldPos = _transform->GetWorldPosition();

	if (!_isBone)
		GraphicsManager::Get()->SendObjectRenderingData(_objectInfo);
}
