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
	// FBXModel 생성
	fbxModel = std::make_shared<FBXModel>();

	// 파일 데이터 로드
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
	// FBX SDK 관리자 객체 생성
	manager = fbxsdk::FbxManager::Create();

	// IOSettings 객체 생성 및 설정
	fbxsdk::FbxIOSettings* settings = fbxsdk::FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(settings);

	// 씬 객체 생성
	scene = fbxsdk::FbxScene::Create(manager, "");

	// FbxImporter 객체 생성
	importer = fbxsdk::FbxImporter::Create(manager, "");

	// importer 초기화
	importer->Initialize(path.c_str(), -1, manager->GetIOSettings());

	// fbx 파일 내용을 scene 으로 가져온다.
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

	// GeometryConverter 객체 생성
	geometryConverter = new FbxGeometryConverter(manager);

	// 씬 내에서 삼각형화 할 수 있는 모든 노드를 삼각형화 시킨다. 
	geometryConverter->Triangulate(scene, true, true);

	// 여러 머터리얼이 하나의 메쉬에 할당된것을 하나의 메쉬가 하나의 머터리얼로 가지게함
	geometryConverter->SplitMeshesPerMaterial(scene, true);

	// importer 파괴
	importer->Destroy();
}

/// <summary>
/// Mesh들을 로드해서 FBXModel의 fbxMeshInfoList에 저장한다.
/// 버텍스의 정보를 모두 담고 bone에 영향을 받는 mesh가 있다면 bone 가중치도 넣어주고
/// bone에 offsetMatrix 정보도 추가해준다
/// <summary>
void FBXParser::ProcessMesh(fbxsdk::FbxNode* node, FbxNodeAttribute::EType attribute)
{
	fbxsdk::FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();

	if (nodeAttribute && nodeAttribute->GetAttributeType() == attribute)
	{
		// SplitMeshesPerMaterial 함수로 인해 메시가 분리된 경우 
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

	// Tree 구조 재귀 호출
	const int childCount = node->GetChildCount();

	for (int i = 0; i < childCount; ++i)
		ProcessMesh(node->GetChild(i), FbxNodeAttribute::eMesh);
}


void FBXParser::LoadMesh(fbxsdk::FbxNode* node, fbxsdk::FbxMesh* mesh)
{
	std::shared_ptr<FBXMeshInfo> fbxMeshInfo = std::make_shared<FBXMeshInfo>();

	fbxModel->fbxMeshInfoList.push_back(fbxMeshInfo);

	std::shared_ptr<FBXMeshInfo>& meshInfo = fbxModel->fbxMeshInfoList.back();

	// Node TM 넣기
	FbxAMatrix nodeTransform = scene->GetAnimationEvaluator()->GetNodeGlobalTransform(node);

	DirectX::SimpleMath::Matrix nodeMatrix = ConvertMatrix(nodeTransform);

	const auto roll = -90.0f * DirectX::XM_PI / 180.0f;

	const auto pitch = 180.0f * DirectX::XM_PI / 180.0f;

	DirectX::SimpleMath::Quaternion q = DirectX::XMQuaternionRotationRollPitchYaw(roll, pitch, 0.0f);

	nodeMatrix *= XMMatrixRotationQuaternion(q);

	meshInfo->nodeTM = nodeMatrix;

	// mesh 이름 넣기
	meshInfo->meshName = mesh->GetName();

	// Node Parent 찾기
	std::string parentName = node->GetParent()->GetName();

	// 부모 이름 넣기
	meshInfo->parentName = parentName;

	const int vertexCount = mesh->GetControlPointsCount();
	meshInfo->meshVertexList.resize(vertexCount);

	// Position정보를 가져옴(축 바꿔서 가져옴)
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

		fbxModel->isSkinnedAnimation = true;	// 일단 여기서..

		FbxSkin* fbxSkin = static_cast<FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin));

		if (fbxSkin)
		{
			FbxSkin::EType type = fbxSkin->GetSkinningType();

			if (FbxSkin::eRigid == type || FbxSkin::eLinear)
			{
				// FbxCluster는 Skinning정보가 있는 뼈대의 갯수만 준다.
				const int clusterCount = fbxSkin->GetClusterCount();

				for (int j = 0; j < clusterCount; j++)
				{
					FbxCluster* cluster = fbxSkin->GetCluster(j);
					if (cluster->GetLink() == nullptr)
						continue;

					// 해당 본이 뭔지 찾아옴
					int boneIdx = FindBoneIndex(cluster->GetLink()->GetName());
					assert(boneIdx >= 0);

					// 해당 본에 영향을 받는 정점의 인덱스 갯수가 나옴
					const int indicesCount = cluster->GetControlPointIndicesCount();

					for (int k = 0; k < indicesCount; k++)
					{
						double weight = cluster->GetControlPointWeights()[k];	// 해당 정점에서의 가중치

						int vtxIdx = cluster->GetControlPointIndices()[k];		// 해당 정점의 인덱스를 얻기

						// 최대 8개로 할거야
						// 돌면서 빈곳에 넣고 break 함
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

					// 계층구조를 가진 BoneOffsetMatrix를 구해야한다.
					// GetTrasnformLinkMatrix로 해당 Bone의 월드 Matrix를 얻고 GetTransformMatrix로는 부모까지 오게된 Matrix를 얻는다.
					// 해당 부모 행렬의 역행렬을 곱하면 부모행렬기준 자기 자신의 정보에 대한 행렬값만 남게 되서 계층 구조적 행렬 값을 갖는다.
					FbxAMatrix matClusterTransformMatrix;
					FbxAMatrix matClusterLinkTransformMatrix;

					cluster->GetTransformMatrix(matClusterTransformMatrix);				// The transformation of the mesh at binding time 
					cluster->GetTransformLinkMatrix(matClusterLinkTransformMatrix);		// The transformation of the cluster(joint) at binding time from joint space to world space 

					// Bone Matrix 설정..
					DirectX::SimpleMath::Matrix clusterMatrix = ConvertMatrix(matClusterTransformMatrix);
					DirectX::SimpleMath::Matrix clusterlinkMatrix = ConvertMatrix(matClusterLinkTransformMatrix);

					// BindPose 행렬을 구하자
					FbxAMatrix geometryTransform = GetTransformMatrix(mesh->GetNode());
					DirectX::SimpleMath::Matrix geometryMatrix = ConvertMatrix(geometryTransform);

					// OffsetMatrix는 WorldBindPose의 역행렬
					DirectX::SimpleMath::Matrix offsetMatrix = clusterMatrix * clusterlinkMatrix.Invert() * geometryMatrix;

					fbxModel->fbxBoneInfoList[boneIdx]->offsetMatrix = offsetMatrix;
				}
			}
		}
	}

	const int polygonSize = mesh->GetPolygonSize(0);

	assert(polygonSize == 3);		// 삼각형이 아니면 오류

	int arrIdx[3];
	int vertexCounter = 0;			// 정점의 개수

	const int triCount = mesh->GetPolygonCount(); // 메쉬의 삼각형 개수를 가져온다

	// 중복 체크를 위한 것
	std::vector<bool> isVertex(vertexCount, false);

	std::unordered_map<int, std::vector<std::pair<DirectX::SimpleMath::Vector2, DirectX::SimpleMath::Vector3>>> vertexMap;

	std::tuple<int ,DirectX::SimpleMath::Vector2, DirectX::SimpleMath::Vector3> vertexTuple;

	for (int i = 0; i < triCount; i++) // 삼각형의 개수
	{
		for (int j = 0; j < 3; j++)	   // 삼각형은 세 개의 정점으로 구성
 		{
			int controlPointIndex = mesh->GetPolygonVertex(i, j); // 제어점의 인덱스 추출

			arrIdx[j] = controlPointIndex;

			//// map에 없으면 insert
			//if (vertexMap.find(controlPointIndex) == vertexMap.end())
			//{
			//	// uv 정보를 가져온다.
			//	if (mesh->GetElementUVCount() >= 1)
			//		GetUV(mesh, meshInfo, controlPointIndex, mesh->GetTextureUVIndex(i, j));

			//	// normal 정보를 가져온다.
			//	if (mesh->GetElementNormalCount() >= 1)
			//		GetNormal(mesh, meshInfo, controlPointIndex, vertexCounter);

			//	vertexMap[controlPointIndex].push_back(
			//		std::make_pair(meshInfo->meshVertexList[controlPointIndex].uv
			//		, meshInfo->meshVertexList[controlPointIndex].normal));
			//}
			//// map에 있던거라면 비교해준다.
			//else
			//{
			//	FbxVector2 uv = mesh->GetElementUV()->GetDirectArray().GetAt(mesh->GetTextureUVIndex(i, j));

			//	float uvX = static_cast<float>(uv.mData[0]);
			//	float uvY = 1.f - static_cast<float>(uv.mData[1]);

			//	FbxGeometryElementNormal* normal = mesh->GetElementNormal();
			//	int normalIdx = 0;

			//	// 인덱스를 기준으로 노멀 값이 들어간다
			//	// 버텍스 스플릿이 필요하다.
			//	if (normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			//	{
			//		if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
			//			normalIdx = vertexCounter;
			//		else
			//			normalIdx = normal->GetIndexArray().GetAt(vertexCounter);
			//	}
			//	// 정점을 기준으로 노멀 값이 들어간다.
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
			//	bool isNew = true;		// 새로운 버텍스를 제작해야함

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

			//		// 동일한걸 찾았다면 기존에 있던 버텍스를 쓴다.
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

			//	// uv, normal이 동일한걸 못찾았다면 새로만들어 준다.
			//	if (isNew == true)
			//	{
			//		DirectX::SimpleMath::Vector2 uv = { uvX, uvY };
			//		DirectX::SimpleMath::Vector3 normal = { normalX, normalY, normalZ };

			//		vertexMap[controlPointIndex].push_back(std::make_pair(uv, normal));

			//		Vertex vertex;
			//		vertex.position = meshInfo->meshVertexList[controlPointIndex].position;	// 포지션은 동일

			//		// 가중치 정보 동일
			//		for (int weightIdx = 0; weightIdx < 8; weightIdx++)
			//		{
			//			vertex.weights[weightIdx] = meshInfo->meshVertexList[controlPointIndex].weights[weightIdx];

			//			vertex.boneIndices[weightIdx] = meshInfo->meshVertexList[controlPointIndex].boneIndices[weightIdx];
			//		}

			//		meshInfo->meshVertexList.push_back(vertex);								// 새로운 버텍스 삽입

			//		controlPointIndex = meshInfo->meshVertexList.size() - 1;				// index 새로운 버텍스 껄로 바꾸기

			//		arrIdx[j] = controlPointIndex;

			//		meshInfo->meshVertexList[controlPointIndex].uv = uv;
			//		meshInfo->meshVertexList[controlPointIndex].normal = normal;
			//	}
			//}

			// 나왔었던 controlPointIndex 라면 새로운 버텍스 생성 및 controlPointIndex 값 바꿔주기 
			if (isVertex[controlPointIndex] == true)
			{
				Vertex vertex;
				vertex.position = meshInfo->meshVertexList[controlPointIndex].position;	// 포지션은 동일

				// 가중치 정보 동일
				for (int weightIdx = 0; weightIdx < 8; weightIdx++)
				{
					vertex.weights[weightIdx] = meshInfo->meshVertexList[controlPointIndex].weights[weightIdx];

					vertex.boneIndices[weightIdx] = meshInfo->meshVertexList[controlPointIndex].boneIndices[weightIdx];
				}

				meshInfo->meshVertexList.push_back(vertex);								// 새로운 버텍스 삽입

				controlPointIndex = meshInfo->meshVertexList.size() - 1;				// index 새로운 버텍스 껄로 바꾸기

				isVertex.push_back(true);
			}

			arrIdx[j] = controlPointIndex;

			// uv 정보를 가져온다.
			if(mesh->GetElementUVCount() >= 1)
				GetUV(mesh, meshInfo, controlPointIndex, mesh->GetTextureUVIndex(i, j));

			// normal 정보를 가져온다.
			if(mesh->GetElementNormalCount() >= 1)
				GetNormal(mesh, meshInfo, controlPointIndex, vertexCounter);

			vertexCounter++;

			// 나왔었던 controlPointIndex면 true
			isVertex[controlPointIndex] = true;
		}

		meshInfo->indices.push_back(arrIdx[0]);
		meshInfo->indices.push_back(arrIdx[2]);
		meshInfo->indices.push_back(arrIdx[1]);
	}

	// tangent 정보를 가져온다.
	if (mesh->GetElementNormalCount() >= 1)
		GetTangent(meshInfo);
}

/// <summary>
/// FBXModel의 fbxBoneInfoList에 bone들을 전부 저장한다.
/// 정보는 boneName, parentIndex 이 담긴다.
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

	// 메시에는 머터리얼 이름만
	fbxModel->fbxMeshInfoList.back()->materialName = matName;

	auto it = find_if(fbxModel->materialList.begin(), fbxModel->materialList.end(), [&name = matName](const std::shared_ptr<FBXMaterialInfo>& s)->bool {return (s->materialName == name); });

	// 이미 있던 material이면 return
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

	// 머터리얼 리스트에 추가
	fbxModel->materialList.push_back(material);
}

void FBXParser::LoadAnimation()
{
	// animNames Array안에 모든 애니메이션 이름이 담긴다.
	scene->FillAnimStackNameArray(OUT animNames);

	const int animCount = animNames.GetCount();

	for (int i = 0; i < animCount; i++)
	{
		FbxAnimStack* animStack = scene->FindMember<FbxAnimStack>(animNames[i]->Buffer());

		if (animStack == nullptr)
			continue;

		std::shared_ptr<FBXAnimClipInfo> animClip = std::make_shared<FBXAnimClipInfo>();
		animClip->animationName = animStack->GetName();						// 애니메이션 이름

		// 시작시간, 종료시간, 초당 프레임에 대한 정보
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

			// Local Transform = 부모 Bone의 Global Transform의 inverse Transform * 자신 Bone의 Global Transform;
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

	// 인덱스를 기준으로 노멀 값이 들어간다
	// 버텍스 스플릿이 필요하다.
	if (normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
			normalIdx = vertexCounter;
		else
			normalIdx = normal->GetIndexArray().GetAt(vertexCounter);
	}
	// 정점을 기준으로 노멀 값이 들어간다.
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
	for (int i = 0; i < meshInfo->indices.size(); i += 3) // 삼각형의 개수
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
