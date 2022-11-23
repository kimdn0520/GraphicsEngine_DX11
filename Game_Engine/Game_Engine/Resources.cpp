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
#include "Animator.h"
#include "StructDefine.h"
#include "..\FBXParser\FBXParser\FBXBinaryLayout.h"

std::shared_ptr<Resources> Resources::resources = nullptr;

std::shared_ptr<Resources> Resources::Get()
{
	if (resources == nullptr)
		resources = std::make_shared<Resources>();

	return resources;
}

void Resources::Initialize()
{
	// ASEParser 객체 생성
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
	// TODO : 음 좀더 봐야함.. Release를 만들어둘까
	_aseParser->ResetData();

	_aseParser.reset();
}

void Resources::LoadCubeMesh(int topology, int rasterState)
{
	float x = 0.5f;
	float y = 0.5f;
	float z = 0.5f;

	vector<StaticMeshVertex> vertices(24);

	// 앞면
	vertices[0] = StaticMeshVertex(Vector3(-x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f));
	vertices[1] = StaticMeshVertex(Vector3(-x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
	vertices[2] = StaticMeshVertex(Vector3(+x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
	vertices[3] = StaticMeshVertex(Vector3(+x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f));
	// 뒷면
	vertices[4] = StaticMeshVertex(Vector3(-x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));
	vertices[5] = StaticMeshVertex(Vector3(+x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));
	vertices[6] = StaticMeshVertex(Vector3(+x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	vertices[7] = StaticMeshVertex(Vector3(-x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	// 윗면
	vertices[8] = StaticMeshVertex(Vector3(-x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));
	vertices[9] = StaticMeshVertex(Vector3(-x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	vertices[10] = StaticMeshVertex(Vector3(+x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	vertices[11] = StaticMeshVertex(Vector3(+x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));
	// 아랫면
	vertices[12] = StaticMeshVertex(Vector3(-x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f));
	vertices[13] = StaticMeshVertex(Vector3(+x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f));
	vertices[14] = StaticMeshVertex(Vector3(+x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
	vertices[15] = StaticMeshVertex(Vector3(-x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
	// 왼쪽면		
	vertices[16] = StaticMeshVertex(Vector3(-x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vertices[17] = StaticMeshVertex(Vector3(-x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vertices[18] = StaticMeshVertex(Vector3(-x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vertices[19] = StaticMeshVertex(Vector3(-x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f));
	// 오른쪽면		
	vertices[20] = StaticMeshVertex(Vector3(+x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f));
	vertices[21] = StaticMeshVertex(Vector3(+x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
	vertices[22] = StaticMeshVertex(Vector3(+x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
	vertices[23] = StaticMeshVertex(Vector3(+x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f));

	vector<unsigned int> idx(36);

	// 앞면
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// 뒷면
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// 윗면
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// 아랫면
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// 왼쪽면
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// 오른쪽면
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
	float radius = 0.5f;		// 구의 반지름
	int stackCount = 20;		// 가로 분할
	int sliceCount = 20;		// 세로 분할

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	vector<StaticMeshVertex> vertices;

	// 북극
	vertices.push_back(Vector3(0.0f, radius, 0.0f));

	// 고리마다 돌면서 정점을 계산한다 (북극/남극 단일점은 고리가 X)
	for (int y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// 고리에 위치한 정점
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

	// 남극
	vertices.push_back(Vector3(0.0f, -radius, 0.0f));

	vector<unsigned int> idx;

	// 북극 인덱스
	for (int i = 0; i <= sliceCount; ++i)
	{
		idx.push_back(0);
		idx.push_back(i + 2);
		idx.push_back(i + 1);
	}

	// 몸통 인덱스
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

	// 남극 인덱스
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

	// 좌 하단
	vertices[0].position = Vector3(-1.f, -1.f, 0.f);
	vertices[0].uv = Vector2(0.f, 1.f);

	// 좌 상단
	vertices[1].position = Vector3(-1.f, 1.f, 0.f);
	vertices[1].uv = Vector2(0.f, 0.f);

	// 우 상단
	vertices[2].position = Vector3(1.f, 1.f, 0.f);
	vertices[2].uv = Vector2(1.f, 0.f);

	// 좌 하단
	vertices[3].position = Vector3(-1.f, -1.f, 0.f);
	vertices[3].uv = Vector2(0.f, 1.f);

	// 우 상단
	vertices[4].position = Vector3(1.f, 1.f, 0.f);
	vertices[4].uv = Vector2(1.f, 0.f);

	// 우 하단
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

	// 좌 하단
	vertices[0].position = Vector3(-1.f, 0.6f, 0.f);
	vertices[0].uv = Vector2(0.f, 1.f);

	// 좌 상단
	vertices[1].position = Vector3(-1.f, 1.f, 0.f);
	vertices[1].uv = Vector2(0.f, 0.f);

	// 우 상단
	vertices[2].position = Vector3(1.f, 1.f, 0.f);
	vertices[2].uv = Vector2(1.f, 0.f);

	// 좌 하단
	vertices[3].position = Vector3(-1.f, 0.6f, 0.f);
	vertices[3].uv = Vector2(0.f, 1.f);

	// 우 상단
	vertices[4].position = Vector3(1.f, 1.f, 0.f);
	vertices[4].uv = Vector2(1.f, 0.f);

	// 우 하단
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
	// 모델을 로드한다.
	std::shared_ptr<ASEData::ASEModel> aseModel = _aseParser->Load(path);

	// 게임오브젝트들로 바꾸어서 반환할거임.
	vector<std::shared_ptr<GameObject>> gameObjects;

	// Animation이 있는 경우 대부분 SkinnedMesh
	if(aseModel->isAnimation)
	{

	}
	// Animation이 없는 경우 StaticMesh
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

			// 해당 메시의 vertices, indices 그래픽스 ResourceManager에등록과 해당 meshID를 가져온다.
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
			gameObject->GetComponent<MeshRenderer>()->SetMeshID(meshID);		// meshID 등록

			for (auto& mat : aseModel->materials)
			{
				std::shared_ptr<Material> material = std::make_shared<Material>();
				material->name = material->name;
				material->metallic = 0.1f;
				material->roughness = 0.0f;
				material->albedoMap = mat->diffuseTexName;
				material->normalMap = mat->normalTexName;

				GraphicsManager::Get()->SendMaterialData(material);
				
				gameObject->GetComponent<MeshRenderer>()->SetMaterial(material->name); 
			}

			Vector3 localScale = { 1.0f, 1.0f, 1.0f };
			Vector3 localRotation = { 0.f, 0.f, 0.f };
			Vector3 localTranslation = { 0.f, .0f, 0.f };

			if (mesh->parentName != "")
			{
				// 움.. 부모가 먼저 생기긴해
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
	// fbx 모델 준비..
	shared_ptr<ParserBase> fbxLoader = ParserBase::Create();
	
	// fbx 모델 로드
	shared_ptr<FBXModel> fbxModel = fbxLoader->LoadFbx(path);

	// Map에 저장
	unordered_map<std::string ,std::shared_ptr<GameObject>> boneMap;

	// 게임오브젝트들로 바꾸어서 반환할거임.
	vector<std::shared_ptr<GameObject>> gameObjects;

	// Animation이 있는 경우 대부분 SkinnedMesh
	if (fbxModel->isSkinnedAnimation)
	{
		// 본오브젝트를 만들자..
		for (int boneIdx = 0; boneIdx < fbxModel->fbxBoneInfoList.size(); boneIdx++)
		{
			std::shared_ptr<GameObject> boneObject = std::make_shared<GameObject>();
			boneObject->SetName(fbxModel->fbxBoneInfoList[boneIdx]->boneName);
			boneObject->AddComponent<Transform>();
			boneObject->GetComponent<Transform>()->SetBoneOffsetMatrix(fbxModel->fbxBoneInfoList[boneIdx]->offsetMatrix);
			boneObject->GetComponent<Transform>()->SetNodeTM(fbxModel->fbxBoneInfoList[boneIdx]->nodeMatrix);
			boneObject->AddComponent<MeshRenderer>();
			boneObject->GetComponent<MeshRenderer>()->SetMeshID(0);				// cube는 0
			shared_ptr<Material> boneMaterial = make_shared<Material>();
			boneMaterial->name = "BoneMaterial";
			boneMaterial->metallic = 0.0f;
			boneMaterial->roughness = 0.0f;
			GraphicsManager::Get()->SendMaterialData(boneMaterial);
			boneObject->GetComponent<MeshRenderer>()->SetMaterial(boneMaterial->name);
			boneObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_PS");

			Vector3 localScale = { 1.0f, 1.0f, 1.0f };
			Vector3 localRotation = { 0.f, 0.f, 0.f };
			Vector3 localTranslation = { 0.f, .0f, 0.f };

			if (!gameObjects.empty())
			{
				boneObject->GetTransform()->SetParent(gameObjects[fbxModel->fbxBoneInfoList[boneIdx]->parentIndex]->GetTransform());
				gameObjects[fbxModel->fbxBoneInfoList[boneIdx]->parentIndex]->SetChild(boneObject);

				XMMATRIX parentMatrixInverse = XMMatrixInverse(nullptr, gameObjects[fbxModel->fbxBoneInfoList[boneIdx]->parentIndex]->GetTransform()->GetWorldMatrix());
				XMMATRIX multipleMatrix = XMMatrixMultiply(boneObject->GetTransform()->GetNodeMatrix(), parentMatrixInverse);

				Transform::DecomposeMatrix(multipleMatrix, localScale, localRotation, localTranslation);
			}
			else
			{
				Transform::DecomposeMatrix(boneObject->GetTransform()->GetNodeMatrix(), localScale, localRotation, localTranslation);
			}
			boneObject->GetComponent<Transform>()->SetLocalScale(localScale);
			boneObject->GetComponent<Transform>()->SetLocalRotation(localRotation);
			boneObject->GetComponent<Transform>()->SetLocalPosition(localTranslation);
			boneObject->GetComponent<Transform>()->FixedUpdate();

			// 일단 모델의 애니메이션 클립 리스트를 돈다
			for (auto& anim : fbxModel->animationClipList)
			{
				std::shared_ptr<AnimationClip> animationClip = std::make_shared<AnimationClip>();
				animationClip->animationName = anim->animationName;
				animationClip->frameRate = anim->frameRate;
				animationClip->startKeyFrame = anim->startKeyFrame;
				animationClip->endKeyFrame = anim->endKeyFrame;
				animationClip->totalKeyFrame = anim->totalKeyFrame;
				animationClip->tickPerFrame = anim->tickPerFrame;

				// 애니메이션의 키프레임 리스트에서 boneIdx에 해당하는게 있으면 Animator 생성해주고 정보를 넣어준다.
				if (!anim->keyFrameList[boneIdx].empty())
				{
					if(boneObject->GetComponent<Animator>() == nullptr)
						boneObject->AddComponent<Animator>();
					
					for (int keyIdx = 0; keyIdx < anim->keyFrameList[boneIdx].size(); keyIdx++)
					{
						std::shared_ptr<AnimKeyFrame> animKeyFrame = std::make_shared<AnimKeyFrame>();
						animKeyFrame->time = anim->keyFrameList[boneIdx][keyIdx]->time;
						animKeyFrame->localTransform = anim->keyFrameList[boneIdx][keyIdx]->localTransform;
						animKeyFrame->localRotation = anim->keyFrameList[boneIdx][keyIdx]->localRotation;
						animKeyFrame->localScale = anim->keyFrameList[boneIdx][keyIdx]->localScale;

						animationClip->keyFrame.push_back(animKeyFrame);
					}

					boneObject->GetComponent<Animator>()->SetAnimClip(anim->animationName, animationClip);
				}
			}

			boneMap.insert(make_pair(fbxModel->fbxBoneInfoList[boneIdx]->boneName, boneObject));

			gameObjects.push_back(boneObject);
		}

		for (auto& mesh : fbxModel->fbxMeshInfoList)
		{
			std::vector<SkinnedMeshVertex> skinnedMeshVertices;

			for (auto& vertex : mesh->meshVertexList)
			{
				SkinnedMeshVertex skinnedMeshVertex;
				skinnedMeshVertex.position = vertex.position;
				skinnedMeshVertex.color = vertex.color;
				skinnedMeshVertex.uv = vertex.uv;
				skinnedMeshVertex.normal = vertex.normal;
				skinnedMeshVertex.tangent = vertex.tangent;

				for (int weightCnt = 0; weightCnt < 8; weightCnt++)
				{
					skinnedMeshVertex.weights[weightCnt] = vertex.weights[weightCnt];
					skinnedMeshVertex.boneIndices[weightCnt] = vertex.boneIndices[weightCnt];
				}

				skinnedMeshVertices.push_back(skinnedMeshVertex);
			}

			// 해당 메시의 vertices, indices 그래픽스 ResourceManager에등록과 해당 meshID를 가져온다.
			size_t meshID = GraphicsManager::Get()->CreateMesh(
				skinnedMeshVertices,
				mesh->indices,
				topology,
				rasterizerState);

			std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();
			gameObject->SetName(mesh->meshName);
			gameObject->AddComponent<Transform>();
			gameObject->GetComponent<Transform>()->SetNodeTM(mesh->nodeTM);		// NodeTM 넣기
			gameObject->AddComponent<MeshRenderer>();
			gameObject->GetComponent<MeshRenderer>()->IsSkinnedMesh(mesh->isSkinned);
			gameObject->GetComponent<MeshRenderer>()->SetMeshID(meshID);		// meshID 등록

			Vector3 localScale = { 1.0f, 1.0f, 1.0f };
			Vector3 localRotation = { 0.f, 0.f, 0.f };
			Vector3 localTranslation = { 0.f, .0f, 0.f };
			Transform::DecomposeMatrix(gameObject->GetTransform()->GetNodeMatrix(), localScale, localRotation, localTranslation);
			gameObject->GetComponent<Transform>()->SetLocalScale(localScale);
			gameObject->GetComponent<Transform>()->SetLocalRotation(localRotation);
			gameObject->GetComponent<Transform>()->SetLocalPosition(localTranslation);
			gameObject->GetComponent<Transform>()->FixedUpdate();

			// Bone에 의해 영향을 받는 Mesh라면
			if (mesh->isSkinned == true)
			{
				for (auto& bone : fbxModel->fbxBoneInfoList)
				{
					gameObject->GetComponent<MeshRenderer>()->SetBoneObject(boneMap[bone->boneName]);
				}
			}

			// 메시 하나당 머터리얼 하나
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

					material->material_Ambient = mat->material_Ambient;
					material->material_Diffuse = mat->material_Diffuse;
					material->material_Specular = mat->material_Specular;
					material->material_Emissive = mat->material_Emissive;

					material->emissionFactor = mat->emissionFactor;
					material->material_Transparency = mat->material_Transparency;
					material->material_Reflectivity = mat->material_Reflectivity;

					if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap != L"" && material->roughnessMap != L""
						&& material->AOMap != L"" && material->emissiveMap != L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CNMRAE_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap != L"" && material->roughnessMap != L""
						&& material->AOMap != L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CNMRA_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap == L"" && material->roughnessMap != L""
						&& material->AOMap == L"" && material->emissiveMap != L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CNRE_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap == L"" && material->roughnessMap == L""
						&& material->AOMap != L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CNA_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap != L"" && material->roughnessMap != L""
						&& material->AOMap == L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CNMR_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap == L"" && material->metallicMap != L"" && material->roughnessMap != L""
						&& material->AOMap == L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CMR_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap == L"" && material->roughnessMap == L""
						&& material->AOMap == L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CN_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap == L"" && material->metallicMap == L"" && material->roughnessMap == L""
						&& material->AOMap == L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_C_PS");
					}
					else
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_PS");
					}

					GraphicsManager::Get()->SendMaterialData(material);

					gameObject->GetComponent<MeshRenderer>()->SetMaterial(mat->materialName);

					break;
				}
			}

			if(mesh->materialName == "")
				gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_PS");

			gameObjects.push_back(gameObject);
		}
	}
	// Animation이 없는 경우 StaticMesh
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

			// 해당 메시의 vertices, indices 그래픽스 ResourceManager에등록과 해당 meshID를 가져온다.
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
			gameObject->GetComponent<MeshRenderer>()->SetMeshID(meshID);		// meshID 등록

			Vector3 localScale = { 1.0f, 1.0f, 1.0f };
			Vector3 localRotation = { 0.f, 0.f, 0.f };
			Vector3 localTranslation = { 0.f, .0f, 0.f };
			Transform::DecomposeMatrix(gameObject->GetTransform()->GetNodeMatrix(), localScale, localRotation, localTranslation);
			gameObject->GetComponent<Transform>()->SetLocalScale(localScale);
			gameObject->GetComponent<Transform>()->SetLocalRotation(localRotation);
			gameObject->GetComponent<Transform>()->SetLocalPosition(localTranslation);

			// 메시 하나당 머터리얼 하나
			for (auto& mat : fbxModel->materialList)
			{
				if (mesh->materialName == mat->materialName)
				{
					std::shared_ptr<Material> material = std::make_shared<Material>();

					material->name = mat->materialName;

					material->metallic = mat->metallic;
					material->roughness = mat->roughness;

					material->albedoMap = mat->albedoMap;
					material->normalMap = mat->normalMap;
					material->metallicMap = mat->metallicMap;
					material->roughnessMap = mat->roughnessMap;
					material->AOMap = mat->AOMap;
					material->emissiveMap = mat->emissiveMap;

					material->material_Ambient = mat->material_Ambient;
					material->material_Diffuse = mat->material_Diffuse;
					material->material_Specular = mat->material_Specular;
					material->material_Emissive = mat->material_Emissive;

					material->emissionFactor = mat->emissionFactor;
					material->material_Transparency = mat->material_Transparency;
					material->material_Reflectivity = mat->material_Reflectivity;

					if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap != L"" && material->roughnessMap != L""
						&& material->AOMap != L"" && material->emissiveMap != L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CNMRAE_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap != L"" && material->roughnessMap != L""
						&& material->AOMap != L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CNMRA_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap == L"" && material->roughnessMap != L""
						&& material->AOMap == L"" && material->emissiveMap != L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CNRE_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap == L"" && material->roughnessMap == L""
						&& material->AOMap != L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CNA_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap != L"" && material->roughnessMap != L""
						&& material->AOMap == L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CNMR_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap == L"" && material->metallicMap != L"" && material->roughnessMap != L""
						&& material->AOMap == L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CMR_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap == L"" && material->roughnessMap == L""
						&& material->AOMap == L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CN_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap == L"" && material->metallicMap == L"" && material->roughnessMap == L""
						&& material->AOMap == L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_C_PS");
					}
					else
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_PS");
					}

					GraphicsManager::Get()->SendMaterialData(material);

					gameObject->GetComponent<MeshRenderer>()->SetMaterial(mat->materialName);

					break;
				}
			}

			if (mesh->materialName == "")
				gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_PS");

			gameObjects.push_back(gameObject);
		}
	}

	return gameObjects;
}

std::vector<std::shared_ptr<GameObject>> Resources::LoadFBXBinary(std::string path, int topology, int rasterizerState)
{
	//ifstream input("test.noob", ios_base::binary);

	/*boost::iostreams::filtering_istream input;
	input.push(boost::iostreams::zlib_decompressor());
	input.push(boost::iostreams::file_source("test.noob"));*/

	std::ifstream ifs("test.noob", std::ios_base::binary);
	boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
	buffer.push(boost::iostreams::zlib_decompressor());
	buffer.push(ifs);
	boost::archive::binary_iarchive ia(buffer);

	//boost::archive::binary_iarchive ia(input);

	FBXBinaryData::ModelData modelData;

	ia >> modelData;

	// Map에 저장
	unordered_map<std::string, std::shared_ptr<GameObject>> boneMap;

	// 게임오브젝트들로 바꾸어서 반환할거임.
	vector<std::shared_ptr<GameObject>> gameObjects;

	// Animation이 있는 경우 대부분 SkinnedMesh
	if (modelData.isSkinnedAnimation)
	{
		// 본오브젝트를 만들자..
		for (int boneIdx = 0; boneIdx < modelData.boneInfoList.size(); boneIdx++)
		{
			std::shared_ptr<GameObject> boneObject = std::make_shared<GameObject>();
			boneObject->SetName(modelData.boneInfoList[boneIdx].boneName);
			boneObject->AddComponent<Transform>();

			FBXBinaryData::Float4x4 offset4x4 = modelData.boneInfoList[boneIdx].offsetMatrix;
			Matrix offsetMatrix
			(
				offset4x4.m00, offset4x4.m01, offset4x4.m02, offset4x4.m03,
				offset4x4.m10, offset4x4.m11, offset4x4.m12, offset4x4.m13,
				offset4x4.m20, offset4x4.m21, offset4x4.m22, offset4x4.m23,
				offset4x4.m30, offset4x4.m31, offset4x4.m32, offset4x4.m33
			);

			FBXBinaryData::Float4x4 node4x4 = modelData.boneInfoList[boneIdx].nodeMatrix;
			Matrix nodeMatrix
			(
				node4x4.m00, node4x4.m01, node4x4.m02, node4x4.m03,
				node4x4.m10, node4x4.m11, node4x4.m12, node4x4.m13,
				node4x4.m20, node4x4.m21, node4x4.m22, node4x4.m23,
				node4x4.m30, node4x4.m31, node4x4.m32, node4x4.m33
			);

			boneObject->GetComponent<Transform>()->SetBoneOffsetMatrix(offsetMatrix);
			boneObject->GetComponent<Transform>()->SetNodeTM(nodeMatrix);
			boneObject->AddComponent<MeshRenderer>();
			boneObject->GetComponent<MeshRenderer>()->SetMeshID(0);				// cube는 0
			shared_ptr<Material> boneMaterial = make_shared<Material>();
			boneMaterial->name = "BoneMaterial";
			boneMaterial->metallic = 0.0f;
			boneMaterial->roughness = 0.0f;
			GraphicsManager::Get()->SendMaterialData(boneMaterial);
			boneObject->GetComponent<MeshRenderer>()->SetMaterial(boneMaterial->name);
			boneObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_PS");

			Vector3 localScale = { 1.0f, 1.0f, 1.0f };
			Vector3 localRotation = { 0.f, 0.f, 0.f };
			Vector3 localTranslation = { 0.f, .0f, 0.f };

			if (!gameObjects.empty())
			{
				boneObject->GetTransform()->SetParent(gameObjects[modelData.boneInfoList[boneIdx].parentIndex]->GetTransform());
				gameObjects[modelData.boneInfoList[boneIdx].parentIndex]->SetChild(boneObject);

				XMMATRIX parentMatrixInverse = XMMatrixInverse(nullptr, gameObjects[modelData.boneInfoList[boneIdx].parentIndex]->GetTransform()->GetWorldMatrix());
				XMMATRIX multipleMatrix = XMMatrixMultiply(boneObject->GetTransform()->GetNodeMatrix(), parentMatrixInverse);

				Transform::DecomposeMatrix(multipleMatrix, localScale, localRotation, localTranslation);
			}
			else
			{
				Transform::DecomposeMatrix(boneObject->GetTransform()->GetNodeMatrix(), localScale, localRotation, localTranslation);
			}
			boneObject->GetComponent<Transform>()->SetLocalScale(localScale);
			boneObject->GetComponent<Transform>()->SetLocalRotation(localRotation);
			boneObject->GetComponent<Transform>()->SetLocalPosition(localTranslation);
			boneObject->GetComponent<Transform>()->FixedUpdate();

			// 일단 모델의 애니메이션 클립 리스트를 돈다
			for (auto& anim : modelData.animationClipList)
			{
				std::shared_ptr<AnimationClip> animationClip = std::make_shared<AnimationClip>();
				animationClip->animationName = anim.animationName;
				animationClip->frameRate = anim.frameRate;
				animationClip->startKeyFrame = anim.startKeyFrame;
				animationClip->endKeyFrame = anim.endKeyFrame;
				animationClip->totalKeyFrame = anim.totalKeyFrame;
				animationClip->tickPerFrame = anim.tickPerFrame;

				// 애니메이션의 키프레임 리스트에서 boneIdx에 해당하는게 있으면 Animator 생성해주고 정보를 넣어준다.
				if (!anim.keyFrameList[boneIdx].empty())
				{
					if (boneObject->GetComponent<Animator>() == nullptr)
						boneObject->AddComponent<Animator>();

					for (int keyIdx = 0; keyIdx < anim.keyFrameList[boneIdx].size(); keyIdx++)
					{
						std::shared_ptr<AnimKeyFrame> animKeyFrame = std::make_shared<AnimKeyFrame>();
						animKeyFrame->time = anim.keyFrameList[boneIdx][keyIdx].time;
						Vector3 localTransform = { anim.keyFrameList[boneIdx][keyIdx].localTransform.x, anim.keyFrameList[boneIdx][keyIdx].localTransform.y, anim.keyFrameList[boneIdx][keyIdx].localTransform.z };
						Vector4 localRotation = { anim.keyFrameList[boneIdx][keyIdx].localRotation.x,  anim.keyFrameList[boneIdx][keyIdx].localRotation.y,  anim.keyFrameList[boneIdx][keyIdx].localRotation.z, anim.keyFrameList[boneIdx][keyIdx].localRotation.w };
						Vector3 localScale = { anim.keyFrameList[boneIdx][keyIdx].localScale.x, anim.keyFrameList[boneIdx][keyIdx].localScale.y, anim.keyFrameList[boneIdx][keyIdx].localScale.z};
						animKeyFrame->localTransform = localTransform;
						animKeyFrame->localRotation = localRotation;
						animKeyFrame->localScale = localScale;

						animationClip->keyFrame.push_back(animKeyFrame);
					}

					boneObject->GetComponent<Animator>()->SetAnimClip(anim.animationName, animationClip);
				}
			}

			boneMap.insert(make_pair(modelData.boneInfoList[boneIdx].boneName, boneObject));

			gameObjects.push_back(boneObject);
		}

		for (auto& mesh : modelData.meshInfoList)
		{
			std::vector<SkinnedMeshVertex> skinnedMeshVertices;

			for (auto& vertex : mesh.meshVertexList)
			{
				SkinnedMeshVertex skinnedMeshVertex;
				Vector3 position = { vertex.position.x, vertex.position.y, vertex.position.z };
				Vector4 color = { vertex.color.x, vertex.color.y, vertex.color.z, vertex.color.w };
				Vector2 uv = { vertex.uv.x, vertex.uv.y };
				Vector3 normal = { vertex.normal.x, vertex.normal.y, vertex.normal.z };
				Vector3 tangent = { vertex.tangent.x, vertex.tangent.y, vertex.tangent.z };
				skinnedMeshVertex.position = position;
				skinnedMeshVertex.color = color;
				skinnedMeshVertex.uv = uv;
				skinnedMeshVertex.normal = normal;
				skinnedMeshVertex.tangent = tangent;

				for (int weightCnt = 0; weightCnt < 8; weightCnt++)
				{
					skinnedMeshVertex.weights[weightCnt] = vertex.weights[weightCnt];
					skinnedMeshVertex.boneIndices[weightCnt] = vertex.boneIndices[weightCnt];
				}

				skinnedMeshVertices.push_back(skinnedMeshVertex);
			}

			// 해당 메시의 vertices, indices 그래픽스 ResourceManager에등록과 해당 meshID를 가져온다.
			size_t meshID = GraphicsManager::Get()->CreateMesh(
				skinnedMeshVertices,
				mesh.indices,
				topology,
				rasterizerState);

			std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();
			gameObject->SetName(mesh.meshName);
			gameObject->AddComponent<Transform>();
			Matrix matrix
			(
				mesh.nodeTM.m00, mesh.nodeTM.m01, mesh.nodeTM.m02, mesh.nodeTM.m03,
				mesh.nodeTM.m10, mesh.nodeTM.m11, mesh.nodeTM.m12, mesh.nodeTM.m13,
				mesh.nodeTM.m20, mesh.nodeTM.m21, mesh.nodeTM.m22, mesh.nodeTM.m23,
				mesh.nodeTM.m30, mesh.nodeTM.m31, mesh.nodeTM.m32, mesh.nodeTM.m33
			);
			gameObject->GetComponent<Transform>()->SetNodeTM(matrix);		// NodeTM 넣기
			gameObject->AddComponent<MeshRenderer>();
			gameObject->GetComponent<MeshRenderer>()->IsSkinnedMesh(mesh.isSkinned);
			gameObject->GetComponent<MeshRenderer>()->SetMeshID(meshID);		// meshID 등록

			Vector3 localScale = { 1.0f, 1.0f, 1.0f };
			Vector3 localRotation = { 0.f, 0.f, 0.f };
			Vector3 localTranslation = { 0.f, .0f, 0.f };
			Transform::DecomposeMatrix(gameObject->GetTransform()->GetNodeMatrix(), localScale, localRotation, localTranslation);
			gameObject->GetComponent<Transform>()->SetLocalScale(localScale);
			gameObject->GetComponent<Transform>()->SetLocalRotation(localRotation);
			gameObject->GetComponent<Transform>()->SetLocalPosition(localTranslation);
			gameObject->GetComponent<Transform>()->FixedUpdate();

			// Bone에 의해 영향을 받는 Mesh라면
			if (mesh.isSkinned == true)
			{
				for (auto& bone : modelData.boneInfoList)
				{
					gameObject->GetComponent<MeshRenderer>()->SetBoneObject(boneMap[bone.boneName]);
				}
			}

			// 메시 하나당 머터리얼 하나
			for (auto& mat : modelData.materialList)
			{
				if (mesh.materialName == mat.materialName)
				{
					std::shared_ptr<Material> material = std::make_shared<Material>();

					material->name = mat.materialName;

					material->metallic = 0.1f;
					material->roughness = 0.0f;

					material->albedoMap = mat.albedoMap;
					material->normalMap = mat.normalMap;
					material->metallicMap = mat.metallicMap;
					material->roughnessMap = mat.roughnessMap;
					material->AOMap = mat.AOMap;
					material->emissiveMap = mat.emissiveMap;

					material->material_Ambient = Vector4(mat.material_Ambient.x, mat.material_Ambient.y, mat.material_Ambient.z, mat.material_Ambient.w);
					material->material_Diffuse = Vector4(mat.material_Diffuse.x, mat.material_Diffuse.y, mat.material_Diffuse.z, mat.material_Diffuse.w);
					material->material_Specular = Vector4(mat.material_Specular.x, mat.material_Specular.y, mat.material_Specular.z, mat.material_Specular.w);
					material->material_Emissive = Vector4(mat.material_Emissive.x, mat.material_Emissive.y, mat.material_Emissive.z, mat.material_Emissive.w);

					material->emissionFactor = mat.emissionFactor;
					material->material_Transparency = mat.material_Transparency;
					material->material_Reflectivity = mat.material_Reflectivity;

					if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap != L"" && material->roughnessMap != L""
						&& material->AOMap != L"" && material->emissiveMap != L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CNMRAE_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap != L"" && material->roughnessMap != L""
						&& material->AOMap != L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CNMRA_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap == L"" && material->roughnessMap != L""
						&& material->AOMap == L"" && material->emissiveMap != L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CNRE_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap == L"" && material->roughnessMap == L""
						&& material->AOMap != L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CNA_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap != L"" && material->roughnessMap != L""
						&& material->AOMap == L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CNMR_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap == L"" && material->metallicMap != L"" && material->roughnessMap != L""
						&& material->AOMap == L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CMR_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap == L"" && material->roughnessMap == L""
						&& material->AOMap == L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CN_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap == L"" && material->metallicMap == L"" && material->roughnessMap == L""
						&& material->AOMap == L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_C_PS");
					}
					else
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_PS");
					}

					GraphicsManager::Get()->SendMaterialData(material);

					gameObject->GetComponent<MeshRenderer>()->SetMaterial(mat.materialName);

					break;
				}
			}

			if (mesh.materialName == "")
				gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_PS");

			gameObjects.push_back(gameObject);
		}
	}
	// Animation이 없는 경우 StaticMesh
	else
	{
		for (auto& mesh : modelData.meshInfoList)
		{
			std::vector<StaticMeshVertex> staticMeshVertices;

			for (auto& vertex : mesh.meshVertexList)
			{
				StaticMeshVertex staticMeshVertex;
				staticMeshVertex.position = Vector3(vertex.position.x, vertex.position.y, vertex.position.z);
				staticMeshVertex.color = Vector4(vertex.color.x, vertex.color.y, vertex.color.z, vertex.color.w);
				staticMeshVertex.uv = Vector2(vertex.uv.x, vertex.uv.y);
				staticMeshVertex.normal = Vector3(vertex.normal.x, vertex.normal.y, vertex.normal.z);
				staticMeshVertex.tangent = Vector3(vertex.tangent.x, vertex.tangent.y, vertex.tangent.z);

				staticMeshVertices.push_back(staticMeshVertex);
			}

			// 해당 메시의 vertices, indices 그래픽스 ResourceManager에등록과 해당 meshID를 가져온다.
			size_t meshID = GraphicsManager::Get()->CreateMesh(
				staticMeshVertices,
				mesh.indices,
				topology,
				rasterizerState);

			std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();
			gameObject->SetName(mesh.meshName);
			gameObject->AddComponent<Transform>();

			Matrix matrix
			(
				mesh.nodeTM.m00, mesh.nodeTM.m01, mesh.nodeTM.m02, mesh.nodeTM.m03,
				mesh.nodeTM.m10, mesh.nodeTM.m11, mesh.nodeTM.m12, mesh.nodeTM.m13,
				mesh.nodeTM.m20, mesh.nodeTM.m21, mesh.nodeTM.m22, mesh.nodeTM.m23,
				mesh.nodeTM.m30, mesh.nodeTM.m31, mesh.nodeTM.m32, mesh.nodeTM.m33
			);
			gameObject->GetComponent<Transform>()->SetNodeTM(matrix);
			gameObject->AddComponent<MeshRenderer>();
			gameObject->GetComponent<MeshRenderer>()->SetMeshID(meshID);		// meshID 등록

			Vector3 localScale = { 1.0f, 1.0f, 1.0f };
			Vector3 localRotation = { 0.f, 0.f, 0.f };
			Vector3 localTranslation = { 0.f, .0f, 0.f };
			Transform::DecomposeMatrix(gameObject->GetTransform()->GetNodeMatrix(), localScale, localRotation, localTranslation);
			gameObject->GetComponent<Transform>()->SetLocalScale(localScale);
			gameObject->GetComponent<Transform>()->SetLocalRotation(localRotation);
			gameObject->GetComponent<Transform>()->SetLocalPosition(localTranslation);

			// 메시 하나당 머터리얼 하나
			for (auto& mat : modelData.materialList)
			{
				if (mesh.materialName == mat.materialName)
				{
					std::shared_ptr<Material> material = std::make_shared<Material>();

					material->name = mat.materialName;

					material->metallic = 0.1f;
					material->roughness = 0.0f;

					material->albedoMap = mat.albedoMap;
					material->normalMap = mat.normalMap;
					material->metallicMap = mat.metallicMap;
					material->roughnessMap = mat.roughnessMap;
					material->AOMap = mat.AOMap;
					material->emissiveMap = mat.emissiveMap;

					material->material_Ambient = Vector4(mat.material_Ambient.x, mat.material_Ambient.y, mat.material_Ambient.z, mat.material_Ambient.w);
					material->material_Diffuse = Vector4(mat.material_Diffuse.x, mat.material_Diffuse.y, mat.material_Diffuse.z, mat.material_Diffuse.w);
					material->material_Specular = Vector4(mat.material_Specular.x, mat.material_Specular.y, mat.material_Specular.z, mat.material_Specular.w);
					material->material_Emissive = Vector4(mat.material_Emissive.x, mat.material_Emissive.y, mat.material_Emissive.z, mat.material_Emissive.w);

					material->emissionFactor = mat.emissionFactor;
					material->material_Transparency = mat.material_Transparency;
					material->material_Reflectivity = mat.material_Reflectivity;

					if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap != L"" && material->roughnessMap != L""
						&& material->AOMap != L"" && material->emissiveMap != L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CNMRAE_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap != L"" && material->roughnessMap != L""
						&& material->AOMap != L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CNMRA_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap == L"" && material->roughnessMap != L""
						&& material->AOMap == L"" && material->emissiveMap != L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CNRE_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap == L"" && material->roughnessMap == L""
						&& material->AOMap != L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CNA_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap != L"" && material->roughnessMap != L""
						&& material->AOMap == L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CNMR_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap == L"" && material->metallicMap != L"" && material->roughnessMap != L""
						&& material->AOMap == L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CMR_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap != L"" && material->metallicMap == L"" && material->roughnessMap == L""
						&& material->AOMap == L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_CN_PS");
					}
					else if (material->albedoMap != L"" && material->normalMap == L"" && material->metallicMap == L"" && material->roughnessMap == L""
						&& material->AOMap == L"" && material->emissiveMap == L"")
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_C_PS");
					}
					else
					{
						gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_PS");
					}

					GraphicsManager::Get()->SendMaterialData(material);

					gameObject->GetComponent<MeshRenderer>()->SetMaterial(mat.materialName);

					break;
				}
			}

			if (mesh.materialName == "")
				gameObject->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_PS");

			gameObjects.push_back(gameObject);
		}
	}

	return gameObjects;
}

void Resources::MaterialReconstruction(std::string materialName, std::string textureName, std::string info)
{
	GraphicsManager::Get()->MaterialReconstruction(materialName, textureName, info);
}

