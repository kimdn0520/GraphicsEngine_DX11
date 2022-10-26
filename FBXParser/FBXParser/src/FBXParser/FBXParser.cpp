#include "pch.h"
#include "FBXParser/FBXParser.h"
#include "ParserData/ParserData.h"

FBXParser::FBXParser()
{
}

FBXParser::~FBXParser()
{
	if (scene)
		scene->Destroy();

	if (manager)
		manager->Destroy();
}

std::shared_ptr<FBXModel> FBXParser::LoadFbx(const std::string& path)
{
	// FBXModel ����
	fbxModel = std::make_shared<FBXModel>();

	// ���� ������ �ε�
	Import(path);

	LoadAnimation();

	ProcessBones(scene->GetRootNode(), 0, -1);

	ProcessMesh(scene->GetRootNode(), FbxNodeAttribute::eMesh);

	return fbxModel;
}

/// <summary>
/// Mesh���� �ε��ؼ� FBXModel�� fbxMeshInfoList�� �����Ѵ�.
/// ���ؽ��� ������ ��� ��� bone�� ������ �޴� mesh�� �ִٸ� bone ����ġ�� �־��ְ�
/// bone�� offsetMatrix ������ �߰����ش�
/// <summary>
void FBXParser::ProcessMesh(fbxsdk::FbxNode* node, FbxNodeAttribute::EType attribute)
{
	fbxsdk::FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();

	if (nodeAttribute && nodeAttribute->GetAttributeType() == attribute)
	{
		LoadMesh(node);
	}

	// Material �ε�
	const int materialCount = node->GetMaterialCount();

	for (int i = 0; i < materialCount; ++i)
	{
		fbxsdk::FbxSurfaceMaterial* surfaceMaterial = node->GetMaterial(i);

		LoadMaterial(surfaceMaterial);
	}

	// Tree ���� ��� ȣ��
	const int childCount = node->GetChildCount();

	for (int i = 0; i < childCount; ++i)
		ProcessMesh(node->GetChild(i), FbxNodeAttribute::eMesh);
}

void FBXParser::Import(const std::string& path)
{
	// FBX SDK ������ ��ü ����
	manager = fbxsdk::FbxManager::Create();

	// IOSettings ��ü ���� �� ����
	fbxsdk::FbxIOSettings* settings = fbxsdk::FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(settings);

	// �� ��ü ����
	scene = fbxsdk::FbxScene::Create(manager, "");

	// FbxImporter ��ü ����
	importer = fbxsdk::FbxImporter::Create(manager, "");

	// importer �ʱ�ȭ
	importer->Initialize(path.c_str(), -1, manager->GetIOSettings());

	// fbx ���� ������ scene ���� �����´�.
	importer->Import(scene);

	// Axis�� DirectX�� �°� ��ȯ

	// ��ǥ���� �����´�.
	FbxAxisSystem sceneAxisSystem = scene->GetGlobalSettings().GetAxisSystem();

	// �� ���� ��ǥ���� �ٲ۴�.
	scene->GetGlobalSettings().SetAxisSystem(fbxsdk::FbxAxisSystem::DirectX);
	//FbxAxisSystem::DirectX.ConvertScene(scene);

	// �� ������ �ﰢ��ȭ �� �� �ִ� ��� ��带 �ﰢ��ȭ ��Ų��. 
	geometryConverter = new FbxGeometryConverter(manager);

	geometryConverter->Triangulate(scene, true, true);

	// importer �ı�
	importer->Destroy();
}

void FBXParser::LoadMesh(fbxsdk::FbxNode* node)
{
	fbxsdk::FbxMesh* mesh = node->GetMesh();

	std::shared_ptr<FBXMeshInfo> fbxMeshInfo = std::make_shared<FBXMeshInfo>();

	fbxModel->fbxMeshInfoList.push_back(fbxMeshInfo);

	std::shared_ptr<FBXMeshInfo>& meshInfo = fbxModel->fbxMeshInfoList.back();

	FbxAMatrix nodeTransform = GetTransformMatrix(node);
	DirectX::SimpleMath::Matrix nodeMatrix = ConvertMatrix(nodeTransform);
	meshInfo->nodeTM = nodeMatrix;

	meshInfo->meshName = mesh->GetName();

	const int vertexCount = mesh->GetControlPointsCount();
	meshInfo->meshVertexList.resize(vertexCount);

	// �ߺ� üũ�� ���� ��
	std::vector<bool> isVertex(vertexCount, false);

	// Position������ ������(�� �ٲ㼭 ������)
	FbxVector4* controlPoints = mesh->GetControlPoints();
	for (int i = 0; i < vertexCount; ++i)
	{
		DirectX::SimpleMath::Matrix localInverse = XMMatrixInverse(nullptr, meshInfo->nodeTM);
		DirectX::XMVECTOR vertexVec = { controlPoints[i].mData[0], controlPoints[i].mData[1], controlPoints[i].mData[2] };
		DirectX::XMVECTOR vertexXworldTM = XMVector3Transform(vertexVec, localInverse);
		DirectX::XMFLOAT3 vertex;
		DirectX::XMStoreFloat3(&vertex, vertexXworldTM);

		meshInfo->meshVertexList[i].position.x = static_cast<float>(vertex.x);
		meshInfo->meshVertexList[i].position.y = static_cast<float>(vertex.y);
		meshInfo->meshVertexList[i].position.z = static_cast<float>(vertex.z);
	}

	// DeformerCount�� 1���� ������ Bone Data�� ���ٰ� ����
	const int deformerCount = mesh->GetDeformerCount(FbxDeformer::eSkin);

	for (int i = 0; i < deformerCount; i++)
	{
		meshInfo->isSkinned = true;

		fbxModel->isSkinnedAnimation = true;	// �ϴ� ���⼭..

		FbxSkin* fbxSkin = static_cast<FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin));

		if (fbxSkin)
		{
			FbxSkin::EType type = fbxSkin->GetSkinningType();

			if (FbxSkin::eRigid == type || FbxSkin::eLinear)
			{
				// FbxCluster�� Skinning������ �ִ� ������ ������ �ش�.
				const int clusterCount = fbxSkin->GetClusterCount();

				for (int j = 0; j < clusterCount; j++)
				{
					FbxCluster* cluster = fbxSkin->GetCluster(j);
					if (cluster->GetLink() == nullptr)
						continue;

					// �ش� ���� ���� ã�ƿ�
					int boneIdx = FindBoneIndex(cluster->GetLink()->GetName());
					assert(boneIdx >= 0);

					// �ش� ���� ������ �޴� ������ �ε��� ������ ����
					const int indicesCount = cluster->GetControlPointIndicesCount();

					for (int k = 0; k < indicesCount; k++)
					{
						double weight = cluster->GetControlPointWeights()[k];	// �ش� ���������� ����ġ

						int vtxIdx = cluster->GetControlPointIndices()[k];		// �ش� ������ �ε����� ���

						// �ִ� 8���� �Ұž�
						// ���鼭 ����� �ְ� break ��
						for (int weightIdx = 0; weightIdx < 8; weightIdx++)
						{
							if (meshInfo->meshVertexList[vtxIdx].boneIndices[weightIdx] == -1)
							{
								meshInfo->meshVertexList[vtxIdx].weights[weightIdx] = weight;

								meshInfo->meshVertexList[vtxIdx].boneIndices[weightIdx] = boneIdx;

								break;
							}
						}
					}

					// ���������� ���� BoneOffsetMatrix�� ���ؾ��Ѵ�.
					// GetTrasnformLinkMatrix�� �ش� Bone�� ���� Matrix�� ��� GetTransformMatrix�δ� �θ���� ���Ե� Matrix�� ��´�.
					// �ش� �θ� ����� ������� ���ϸ� �θ���ı��� �ڱ� �ڽ��� ������ ���� ��İ��� ���� �Ǽ� ���� ������ ��� ���� ���´�.
					FbxAMatrix matClusterTransformMatrix;
					FbxAMatrix matClusterLinkTransformMatrix;

					cluster->GetTransformMatrix(matClusterTransformMatrix);				// The transformation of the mesh at binding time 
					cluster->GetTransformLinkMatrix(matClusterLinkTransformMatrix);		// The transformation of the cluster(joint) at binding time from joint space to world space 

					// Bone Matrix ����..
					DirectX::SimpleMath::Matrix clusterMatrix = ConvertMatrix(matClusterTransformMatrix);
					DirectX::SimpleMath::Matrix clusterlinkMatrix = ConvertMatrix(matClusterLinkTransformMatrix);

					// BindPose ����� ������
					FbxAMatrix geometryTransform = GetTransformMatrix(mesh->GetNode());
					DirectX::SimpleMath::Matrix geometryMatrix = ConvertMatrix(geometryTransform);

					// OffsetMatrix�� WorldBindPose�� �����
					DirectX::SimpleMath::Matrix offsetMatrix = clusterMatrix * clusterlinkMatrix.Invert() * geometryMatrix;

					fbxModel->fbxBoneInfoList[boneIdx]->offsetMatrix = offsetMatrix;

					const int animCount = fbxModel->animationClipList.size();

					// Ű������ �Ľ��� ���� ��������..
					/*if (!isParsingAnim)
					{
						for (int animIdx = 0; animIdx < animCount; animIdx++)
							LoadKeyFrame(animIdx, mesh->GetNode(), cluster, boneIdx);
					}*/
				}

				isParsingAnim = true;
			}
		}
	}

	const int polygonSize = mesh->GetPolygonSize(0);

	assert(polygonSize == 3);		// �ﰢ���� �ƴϸ� ����

	int arrIdx[3];
	int vertexCounter = 0;			// ������ ����

	const int triCount = mesh->GetPolygonCount(); // �޽��� �ﰢ�� ������ �����´�

	for (int i = 0; i < triCount; i++) // �ﰢ���� ����
	{
		for (int j = 0; j < 3; j++) // �ﰢ���� �� ���� �������� ����
		{
			int controlPointIndex = mesh->GetPolygonVertex(i, j); // �������� �ε��� ����

			// ���Ծ��� controlPointIndex ��� ���ο� ���ؽ� ���� �� controlPointIndex �� �ٲ��ֱ� 
			if (isVertex[controlPointIndex] == true)
			{
				Vertex vertex;
				vertex.position = meshInfo->meshVertexList[controlPointIndex].position;	// �������� ����

				// ����ġ ���� ����
				for (int weightIdx = 0; weightIdx < 8; weightIdx++)
				{
					vertex.weights[weightIdx] = meshInfo->meshVertexList[controlPointIndex].weights[weightIdx];

					vertex.boneIndices[weightIdx] = meshInfo->meshVertexList[controlPointIndex].boneIndices[weightIdx];
				}

				meshInfo->meshVertexList.push_back(vertex);								// ���ο� ���ؽ� ����

				controlPointIndex = meshInfo->meshVertexList.size() - 1;				// index ���ο� ���ؽ� ���� �ٲٱ�

				isVertex.push_back(true);
			}

			arrIdx[j] = controlPointIndex;

			// uv ������ �����´�.
			GetUV(mesh, meshInfo, controlPointIndex, mesh->GetTextureUVIndex(i, j));

			// normal ������ �����´�.
			GetNormal(mesh, meshInfo, controlPointIndex, vertexCounter);

			vertexCounter++;

			// ���Ծ��� controlPointIndex�� true
			isVertex[controlPointIndex] = true;
		}

		meshInfo->indices.push_back(arrIdx[0]);
		meshInfo->indices.push_back(arrIdx[1]);
		meshInfo->indices.push_back(arrIdx[2]);
	}

	// tangent ������ �����´�.
	GetTangent(meshInfo);
}

/// <summary>
/// FBXModel�� fbxBoneInfoList�� bone���� ���� �����Ѵ�.
/// ������ boneName, parentIndex �� ����.
/// </summary>
void FBXParser::ProcessBones(fbxsdk::FbxNode* node, int idx, int parentIdx)
{
	FbxNodeAttribute* attribute = node->GetNodeAttribute();

	if (attribute && attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		std::shared_ptr<FBXBoneInfo> fbxBoneInfo = std::make_shared<FBXBoneInfo>();

		fbxBoneInfo->boneName = node->GetName();

		fbxBoneInfo->parentIndex = parentIdx;

		FbxAMatrix nodeTransform = GetTransformMatrix(node);
		DirectX::SimpleMath::Matrix nodeMatrix = ConvertMatrix(nodeTransform);
		fbxBoneInfo->nodeMatrix = nodeMatrix;

		fbxModel->fbxBoneInfoList.push_back(fbxBoneInfo);

		ProcessAnimationData(node);
	}

	const int childCount = node->GetChildCount();

	for (int i = 0; i < childCount; i++)
		ProcessBones(node->GetChild(i), static_cast<int>(fbxModel->fbxBoneInfoList.size()), idx);
}

void FBXParser::LoadMaterial(fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	std::shared_ptr<FBXMaterialInfo> material = std::make_shared<FBXMaterialInfo>();

	material->materialName = surfaceMaterial->GetName();

	material->albedoMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sDiffuse);
	material->normalMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sNormalMap);
	material->roughnessMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sShininess);
	material->emissiveMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sEmissive);

	if (material->albedoMap != L"") { material->isAlbedo = true; }
	if (material->normalMap != L"") { material->isNormal = true; }
	if (material->roughnessMap != L"") { material->isRoughness = true; }
	if (material->emissiveMap != L"") { material->isEmissive = true; }

	// �޽ÿ��� ���͸��� �̸���
	fbxModel->fbxMeshInfoList.back()->materialName = surfaceMaterial->GetName();

	// ���͸��� ����Ʈ�� �߰�
	fbxModel->materialList.push_back(material);
}

void FBXParser::LoadAnimation()
{
	// animNames Array�ȿ� ��� �ִϸ��̼� �̸��� ����.
	scene->FillAnimStackNameArray(OUT animNames);

	const int animCount = animNames.GetCount();

	for (int i = 0; i < animCount; i++)
	{
		FbxAnimStack* animStack = scene->FindMember<FbxAnimStack>(animNames[i]->Buffer());

		if (animStack == nullptr)
			continue;

		std::shared_ptr<FBXAnimClipInfo> animClip = std::make_shared<FBXAnimClipInfo>();
		animClip->animationName = animStack->GetName();						// �ִϸ��̼� �̸�

		// ���۽ð�, ����ð�, �ʴ� �����ӿ� ���� ����
		FbxTakeInfo* takeInfo = scene->GetTakeInfo(animStack->GetName());
		double startTime = takeInfo->mLocalTimeSpan.GetStart().GetSecondDouble();
		double endTime = takeInfo->mLocalTimeSpan.GetStop().GetSecondDouble();
		double frameRate = (float)FbxTime::GetFrameRate(scene->GetGlobalSettings().GetTimeMode());

		animClip->frameRate = frameRate;

		if (startTime < endTime)
		{
			animClip->totalKeyFrame = (int)((endTime - startTime) * (double)frameRate);
			animClip->endKeyFrame = (int)((endTime - startTime) * (double)frameRate);
			animClip->tickPerFrame = (endTime - startTime) / animClip->totalKeyFrame;
			animClip->startKeyFrame = (int)(startTime)*animClip->totalKeyFrame;
		}

		// �ִ�Ŭ���� Ű�������� ���� ���� ��ŭ
		//animClip->keyFrameList.resize(fbxModel->fbxBoneInfoList.size());

		fbxModel->animationClipList.push_back(animClip);
	}
}

void FBXParser::LoadKeyFrame(int animIndex, FbxNode* node, FbxCluster* cluster, int boneIdx)
{
	std::shared_ptr<FBXKeyFrameInfo> fbxKeyFrameInfo = std::make_shared<FBXKeyFrameInfo>();

	FbxTime::EMode timeMode = scene->GetGlobalSettings().GetTimeMode();

	FbxAnimStack* animStack = scene->FindMember<FbxAnimStack>(animNames[animIndex]->Buffer());
	scene->SetCurrentAnimationStack(OUT animStack);

	for (FbxLongLong frame = 0; frame < fbxModel->animationClipList[animIndex]->totalKeyFrame; frame++)
	{
		std::shared_ptr<FBXKeyFrameInfo> keyFrameInfo = std::make_shared<FBXKeyFrameInfo>();

		FbxTime fbxTime = 0;

		fbxTime.SetFrame(frame, timeMode);

		// Local Transform = �θ� Bone�� Global Transform�� inverse Transform * �ڽ� Bone�� Global Transform;
		//FbxAMatrix localTransform = node->EvaluateLocalTransform(fbxTime);
		FbxAMatrix worldTransform = cluster->GetLink()->EvaluateLocalTransform(fbxTime);

		DirectX::SimpleMath::Matrix worldTM = ConvertMatrix(worldTransform);

		DirectX::XMVECTOR worldScale;
		DirectX::XMVECTOR worldRot;
		DirectX::XMVECTOR worldPos;

		//XMMatrixDecompose(&localScale, &localRot, &localPos, localTM);
		XMMatrixDecompose(&worldScale, &worldRot, &worldPos, worldTM);

		keyFrameInfo->time = fbxTime.GetSecondDouble();

		//keyFrameInfo->localTransform = DirectX::SimpleMath::Vector3(localPos);
		//keyFrameInfo->localRotation = DirectX::SimpleMath::Quaternion(localRot);
		//keyFrameInfo->localScale = DirectX::SimpleMath::Vector3(localScale);

		fbxModel->animationClipList[animIndex]->keyFrameList[boneIdx].push_back(keyFrameInfo);
	}
}

void FBXParser::ProcessAnimationData(FbxNode* node)
{
	const int animCount = fbxModel->animationClipList.size();

	std::vector<std::shared_ptr<FBXKeyFrameInfo>> keyFrameList;

	for (int animIdx = 0; animIdx < animCount; animIdx++)
	{
		std::shared_ptr<FBXKeyFrameInfo> fbxKeyFrameInfo = std::make_shared<FBXKeyFrameInfo>();

		FbxTime::EMode timeMode = scene->GetGlobalSettings().GetTimeMode();

		FbxAnimStack* animStack = scene->FindMember<FbxAnimStack>(animNames[animIdx]->Buffer());
		scene->SetCurrentAnimationStack(OUT animStack);

		for (FbxLongLong frame = 0; frame < fbxModel->animationClipList[animIdx]->totalKeyFrame; frame++)
		{
			std::shared_ptr<FBXKeyFrameInfo> keyFrameInfo = std::make_shared<FBXKeyFrameInfo>();

			FbxTime fbxTime = 0;

			fbxTime.SetFrame(frame, timeMode);

			// Local Transform = �θ� Bone�� Global Transform�� inverse Transform * �ڽ� Bone�� Global Transform;
			FbxAMatrix localTransform = node->EvaluateGlobalTransform(fbxTime);

			if (FbxNode* parent = node->GetParent())
			{
				FbxNodeAttribute* ParentAttribute = parent->GetNodeAttribute();

				if (ParentAttribute && ParentAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
				{
					FbxAMatrix GlobalParentTransform = parent->EvaluateGlobalTransform(fbxTime);

					localTransform = GlobalParentTransform.Inverse() * localTransform;
				}
			}

			DirectX::SimpleMath::Matrix localTM = ConvertMatrix(localTransform);

			DirectX::XMVECTOR localScale;
			DirectX::XMVECTOR localRot;
			DirectX::XMVECTOR localPos;
			XMMatrixDecompose(&localScale, &localRot, &localPos, localTM);

			keyFrameInfo->time = fbxTime.GetSecondDouble();

			keyFrameInfo->localTransform = DirectX::SimpleMath::Vector3(localPos);
			keyFrameInfo->localRotation = DirectX::SimpleMath::Quaternion(localRot);
			keyFrameInfo->localScale = DirectX::SimpleMath::Vector3(localScale);

			keyFrameList.push_back(keyFrameInfo);
		}

		fbxModel->animationClipList[animIdx]->keyFrameList.push_back(keyFrameList);
	}
}

int FBXParser::FindBoneIndex(std::string boneName)
{
	for (int i = 0; i < fbxModel->fbxBoneInfoList.size(); ++i)
	{
		if (fbxModel->fbxBoneInfoList[i]->boneName == boneName)
			return i;
	}

	return -1;
}

DirectX::SimpleMath::Matrix FBXParser::GetNodeTM(fbxsdk::FbxNode* node)
{
	FbxMatrix localpos = scene->GetAnimationEvaluator()->GetNodeLocalTransform(node);

	DirectX::SimpleMath::Matrix localTM = ConvertMatrix(localpos);

	return localTM;
}

void FBXParser::GetNormal(fbxsdk::FbxMesh* mesh, std::shared_ptr<FBXMeshInfo>& meshInfo, int idx, int vertexCounter)
{
	if (mesh->GetElementNormalCount() == 0)
		return;

	FbxGeometryElementNormal* normal = mesh->GetElementNormal();
	int normalIdx = 0;

	// �ε����� �������� ��� ���� ����
	// ���ؽ� ���ø��� �ʿ��ϴ�.
	if (normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
			normalIdx = vertexCounter;
		else
			normalIdx = normal->GetIndexArray().GetAt(vertexCounter);
	}
	// ������ �������� ��� ���� ����.
	else if (normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
			normalIdx = idx;
		else
			normalIdx = normal->GetIndexArray().GetAt(idx);
	}

	FbxVector4 vec = normal->GetDirectArray().GetAt(normalIdx);

	meshInfo->meshVertexList[idx].normal.x = static_cast<float>(vec.mData[0]);
	meshInfo->meshVertexList[idx].normal.y = static_cast<float>(vec.mData[1]);
	meshInfo->meshVertexList[idx].normal.z = static_cast<float>(vec.mData[2]);
}

void FBXParser::GetTangent(std::shared_ptr<FBXMeshInfo>& meshInfo)
{
	for (int i = 0; i < meshInfo->indices.size(); i += 3) // �ﰢ���� ����
	{
		int i0 = meshInfo->indices[i];
		int i1 = meshInfo->indices[i + 1];
		int i2 = meshInfo->indices[i + 2];

		// e1 = p1 - p0, e2 = p2 - p0
		DirectX::SimpleMath::Vector3 e1 = meshInfo->meshVertexList[i1].position - meshInfo->meshVertexList[i0].position;
		DirectX::SimpleMath::Vector3 e2 = meshInfo->meshVertexList[i2].position - meshInfo->meshVertexList[i0].position;

		float x1 = meshInfo->meshVertexList[i1].uv.x - meshInfo->meshVertexList[i0].uv.x; // u1 - u0
		float y1 = meshInfo->meshVertexList[i1].uv.y - meshInfo->meshVertexList[i0].uv.y; // v1 - v0
		float x2 = meshInfo->meshVertexList[i2].uv.y - meshInfo->meshVertexList[i0].uv.x; // u2 - u0
		float y2 = meshInfo->meshVertexList[i2].uv.y - meshInfo->meshVertexList[i0].uv.y; // v2 - v0

		float det = (x1 * y2) - (x2 * y1); // ad - bc
		if (det == 0.f) { det = 0.00001f; }
		float r = 1.0f / det;

		DirectX::SimpleMath::Vector3 tangent = (e1 * y2 - e2 * y1) * r;

		meshInfo->meshVertexList[i0].tangent += tangent;
		meshInfo->meshVertexList[i1].tangent += tangent;
		meshInfo->meshVertexList[i2].tangent += tangent;
	}
}

void FBXParser::GetUV(fbxsdk::FbxMesh* mesh, std::shared_ptr<FBXMeshInfo>& meshInfo, int idx, int uvIndex)
{
	FbxVector2 uv = mesh->GetElementUV()->GetDirectArray().GetAt(uvIndex);

	meshInfo->meshVertexList[idx].uv.x = static_cast<float>(uv.mData[0]);
	meshInfo->meshVertexList[idx].uv.y = 1.f - static_cast<float>(uv.mData[1]);
}

std::wstring FBXParser::GetTextureRelativeName(fbxsdk::FbxSurfaceMaterial* surface, const char* materialProperty)
{
	std::string name;

	fbxsdk::FbxProperty textureProperty = surface->FindProperty(materialProperty);

	if (textureProperty.IsValid())
	{
		int count = textureProperty.GetSrcObjectCount();

		if (1 <= count)
		{
			FbxFileTexture* texture = textureProperty.GetSrcObject<FbxFileTexture>(0);
			if (texture)
				name = texture->GetRelativeFileName();
		}
	}

	std::wstring wstr = L"";

	wstr.assign(name.begin(), name.end());

	std::wstring filename = fs::path(wstr).filename();

	return filename;
}

FbxAMatrix FBXParser::GetTransformMatrix(FbxNode* node)
{
	const FbxVector4 translation = node->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 rotation = node->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 scaling = node->GetGeometricScaling(FbxNode::eSourcePivot);
	return FbxAMatrix(translation, rotation, scaling);
}
