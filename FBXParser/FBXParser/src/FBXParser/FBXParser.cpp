#include "pch.h"
#include "FBXParser/FBXParser.h"
#include "ParserData/ParserData.h"
#include "Serializer/BinarySerializer.h"

FBXParser::FBXParser()
{}

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

	std::shared_ptr<BinarySerializer> s = std::make_shared<BinarySerializer>();
	s->SaveBinaryFile(fbxModel, "text", path);

	return fbxModel;
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

	FbxSystemUnit lFbxFileSystemUnit = scene->GetGlobalSettings().GetSystemUnit();
	FbxSystemUnit lFbxOriginSystemUnit = scene->GetGlobalSettings().GetOriginalSystemUnit();
	double factor = lFbxOriginSystemUnit.GetScaleFactor();

	const FbxSystemUnit::ConversionOptions lConversionOptions =
	{
	  true,
	  true,
	  true,
	  true,
	  true,
	  true
	};
	lFbxFileSystemUnit.m.ConvertScene(scene, lConversionOptions);
	lFbxOriginSystemUnit.m.ConvertScene(scene, lConversionOptions);

	// GeometryConverter ��ü ����
	geometryConverter = new FbxGeometryConverter(manager);

	// �� ������ �ﰢ��ȭ �� �� �ִ� ��� ��带 �ﰢ��ȭ ��Ų��. 
	geometryConverter->Triangulate(scene, true, true);

	// ���� ���͸����� �ϳ��� �޽��� �Ҵ�Ȱ��� �ϳ��� �޽��� �ϳ��� ���͸���� ��������
	geometryConverter->SplitMeshesPerMaterial(scene, true);

	// importer �ı�
	importer->Destroy();
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
		// SplitMeshesPerMaterial �Լ��� ���� �޽ð� �и��� ��� 
		for (int meshCnt = 0; meshCnt < node->GetNodeAttributeCount(); meshCnt++)
		{
			fbxsdk::FbxMesh* mesh = (fbxsdk::FbxMesh*)node->GetNodeAttributeByIndex(meshCnt);

			LoadMesh(node, mesh);

			fbxsdk::FbxLayerElementMaterial* findMatIndex = mesh->GetElementMaterial(0);

			if (findMatIndex != nullptr)
			{
				int index = findMatIndex->GetIndexArray().GetAt(0);

				fbxsdk::FbxSurfaceMaterial* surfaceMaterial = mesh->GetNode()->GetSrcObject<fbxsdk::FbxSurfaceMaterial>(index);

				LoadMaterial(surfaceMaterial);
			}
		}
	}		

	// Tree ���� ��� ȣ��
	const int childCount = node->GetChildCount();

	for (int i = 0; i < childCount; ++i)
		ProcessMesh(node->GetChild(i), FbxNodeAttribute::eMesh);
}


void FBXParser::LoadMesh(fbxsdk::FbxNode* node, fbxsdk::FbxMesh* mesh)
{
	std::shared_ptr<FBXMeshInfo> fbxMeshInfo = std::make_shared<FBXMeshInfo>();

	fbxModel->fbxMeshInfoList.push_back(fbxMeshInfo);

	std::shared_ptr<FBXMeshInfo>& meshInfo = fbxModel->fbxMeshInfoList.back();

	// Node TM �ֱ�
	FbxAMatrix nodeTransform = scene->GetAnimationEvaluator()->GetNodeGlobalTransform(node);

	DirectX::SimpleMath::Matrix nodeMatrix = ConvertMatrix(nodeTransform);

	const auto roll = -90.0f * DirectX::XM_PI / 180.0f;

	const auto pitch = 180.0f * DirectX::XM_PI / 180.0f;

	DirectX::SimpleMath::Quaternion q = DirectX::XMQuaternionRotationRollPitchYaw(roll, pitch, 0.0f);

	nodeMatrix *= XMMatrixRotationQuaternion(q);

	meshInfo->nodeTM = nodeMatrix;

	// mesh �̸� �ֱ�
	meshInfo->meshName = mesh->GetName();

	// Node Parent ã��
	std::string parentName = node->GetParent()->GetName();

	// �θ� �̸� �ֱ�
	meshInfo->parentName = parentName;

	const int vertexCount = mesh->GetControlPointsCount();
	meshInfo->meshVertexList.resize(vertexCount);

	// Position������ ������(�� �ٲ㼭 ������)
	FbxVector4* controlPoints = mesh->GetControlPoints();
	for (int i = 0; i < vertexCount; ++i)
	{
		/*DirectX::SimpleMath::Matrix localInverse = XMMatrixInverse(nullptr, meshInfo->nodeTM);
		DirectX::XMVECTOR vertexVec = { controlPoints[i].mData[0], controlPoints[i].mData[2], controlPoints[i].mData[1] };
		DirectX::XMVECTOR vertexXworldTM = XMVector3Transform(vertexVec, localInverse);
		DirectX::XMFLOAT3 vertex;
		DirectX::XMStoreFloat3(&vertex, vertexXworldTM);

		meshInfo->meshVertexList[i].position.x = static_cast<float>(vertex.x);
		meshInfo->meshVertexList[i].position.y = static_cast<float>(vertex.y);
		meshInfo->meshVertexList[i].position.z = static_cast<float>(vertex.z);*/

		meshInfo->meshVertexList[i].position.x = static_cast<float>(controlPoints[i].mData[0]);
		meshInfo->meshVertexList[i].position.y = static_cast<float>(controlPoints[i].mData[2]);
		meshInfo->meshVertexList[i].position.z = static_cast<float>(controlPoints[i].mData[1]);
	}
	 
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
				}
			}
		}
	}

	const int polygonSize = mesh->GetPolygonSize(0);

	assert(polygonSize == 3);		// �ﰢ���� �ƴϸ� ����

	int arrIdx[3];
	int vertexCounter = 0;			// ������ ����

	const int triCount = mesh->GetPolygonCount(); // �޽��� �ﰢ�� ������ �����´�

	// �ߺ� üũ�� ���� ��
	std::vector<bool> isVertex(vertexCount, false);

	std::unordered_map<int, std::vector<std::pair<DirectX::SimpleMath::Vector2, DirectX::SimpleMath::Vector3>>> vertexMap;

	std::tuple<int ,DirectX::SimpleMath::Vector2, DirectX::SimpleMath::Vector3> vertexTuple;

	for (int i = 0; i < triCount; i++) // �ﰢ���� ����
	{
		for (int j = 0; j < 3; j++)	   // �ﰢ���� �� ���� �������� ����
 		{
			int controlPointIndex = mesh->GetPolygonVertex(i, j); // �������� �ε��� ����

			arrIdx[j] = controlPointIndex;

			//// map�� ������ insert
			//if (vertexMap.find(controlPointIndex) == vertexMap.end())
			//{
			//	// uv ������ �����´�.
			//	if (mesh->GetElementUVCount() >= 1)
			//		GetUV(mesh, meshInfo, controlPointIndex, mesh->GetTextureUVIndex(i, j));

			//	// normal ������ �����´�.
			//	if (mesh->GetElementNormalCount() >= 1)
			//		GetNormal(mesh, meshInfo, controlPointIndex, vertexCounter);

			//	vertexMap[controlPointIndex].push_back(
			//		std::make_pair(meshInfo->meshVertexList[controlPointIndex].uv
			//		, meshInfo->meshVertexList[controlPointIndex].normal));
			//}
			//// map�� �ִ��Ŷ�� �����ش�.
			//else
			//{
			//	FbxVector2 uv = mesh->GetElementUV()->GetDirectArray().GetAt(mesh->GetTextureUVIndex(i, j));

			//	float uvX = static_cast<float>(uv.mData[0]);
			//	float uvY = 1.f - static_cast<float>(uv.mData[1]);

			//	FbxGeometryElementNormal* normal = mesh->GetElementNormal();
			//	int normalIdx = 0;

			//	// �ε����� �������� ��� ���� ����
			//	// ���ؽ� ���ø��� �ʿ��ϴ�.
			//	if (normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			//	{
			//		if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
			//			normalIdx = vertexCounter;
			//		else
			//			normalIdx = normal->GetIndexArray().GetAt(vertexCounter);
			//	}
			//	// ������ �������� ��� ���� ����.
			//	else if (normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			//	{
			//		if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
			//			normalIdx = controlPointIndex;
			//		else
			//			normalIdx = normal->GetIndexArray().GetAt(controlPointIndex);
			//	}

			//	FbxVector4 vec = normal->GetDirectArray().GetAt(normalIdx);

			//	float normalX = static_cast<float>(vec.mData[0]);
			//	float normalY = static_cast<float>(vec.mData[2]);
			//	float normalZ = static_cast<float>(vec.mData[1]);

			//	bool isUV = false;
			//	bool isNormal = false;
			//	bool isNew = true;		// ���ο� ���ؽ��� �����ؾ���

			//	for (int controlCnt = 0; controlCnt < vertexMap[controlPointIndex].size(); controlCnt++)
			//	{
			//		if (uvX == vertexMap[controlPointIndex][controlCnt].first.x
			//			&& uvY == vertexMap[controlPointIndex][controlCnt].first.y)
			//		{
			//			isUV = true;
			//		}

			//		if (normalX == vertexMap[controlPointIndex][controlCnt].second.x
			//			&& normalY == vertexMap[controlPointIndex][controlCnt].second.y
			//			&& normalZ == vertexMap[controlPointIndex][controlCnt].second.z)
			//		{
			//			isNormal = true;
			//		}

			//		// �����Ѱ� ã�Ҵٸ� ������ �ִ� ���ؽ��� ����.
			//		if (isUV == true && isNormal == true)
			//		{
			//			meshInfo->meshVertexList[controlPointIndex].uv.x = static_cast<float>(uvX);
			//			meshInfo->meshVertexList[controlPointIndex].uv.y = static_cast<float>(uvY);

			//			meshInfo->meshVertexList[controlPointIndex].normal.x = static_cast<float>(normalX);
			//			meshInfo->meshVertexList[controlPointIndex].normal.y = static_cast<float>(normalY);
			//			meshInfo->meshVertexList[controlPointIndex].normal.z = static_cast<float>(normalZ);
			//			
			//			isNew = false;
			//			break;
			//		}

			//		isUV = false;
			//		isNormal = false;
			//	}

			//	// uv, normal�� �����Ѱ� ��ã�Ҵٸ� ���θ���� �ش�.
			//	if (isNew == true)
			//	{
			//		DirectX::SimpleMath::Vector2 uv = { uvX, uvY };
			//		DirectX::SimpleMath::Vector3 normal = { normalX, normalY, normalZ };

			//		vertexMap[controlPointIndex].push_back(std::make_pair(uv, normal));

			//		Vertex vertex;
			//		vertex.position = meshInfo->meshVertexList[controlPointIndex].position;	// �������� ����

			//		// ����ġ ���� ����
			//		for (int weightIdx = 0; weightIdx < 8; weightIdx++)
			//		{
			//			vertex.weights[weightIdx] = meshInfo->meshVertexList[controlPointIndex].weights[weightIdx];

			//			vertex.boneIndices[weightIdx] = meshInfo->meshVertexList[controlPointIndex].boneIndices[weightIdx];
			//		}

			//		meshInfo->meshVertexList.push_back(vertex);								// ���ο� ���ؽ� ����

			//		controlPointIndex = meshInfo->meshVertexList.size() - 1;				// index ���ο� ���ؽ� ���� �ٲٱ�

			//		arrIdx[j] = controlPointIndex;

			//		meshInfo->meshVertexList[controlPointIndex].uv = uv;
			//		meshInfo->meshVertexList[controlPointIndex].normal = normal;
			//	}
			//}

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
			if(mesh->GetElementUVCount() >= 1)
				GetUV(mesh, meshInfo, controlPointIndex, mesh->GetTextureUVIndex(i, j));

			// normal ������ �����´�.
			if(mesh->GetElementNormalCount() >= 1)
				GetNormal(mesh, meshInfo, controlPointIndex, vertexCounter);

			vertexCounter++;

			// ���Ծ��� controlPointIndex�� true
			isVertex[controlPointIndex] = true;
		}

		meshInfo->indices.push_back(arrIdx[0]);
		meshInfo->indices.push_back(arrIdx[2]);
		meshInfo->indices.push_back(arrIdx[1]);
	}

	// tangent ������ �����´�.
	if (mesh->GetElementNormalCount() >= 1)
		GetTangent(meshInfo);
}

/// <summary>
/// FBXModel�� fbxBoneInfoList�� bone���� ���� �����Ѵ�.
/// ������ boneName, parentIndex �� ����.
/// </summary>
void FBXParser::ProcessBones(fbxsdk::FbxNode* node, int idx, int parentIdx)
{
	fbxsdk::FbxNodeAttribute* attribute = node->GetNodeAttribute();

	if (attribute && attribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
	{
		std::shared_ptr<FBXBoneInfo> fbxBoneInfo = std::make_shared<FBXBoneInfo>();

		fbxBoneInfo->boneName = node->GetName();

		fbxBoneInfo->parentIndex = parentIdx;

		FbxAMatrix nodeTransform = node->EvaluateGlobalTransform(fbxsdk::FbxTime(0));

		DirectX::SimpleMath::Matrix nodeMatrix = ConvertMatrix(nodeTransform);

		const auto roll = -90.0f * DirectX::XM_PI / 180.0f;

		const auto pitch = 180.0f * DirectX::XM_PI / 180.0f;

		DirectX::SimpleMath::Quaternion q = DirectX::XMQuaternionRotationRollPitchYaw(roll, pitch, 0.0f);

		nodeMatrix *= XMMatrixRotationQuaternion(q);

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
	std::string matName = surfaceMaterial->GetName();

	// �޽ÿ��� ���͸��� �̸���
	fbxModel->fbxMeshInfoList.back()->materialName = matName;

	auto it = find_if(fbxModel->materialList.begin(), fbxModel->materialList.end(), [&name = matName](const std::shared_ptr<FBXMaterialInfo>& s)->bool {return (s->materialName == name); });

	// �̹� �ִ� material�̸� return
	if(it != fbxModel->materialList.end())
		return;

	std::shared_ptr<FBXMaterialInfo> material = std::make_shared<FBXMaterialInfo>();

	material->materialName = surfaceMaterial->GetName();

	if (surfaceMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		// Ambient Data
		material->material_Ambient.x = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Ambient.Get()[0]) * 10.0f;
		material->material_Ambient.y = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Ambient.Get()[1]) * 10.0f;
		material->material_Ambient.z = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Ambient.Get()[2]) * 10.0f;
		material->material_Ambient.w = 1.0f;

		// Diffuse Data
		material->material_Diffuse.x = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Diffuse.Get()[0]);
		material->material_Diffuse.y = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Diffuse.Get()[1]);
		material->material_Diffuse.z = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Diffuse.Get()[2]);
		material->material_Diffuse.w = 1.0f;
						
		// Specular Data
		material->material_Specular.x = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Specular.Get()[0]);
		material->material_Specular.y = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Specular.Get()[1]);
		material->material_Specular.z = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Specular.Get()[2]);
		material->material_Specular.w = 1.0f;
						
		// Emissive Data
		material->material_Emissive.x = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Emissive.Get()[0]);
		material->material_Emissive.y = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Emissive.Get()[1]);
		material->material_Emissive.z = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Emissive.Get()[2]);
		material->material_Emissive.w = 1.0f;

		// Transparecy Data
		material->material_Transparency = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->TransparencyFactor.Get());

		// Shininess Data
		material->roughness = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Shininess.Get());

		// Reflectivity Data
		material->material_Reflectivity = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->ReflectionFactor.Get());
	}
	else if (surfaceMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		// Ambient Data
		material->material_Ambient.x = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Ambient.Get()[0]);
		material->material_Ambient.y = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Ambient.Get()[1]);
		material->material_Ambient.z = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Ambient.Get()[2]);
		material->material_Ambient.w = 1.0f;

		// Diffuse Data
		material->material_Diffuse.x = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Diffuse.Get()[0]);
		material->material_Diffuse.y = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Diffuse.Get()[1]);
		material->material_Diffuse.z = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Diffuse.Get()[2]);
		material->material_Diffuse.w = 1.0f;

		// Emissive Data
		material->material_Emissive.x = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Emissive.Get()[0]);
		material->material_Emissive.y = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Emissive.Get()[1]);
		material->material_Emissive.z = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Emissive.Get()[2]);
		material->material_Emissive.w = 1.0f;

		// Transparecy Data
		material->material_Transparency = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->TransparencyFactor.Get());
	}

	material->albedoMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sDiffuse);
	material->normalMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sNormalMap);
	material->roughnessMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sShininess);
	material->emissiveMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sEmissive);

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

		fbxModel->animationClipList.push_back(animClip);
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

			DirectX::SimpleMath::Matrix localTM;

			if (FbxNode* parent = node->GetParent())
			{
				FbxNodeAttribute* ParentAttribute = parent->GetNodeAttribute();

				if (ParentAttribute && ParentAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
				{
					FbxAMatrix GlobalParentTransform = parent->EvaluateGlobalTransform(fbxTime);

					localTransform = GlobalParentTransform.Inverse() * localTransform;

					localTM = ConvertMatrix(localTransform);
				}
				else
				{
					localTM = ConvertMatrix(localTransform);

					const auto roll = -90.0f * DirectX::XM_PI / 180.0f;

					const auto pitch = 180.0f * DirectX::XM_PI / 180.0f;

					DirectX::SimpleMath::Quaternion q = DirectX::XMQuaternionRotationRollPitchYaw(roll, pitch, 0.0f);

					localTM *= XMMatrixRotationQuaternion(q);
				}
			}

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
	meshInfo->meshVertexList[idx].normal.y = static_cast<float>(vec.mData[2]);
	meshInfo->meshVertexList[idx].normal.z = static_cast<float>(vec.mData[1]);
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
