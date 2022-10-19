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
	// FBXModel 생성
	fbxModel = std::make_shared<FBXModel>();

	// 파일 데이터 로드
	Import(path);

	ParseNode(scene->GetRootNode());

	return fbxModel;
}

void FBXParser::ParseNode(fbxsdk::FbxNode* node)
{
	fbxsdk::FbxNodeAttribute* attribute = node->GetNodeAttribute();

	if (attribute)
	{
		switch (attribute->GetAttributeType())
		{
		case fbxsdk::FbxNodeAttribute::eMesh:
		{
			LoadMesh(node->GetMesh());

		}
		break;
		}
	}

	// Material 로드
	const int materialCount = node->GetMaterialCount();

	for (int i = 0; i < materialCount; ++i)
	{
		fbxsdk::FbxSurfaceMaterial* surfaceMaterial = node->GetMaterial(i);

		LoadMaterial(surfaceMaterial);
	}

	// Tree 구조 재귀 호출
	const int childCount = node->GetChildCount();

	for (int i = 0; i < childCount; ++i)
		ParseNode(node->GetChild(i));
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

	// Axis를 DirectX에 맞게 변환
	scene->GetGlobalSettings().SetAxisSystem(fbxsdk::FbxAxisSystem::DirectX);

	// 씬 내에서 삼각형화 할 수 있는 모든 노드를 삼각형화 시킨다. 
	fbxsdk::FbxGeometryConverter geometryConverter(manager);
	geometryConverter.Triangulate(scene, true);

	// importer 파괴
	importer->Destroy();
}

void FBXParser::LoadMesh(fbxsdk::FbxMesh* mesh)
{
	std::shared_ptr<FBXMeshInfo> fbxMeshInfo = std::make_shared<FBXMeshInfo>();

	fbxModel->fbxMeshInfoList.push_back(fbxMeshInfo);

	std::shared_ptr<FBXMeshInfo>& meshInfo = fbxModel->fbxMeshInfoList.back();

	meshInfo->meshName = mesh->GetName();

	const int vertexCount = mesh->GetControlPointsCount();
	meshInfo->meshVertexList.resize(vertexCount);

	// 중복 체크를 위한 것
	std::vector<bool> isVertex(vertexCount, false);

	// Position정보를 가져옴(축 바꿔서 가져옴)
	FbxVector4* controlPoints = mesh->GetControlPoints();
	for (int i = 0; i < vertexCount; ++i)
	{
		meshInfo->meshVertexList[i].position.x = static_cast<float>(controlPoints[i].mData[0]);
		meshInfo->meshVertexList[i].position.y = static_cast<float>(controlPoints[i].mData[2]);
		meshInfo->meshVertexList[i].position.z = static_cast<float>(controlPoints[i].mData[1]);
	}

	FbxGeometryElementMaterial* geometryElementMaterial = mesh->GetElementMaterial();

	const int polygonSize = mesh->GetPolygonSize(0);

	assert(polygonSize == 3);		// 삼각형이 아니면 오류

	int arrIdx[3];
	int vertexCounter = 0;			// 정점의 개수

	const int triCount = mesh->GetPolygonCount(); // 메쉬의 삼각형 개수를 가져온다

	for (int i = 0; i < triCount; i++) // 삼각형의 개수
	{
		for (int j = 0; j < 3; j++) // 삼각형은 세 개의 정점으로 구성
		{
			int controlPointIndex = mesh->GetPolygonVertex(i, j); // 제어점의 인덱스 추출

			// 나왔었던 controlPointIndex 라면 새로운 버텍스 생성 및 controlPointIndex 값 바꿔주기 
			if (isVertex[controlPointIndex] == true)
			{
				Vertex vertex;
				vertex.position = meshInfo->meshVertexList[controlPointIndex].position;	// 포지션은 동일

				meshInfo->meshVertexList.push_back(vertex);								// 새로운 버텍스 삽입
				
				controlPointIndex = meshInfo->meshVertexList.size() - 1;				// index 새로운 버텍스 껄로 바꾸기
				
				isVertex.push_back(true);
			}

			arrIdx[j] = controlPointIndex;

			// uv 정보를 가져온다.
			GetUV(mesh, meshInfo, controlPointIndex, mesh->GetTextureUVIndex(i, j));
			
			// normal 정보를 가져온다.
			GetNormal(mesh, meshInfo, controlPointIndex, vertexCounter);

			// tangent 정보를 가져온다.
			GetTangent(mesh, meshInfo, controlPointIndex, vertexCounter);

			vertexCounter++;

			// 나왔었던 controlPointIndex면 true
			isVertex[controlPointIndex] = true;
		}

		meshInfo->indices.push_back(arrIdx[0]);
		meshInfo->indices.push_back(arrIdx[2]);
		meshInfo->indices.push_back(arrIdx[1]);
	}

	// Animation 데이터를 받아준다.
	//LoadAnimationData(mesh, &meshInfo);
}

void FBXParser::LoadMaterial(fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	std::shared_ptr<FBXMaterialInfo> material = std::make_shared<FBXMaterialInfo>();

	material->materialName = surfaceMaterial->GetName();

	material->albedoMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sDiffuse);
	material->normalMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sNormalMap);
	material->metallicMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sSpecular);
	material->roughnessMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sShininess);
	material->emissiveMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sEmissive);

	if (material->albedoMap != L"") { material->isAlbedo = true; }
	if (material->normalMap != L"") { material->isNormal = true; }
	if (material->roughnessMap != L"") { material->isRoughness = true; }
	if (material->emissiveMap != L"") { material->isEmissive = true; }

	// 메시에는 머터리얼 이름만
	fbxModel->fbxMeshInfoList.back()->materialName = surfaceMaterial->GetName();

	// 머터리얼 리스트에 추가
	fbxModel->materialList.push_back(material);
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

void FBXParser::GetTangent(fbxsdk::FbxMesh* mesh, std::shared_ptr<FBXMeshInfo>& meshInfo, int idx, int vertexCounter)
{
	// fbxModel에 tangent 정보가 없다면 계산해서 tangent 값을 넣어주자!
	if (mesh->GetElementTangentCount() == 0)
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

		return;
	}

	FbxGeometryElementTangent* tangent = mesh->GetElementTangent();
	int tangentIdx = 0;

	if (tangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (tangent->GetReferenceMode() == FbxGeometryElement::eDirect)
			tangentIdx = vertexCounter;
		else
			tangentIdx = tangent->GetIndexArray().GetAt(vertexCounter);
	}
	else if (tangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (tangent->GetReferenceMode() == FbxGeometryElement::eDirect)
			tangentIdx = idx;
		else
			tangentIdx = tangent->GetIndexArray().GetAt(idx);
	}

	FbxVector4 vec = tangent->GetDirectArray().GetAt(tangentIdx);
	meshInfo->meshVertexList[idx].tangent.x = static_cast<float>(vec.mData[0]);
	meshInfo->meshVertexList[idx].tangent.y = static_cast<float>(vec.mData[2]);
	meshInfo->meshVertexList[idx].tangent.z = static_cast<float>(vec.mData[1]);
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
