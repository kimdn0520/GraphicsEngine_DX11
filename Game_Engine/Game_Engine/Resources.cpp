#include "pch.h"
#include "Resources.h"
#include "ASEParser.h"
#include "GraphicsManager.h"
#include "GameObject.h"
#include "MonoBehaviour.h"
#include "Transform.h"
#include "MeshRenderer.h"

Resources* Resources::resources = nullptr;

Resources* Resources::Get()
{
	if (resources == nullptr)
		resources = new Resources();

	return resources;
}

void Resources::Initialize()
{
	// ASEParser ��ü ����
	_aseParser = new ASEParser();

	LoadCubeMesh(TOPOLOGY_TRIANGLELIST, RasterState_SOLID);
}

void Resources::Release()
{
	// TODO : �� ���� ������.. Release�� �����ѱ�
	_aseParser->ResetData();

	delete _aseParser;
}

void Resources::LoadCubeMesh(int topology, int rasterState)
{
	float x = 0.5f;
	float y = 0.5f;
	float z = 0.5f;

	vector<StaticMeshVertex> vertices(24);

	// �ո�
	vertices[0] = StaticMeshVertex(Vector3(-x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));
	vertices[1] = StaticMeshVertex(Vector3(-x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	vertices[2] = StaticMeshVertex(Vector3(+x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	vertices[3] = StaticMeshVertex(Vector3(+x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));
	// �޸�
	vertices[4] = StaticMeshVertex(Vector3(-x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f));
	vertices[5] = StaticMeshVertex(Vector3(+x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f));
	vertices[6] = StaticMeshVertex(Vector3(+x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
	vertices[7] = StaticMeshVertex(Vector3(-x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
	// ����
	vertices[8] = StaticMeshVertex(Vector3(-x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f));
	vertices[9] = StaticMeshVertex(Vector3(-x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
	vertices[10] = StaticMeshVertex(Vector3(+x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
	vertices[11] = StaticMeshVertex(Vector3(+x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f));
	// �Ʒ���
	vertices[12] = StaticMeshVertex(Vector3(-x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));
	vertices[13] = StaticMeshVertex(Vector3(+x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));
	vertices[14] = StaticMeshVertex(Vector3(+x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	vertices[15] = StaticMeshVertex(Vector3(-x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	// ���ʸ�		
	vertices[16] = StaticMeshVertex(Vector3(-x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f));
	vertices[17] = StaticMeshVertex(Vector3(-x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
	vertices[18] = StaticMeshVertex(Vector3(-x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
	vertices[19] = StaticMeshVertex(Vector3(-x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f));
	// �����ʸ�		
	vertices[20] = StaticMeshVertex(Vector3(+x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vertices[21] = StaticMeshVertex(Vector3(+x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vertices[22] = StaticMeshVertex(Vector3(+x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vertices[23] = StaticMeshVertex(Vector3(+x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f));

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

	// �������� ���鼭 ������ ����Ѵ� (�ϱ�/���� �������� ������ X)
	for (int y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// ������ ��ġ�� ����
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
	vertices.push_back(StaticMeshVertex(Vector3(20.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f)));
	vertices.push_back(StaticMeshVertex(Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f)));
	vertices.push_back(StaticMeshVertex(Vector3(0.0f, 20.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f)));
	vertices.push_back(StaticMeshVertex(Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f)));
	vertices.push_back(StaticMeshVertex(Vector3(0.0f, 0.0f, 20.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f)));

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

}

void Resources::LoadScreenMesh(int topology, int rasterState)
{

}

std::vector<GameObject*> Resources::LoadASE(std::string path, int topology, int rasterizerState)
{
	// ���� �ε��Ѵ�.
	ASEData::ASEModel* aseModel = _aseParser->Load(path);

	// ���ӿ�����Ʈ��� �ٲپ ��ȯ�Ұ���.
	vector<GameObject*> gameObjects;

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
				staticMeshVertex.color = ::Color::White;
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

			GameObject* gameObject = new GameObject();
			gameObject->SetName(mesh->meshName);
			gameObject->AddComponent<Transform>();
			gameObject->GetComponent<Transform>()->SetNodeTM(mesh->nodeTM);
			gameObject->AddComponent<MeshRenderer>();
			gameObject->GetComponent<MeshRenderer>()->SetMeshID(meshID);		// meshID ���

			for (auto& mat : aseModel->materials)
			{
				Material* material = new Material();
				material->ambient = mat->ambient;
				material->diffuse = mat->diffuse;
				material->specular = mat->specular;
				material->diffuseTexture = mat->diffuseTexName;
				material->normalTexture = mat->normalTexName;
				material->specularTexture = mat->specularTexName;
				material->isDiffuse = mat->isDiffuse;
				material->isNormal = mat->isNormal;
				material->isSpecular = mat->isSpecular;
				
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
