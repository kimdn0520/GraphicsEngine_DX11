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
		ParseNode(node->GetChild(i));
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
	scene->GetGlobalSettings().SetAxisSystem(fbxsdk::FbxAxisSystem::DirectX);

	// �� ������ �ﰢ��ȭ �� �� �ִ� ��� ��带 �ﰢ��ȭ ��Ų��. 
	fbxsdk::FbxGeometryConverter geometryConverter(manager);
	geometryConverter.Triangulate(scene, true);

	// importer �ı�
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

	// �ߺ� üũ�� ���� ��
	std::vector<bool> isVertex(vertexCount, false);

	// Position������ ������(�� �ٲ㼭 ������)
	FbxVector4* controlPoints = mesh->GetControlPoints();
	for (int i = 0; i < vertexCount; ++i)
	{
		meshInfo->meshVertexList[i].position.x = static_cast<float>(controlPoints[i].mData[0]);
		meshInfo->meshVertexList[i].position.y = static_cast<float>(controlPoints[i].mData[2]);
		meshInfo->meshVertexList[i].position.z = static_cast<float>(controlPoints[i].mData[1]);
	}

	FbxGeometryElementMaterial* geometryElementMaterial = mesh->GetElementMaterial();

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

				meshInfo->meshVertexList.push_back(vertex);								// ���ο� ���ؽ� ����
				
				controlPointIndex = meshInfo->meshVertexList.size() - 1;				// index ���ο� ���ؽ� ���� �ٲٱ�
				
				isVertex.push_back(true);
			}

			arrIdx[j] = controlPointIndex;

			// uv ������ �����´�.
			GetUV(mesh, meshInfo, controlPointIndex, mesh->GetTextureUVIndex(i, j));
			
			// normal ������ �����´�.
			GetNormal(mesh, meshInfo, controlPointIndex, vertexCounter);

			// tangent ������ �����´�.
			GetTangent(mesh, meshInfo, controlPointIndex, vertexCounter);

			vertexCounter++;

			// ���Ծ��� controlPointIndex�� true
			isVertex[controlPointIndex] = true;
		}

		meshInfo->indices.push_back(arrIdx[0]);
		meshInfo->indices.push_back(arrIdx[2]);
		meshInfo->indices.push_back(arrIdx[1]);
	}

	// Animation �����͸� �޾��ش�.
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

	// �޽ÿ��� ���͸��� �̸���
	fbxModel->fbxMeshInfoList.back()->materialName = surfaceMaterial->GetName();

	// ���͸��� ����Ʈ�� �߰�
	fbxModel->materialList.push_back(material);
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

void FBXParser::GetTangent(fbxsdk::FbxMesh* mesh, std::shared_ptr<FBXMeshInfo>& meshInfo, int idx, int vertexCounter)
{
	// fbxModel�� tangent ������ ���ٸ� ����ؼ� tangent ���� �־�����!
	if (mesh->GetElementTangentCount() == 0)
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
