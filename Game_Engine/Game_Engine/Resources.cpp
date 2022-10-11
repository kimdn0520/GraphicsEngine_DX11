#include "pch.h"
#include "Resources.h"
#include "ASEParser.h"
#include "GraphicsManager.h"
#include "GameObject.h"
#include "MonoBehaviour.h"
#include "Transform.h"
#include "MeshRenderer.h"

#include "ParserBase.h"
#include "ParserData.h"

std::shared_ptr<Resources> Resources::resources = nullptr;

std::shared_ptr<Resources> Resources::Get()
{
	if (resources == nullptr)
		resources = std::make_shared<Resources>();

	return resources;
}

void Resources::Initialize()
{
	// ASEParser ��ü ����
	_aseParser = std::make_shared<ASEParser>();

	LoadCubeMesh(TOPOLOGY_TRIANGLELIST, RasterState_SOLID);

	LoadSphereMesh(TOPOLOGY_TRIANGLELIST, RasterState_SOLID);

	LoadAxisMesh(TOPOLOGY_LINELIST, RasterState_WIRE);

	LoadGridMesh(TOPOLOGY_LINELIST, RasterState_WIRE);

	LoadScreenMesh(TOPOLOGY_TRIANGLELIST, RasterState_SOLID);

	LoadDebugScreenMesh(TOPOLOGY_TRIANGLELIST, RasterState_SOLID);
}

void Resources::Release()
{
	// TODO : �� ���� ������.. Release�� �����ѱ�
	_aseParser->ResetData();

	_aseParser.reset();
}

void Resources::LoadCubeMesh(int topology, int rasterState)
{
	float x = 0.5f;
	float y = 0.5f;
	float z = 0.5f;

	vector<StaticMeshVertex> vertices(24);

	// �ո�
	vertices[0] = StaticMeshVertex(Vector3(-x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f));
	vertices[1] = StaticMeshVertex(Vector3(-x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
	vertices[2] = StaticMeshVertex(Vector3(+x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
	vertices[3] = StaticMeshVertex(Vector3(+x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f));
	// �޸�
	vertices[4] = StaticMeshVertex(Vector3(-x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));
	vertices[5] = StaticMeshVertex(Vector3(+x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));
	vertices[6] = StaticMeshVertex(Vector3(+x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	vertices[7] = StaticMeshVertex(Vector3(-x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	// ����
	vertices[8] = StaticMeshVertex(Vector3(-x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));
	vertices[9] = StaticMeshVertex(Vector3(-x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	vertices[10] = StaticMeshVertex(Vector3(+x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	vertices[11] = StaticMeshVertex(Vector3(+x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));
	// �Ʒ���
	vertices[12] = StaticMeshVertex(Vector3(-x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f));
	vertices[13] = StaticMeshVertex(Vector3(+x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f));
	vertices[14] = StaticMeshVertex(Vector3(+x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
	vertices[15] = StaticMeshVertex(Vector3(-x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
	// ���ʸ�		
	vertices[16] = StaticMeshVertex(Vector3(-x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vertices[17] = StaticMeshVertex(Vector3(-x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vertices[18] = StaticMeshVertex(Vector3(-x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vertices[19] = StaticMeshVertex(Vector3(-x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f));
	// �����ʸ�		
	vertices[20] = StaticMeshVertex(Vector3(+x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f));
	vertices[21] = StaticMeshVertex(Vector3(+x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
	vertices[22] = StaticMeshVertex(Vector3(+x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
	vertices[23] = StaticMeshVertex(Vector3(+x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f));

	vector<unsigned int> idx(36);

	// �ո�
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// �޸�
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// ����
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// �Ʒ���
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// ���ʸ�
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// �����ʸ�
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	GraphicsManager::Get()->CreateMesh(
		vertices,
		idx,
		topology,
		rasterState);
}

void Resources::LoadSphereMesh(int topology, int rasterState)
{
	float radius = 0.5f;		// ���� ������
	int stackCount = 20;		// ���� ����
	int sliceCount = 20;		// ���� ����

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	vector<StaticMeshVertex> vertices;

	// �ϱ�
	vertices.push_back(Vector3(0.0f, radius, 0.0f));

	// ������ ���鼭 ������ ����Ѵ� (�ϱ�/���� �������� ���� X)
	for (int y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// ���� ��ġ�� ����
		for (int x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			Vector3 pos;
			pos.x = radius * sinf(phi) * cosf(theta);
			pos.y = radius * cosf(phi);
			pos.z = radius * sinf(phi) * sinf(theta);

			vertices.push_back(pos);
		}
	}

	// ����
	vertices.push_back(Vector3(0.0f, -radius, 0.0f));

	vector<unsigned int> idx;

	// �ϱ� �ε���
	for (int i = 0; i <= sliceCount; ++i)
	{
		idx.push_back(0);
		idx.push_back(i + 2);
		idx.push_back(i + 1);
	}

	// ���� �ε���
	int ringVertexCount = sliceCount + 1;
	for (int y = 0; y < stackCount - 2; ++y)
	{
		for (int x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx.push_back(1 + (y)*ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// ���� �ε���
	int bottomIndex = static_cast<int>(vertices.size()) - 1;
	int lastRingStartIndex = bottomIndex - ringVertexCount;
	for (int i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx.push_back(bottomIndex);
		idx.push_back(lastRingStartIndex + i);
		idx.push_back(lastRingStartIndex + i + 1);
	}

	GraphicsManager::Get()->CreateMesh(
		vertices,
		idx,
		topology,
		rasterState);
}

void Resources::LoadAxisMesh(int topology, int rasterState)
{
	vector<StaticMeshVertex> vertices;
	vertices.push_back(StaticMeshVertex(Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f)));
	vertices.push_back(StaticMeshVertex(Vector3(50.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f)));
	vertices.push_back(StaticMeshVertex(Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f)));
	vertices.push_back(StaticMeshVertex(Vector3(0.0f, 50.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f)));
	vertices.push_back(StaticMeshVertex(Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f)));
	vertices.push_back(StaticMeshVertex(Vector3(0.0f, 0.0f, 50.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f)));

	vector<unsigned int> idx;
	idx.push_back(0); idx.push_back(1);
	idx.push_back(2); idx.push_back(3);
	idx.push_back(4); idx.push_back(5);

	GraphicsManager::Get()->CreateMesh(
		vertices,
		idx,
		topology,
		rasterState);
}

void Resources::LoadGridMesh(int topology, int rasterState)
{
	std::vector<StaticMeshVertex> vertices(100);

	Vector4 _color = Vector4(1.f, 1.f, 1.f, 1.f);

	for (int i = 0; i < 100; i++)
	{
		vertices[i].position = Vector3((float)(i % 10) - 5.0f, 0.0f, (float)(i / 10) - 5.0f);
		vertices[i].color = Vector4(_color.x, _color.y, _color.z, _color.w);	
	}

	std::vector<unsigned int> indices(40);

	for (int i = 0; i < 10; i++)
	{
		indices[i * 2] = i;
		indices[i * 2 + 1] = i + 90;
	}

	for (int i = 0; i < 10; i++)
	{
		indices[20 + (i * 2)] = i * 10;
		indices[20 + (i * 2) + 1] = i * 10 + 9;
	}

	GraphicsManager::Get()->CreateMesh(
		vertices,
		indices,
		topology,
		rasterState);
}

void Resources::LoadScreenMesh(int topology, int rasterState)
{
	std::vector<StaticMeshVertex> vertices(6);

	// �� �ϴ�
	vertices[0].position = Vector3(-1.f, -1.f, 0.f);
	vertices[0].uv = Vector2(0.f, 1.f);

	// �� ���
	vertices[1].position = Vector3(-1.f, 1.f, 0.f);
	vertices[1].uv = Vector2(0.f, 0.f);

	// �� ���
	vertices[2].position = Vector3(1.f, 1.f, 0.f);
	vertices[2].uv = Vector2(1.f, 0.f);

	// �� �ϴ�
	vertices[3].position = Vector3(-1.f, -1.f, 0.f);
	vertices[3].uv = Vector2(0.f, 1.f);

	// �� ���
	vertices[4].position = Vector3(1.f, 1.f, 0.f);
	vertices[4].uv = Vector2(1.f, 0.f);

	// �� �ϴ�
	vertices[5].position = Vector3(1.f, -1.f, 0.f);
	vertices[5].uv = Vector2(1.f, 1.f);

	std::vector<unsigned int> indices(6);

	for (int i = 0; i < 6; i++)
		indices[i] = i;

	GraphicsManager::Get()->CreateMesh(
		vertices,
		indices,
		topology,
		rasterState);
}

void Resources::LoadDebugScreenMesh(int topology, int rasterState)
{
	std::vector<StaticMeshVertex> vertices(6);

	// �� �ϴ�
	vertices[0].position = Vector3(-1.f, 0.6f, 0.f);
	vertices[0].uv = Vector2(0.f, 1.f);

	// �� ���
	vertices[1].position = Vector3(-1.f, 1.f, 0.f);
	vertices[1].uv = Vector2(0.f, 0.f);

	// �� ���
	vertices[2].position = Vector3(1.f, 1.f, 0.f);
	vertices[2].uv = Vector2(1.f, 0.f);

	// �� �ϴ�
	vertices[3].position = Vector3(-1.f, 0.6f, 0.f);
	vertices[3].uv = Vector2(0.f, 1.f);

	// �� ���
	vertices[4].position = Vector3(1.f, 1.f, 0.f);
	vertices[4].uv = Vector2(1.f, 0.f);

	// �� �ϴ�
	vertices[5].position = Vector3(1.f, 0.6f, 0.f);
	vertices[5].uv = Vector2(1.f, 1.f);

	std::vector<unsigned int> indices(6);

	for (int i = 0; i < 6; i++)
		indices[i] = i;

	GraphicsManager::Get()->CreateMesh(
		vertices,
		indices,
		topology,
		rasterState);
}

std::vector<std::shared_ptr<GameObject>> Resources::LoadASE(std::string path, int topology, int rasterizerState)
{
	// ���� �ε��Ѵ�.
	std::shared_ptr<ASEData::ASEModel> aseModel = _aseParser->Load(path);

	// ���ӿ�����Ʈ��� �ٲپ ��ȯ�Ұ���.
	vector<std::shared_ptr<GameObject>> gameObjects;

	// Animation�� �ִ� ��� ��κ� SkinnedMesh
	if(aseModel->isAnimation)
	{

	}
	// Animation�� ���� ��� StaticMesh
	else
	{
		for (auto& mesh : aseModel->meshInfos)
		{
			std::vector<StaticMeshVertex> staticMeshVertices;

			for (auto& vertex : mesh->vertices)
			{
				StaticMeshVertex staticMeshVertex;
				staticMeshVertex.position = vertex.pos;
				staticMeshVertex.color = vertex.color;
				staticMeshVertex.uv = vertex.uv;
				staticMeshVertex.normal = vertex.normal;
				staticMeshVertex.tangent = vertex.tangent;

				staticMeshVertices.push_back(staticMeshVertex);
			}

			// �ش� �޽��� vertices, indices �׷��Ƚ� ResourceManager����ϰ� �ش� meshID�� �����´�.
			size_t meshID = GraphicsManager::Get()->CreateMesh(
				staticMeshVertices,
				mesh->indices,
				topology,
				rasterizerState);

			std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();
			gameObject->SetName(mesh->meshName);
			gameObject->AddComponent<Transform>();
			gameObject->GetComponent<Transform>()->SetNodeTM(mesh->nodeTM);
			gameObject->AddComponent<MeshRenderer>();
			gameObject->GetComponent<MeshRenderer>()->SetMeshID(meshID);		// meshID ���

			for (auto& mat : aseModel->materials)
			{
				std::shared_ptr<Material> material = std::make_shared<Material>();
				material->metallic = 0.1f;
				material->roughness = 0.0f;
				material->albedoMap = mat->diffuseTexName;
				material->normalMap = mat->normalTexName;
				material->isAlbedo = mat->isDiffuse;
				material->isNormal = mat->isNormal;
				
				gameObject->GetComponent<MeshRenderer>()->SetMaterial(material); 
			}

			Vector3 localScale = { 1.0f, 1.0f, 1.0f };
			Vector3 localRotation = { 0.f, 0.f, 0.f };
			Vector3 localTranslation = { 0.f, .0f, 0.f };

			if (mesh->parentName != "")
			{
				// ��.. �θ� ���� �������
				for (auto& gameObj : gameObjects)
				{
					if (gameObj->GetName() == mesh->parentName)
					{
						gameObject->GetTransform()->SetParent(gameObj->GetTransform());
						gameObj->SetChild(gameObject);
						XMMATRIX parentMatrixInverse = XMMatrixInverse(nullptr, gameObj->GetComponent<Transform>()->GetWorldMatrix());
						XMMATRIX multipleMatrix = XMMatrixMultiply(gameObject->GetTransform()->GetNodeMatrix(), parentMatrixInverse);

						Transform::DecomposeMatrix(multipleMatrix, localScale, localRotation, localTranslation);
						break;
					}
				}
			}
			else
			{
				Transform::DecomposeMatrix(gameObject->GetTransform()->GetNodeMatrix(), localScale, localRotation, localTranslation);
			}

			gameObject->GetComponent<Transform>()->SetLocalScale(localScale);
			gameObject->GetComponent<Transform>()->SetLocalRotation(localRotation);
			gameObject->GetComponent<Transform>()->SetLocalPosition(localTranslation);
			gameObject->GetComponent<Transform>()->SetOriScale(localScale);
			gameObject->GetComponent<Transform>()->SetOriRot(localRotation);
			gameObject->GetComponent<Transform>()->SetOriTrans(localTranslation);
			gameObject->GetComponent<Transform>()->FixedUpdate();

			gameObjects.push_back(gameObject);
		}		
	}

	return gameObjects;
}

std::vector<std::shared_ptr<GameObject>> Resources::LoadFBX(std::string path, int topology, int rasterizerState)
{
	// fbx �� �غ�..
	shared_ptr<ParserBase> fbxLoader = ParserBase::Create();
	
	// fbx �� �ε�
	shared_ptr<FBXModel> fbxModel = fbxLoader->LoadFbx(path);

	// ���ӿ�����Ʈ��� �ٲپ ��ȯ�Ұ���.
	vector<std::shared_ptr<GameObject>> gameObjects;

	// Animation�� �ִ� ��� ��κ� SkinnedMesh
	if (fbxModel->isSkinnedAnimation)
	{

	}
	// Animation�� ���� ��� StaticMesh
	else
	{
		for (auto& mesh : fbxModel->fbxMeshInfoList)
		{
			std::vector<StaticMeshVertex> staticMeshVertices;

			for (auto& vertex : mesh->meshVertexList)
			{
				StaticMeshVertex staticMeshVertex;
				staticMeshVertex.position = vertex.position;
				staticMeshVertex.color = vertex.color;
				staticMeshVertex.uv = vertex.uv;
				staticMeshVertex.normal = vertex.normal;
				staticMeshVertex.tangent = vertex.tangent;

				staticMeshVertices.push_back(staticMeshVertex);
			}

			// �ش� �޽��� vertices, indices �׷��Ƚ� ResourceManager����ϰ� �ش� meshID�� �����´�.
			size_t meshID = GraphicsManager::Get()->CreateMesh(
				staticMeshVertices,
				mesh->indices,
				topology,
				rasterizerState);

			std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();
			gameObject->SetName(mesh->meshName);
			gameObject->AddComponent<Transform>();
			gameObject->AddComponent<MeshRenderer>();
			gameObject->GetComponent<MeshRenderer>()->SetMeshID(meshID);		// meshID ���

			// TODO : fbxModel�� �ִ� ���͸������ ���� ������ ��
			// �׸��� �̸�? ������ �˰��ϰ� ���߿� ���ҽ��Ŵ����� ���͸��� ��Ƴ���������
			// ���͸����� �����ͼ� cbMaterial�� ������ �ְ� shader�� ������ �ڵ带 �ۼ�����

			// �޽� �ϳ��� ���͸��� �ϳ�
			for (auto& mat : fbxModel->materialList)
			{
				if (mesh->materialName == mat->materialName)
				{
					std::shared_ptr<Material> material = std::make_shared<Material>();

					material->name = mat->materialName;

					material->metallic = 0.1f;
					material->roughness = 0.0f;

					material->albedoMap = mat->albedoMap;
					material->normalMap = mat->normalMap;
					material->metallicMap = mat->metallicMap;
					material->roughnessMap = mat->roughnessMap;
					material->AOMap = mat->AOMap;
					material->emissiveMap = mat->emissiveMap;

					material->isAlbedo = mat->isAlbedo;
					material->isNormal = mat->isNormal;
					material->isMetallic = mat->isMetallic;
					material->isRoughness = mat->isRoughness;
					material->isAO = mat->isAO;
					material->isEmissive = mat->isEmissive;

					gameObject->GetComponent<MeshRenderer>()->SetMaterial(material);

					break;
				}
			}

			gameObjects.push_back(gameObject);
		}
	}

	return gameObjects;
}

