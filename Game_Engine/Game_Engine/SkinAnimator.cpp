#include "pch.h"
#include "SkinAnimator.h"
#include "GameObject.h"
#include "Transform.h"

SkinAnimator::SkinAnimator(shared_ptr<GameObject> gameObject)
	: Component(gameObject, COMPONENT_TYPE::SKINANIMATOR),
	_transform(gameObject->GetTransform())
{
	ZeroMemory(_finalBoneListMatrix, sizeof(XMMATRIX) * 96);
}

SkinAnimator::~SkinAnimator()
{
}


/// <summary>
/// 메시렌더러가 Render() 함수를 통해서 나중에 넘기기때문에
/// 메시렌더러 쪽으로 finalBoneListMatrix를 넘겨주면된다.
/// </summary>
void SkinAnimator::Update()
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
		shared_ptr<GameObject> tmpBone = boneObjList[i];

		boneWorldTM = tmpBone->GetTransform()->GetWorldMatrix();
		boneNodeTM = tmpBone->GetTransform()->GetNodeMatrix();

		Matrix skinWorldTM = _transform->GetWorldMatrix();
		Matrix skinNodeTM = _transform->GetNodeMatrix();
		skinWorldTM_Inverse = XMMatrixInverse(nullptr, skinWorldTM);

		boneNodeTM_Inverse = XMMatrixInverse(nullptr, boneNodeTM);

		// skinNode에 boneNode 역행렬을 곱하면 boneNode에서 skinNode를 가리키는 행렬이 나오고
		// boneWorld를 곱해서 world로 보내고 스키닝셰이더에서 자신의world를 두번곱해주고 있어가지구..
		// skinWorldTM_Inverse를 여기다가 곱해주거나 스키닝셰이더에서 값을 하나 뺀다.
		finalBoneTM = skinNodeTM * boneNodeTM_Inverse * boneWorldTM * skinWorldTM_Inverse;

		_finalBoneListMatrix[i] = finalBoneTM;
	}

	/*Matrix boneWorldTM;

	Matrix boneOffsetTM;

	Matrix boneNodeTM;

	Matrix finalBoneTM;

	for (int i = 0; i < boneObjList.size(); i++)
	{
		std::shared_ptr<Transform> boneTransform = boneObjList[i]->GetTransform();

		boneWorldTM = boneTransform->GetWorldMatrix();

		boneNodeTM = boneObjList[i]->GetTransform()->GetNodeMatrix();

		boneOffsetTM = boneNodeTM * _transform->GetNodeMatrix().Invert();

		_finalBoneListMatrix[i] = boneOffsetTM.Invert() * boneWorldTM * _transform->GetWorldMatrix().Invert();
	}*/
}
