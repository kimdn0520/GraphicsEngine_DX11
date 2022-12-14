#include "pch.h"
#include "TestScene.h"
#include "DataManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "GenjiPrefab.h"
#include "TestFBXPrefab.h"
#include "GraphicsManager.h"

#include "Player.h"
#include "CameraScript.h"
#include "TestAnimPrefab.h"
#include "Resources.h"

void TestScene::Initialize()
{
	if (!DataManager::Get()->isDataLoad)
		DataManager::Get()->DataLoad();

	Resources::Get()->LoadSceneData("Data/Scene/SampleScene.unity");

	//---------------------------------------------------------------------------------------------------------
	// Main Camera
	//---------------------------------------------------------------------------------------------------------
	shared_ptr<GameObject> camera = make_shared<GameObject>();
	camera->SetName("MainCamera");
	camera->AddComponent<Transform>();
	camera->AddComponent<Camera>();
	camera->AddComponent<CameraScript>();
	camera->GetComponent<Transform>()->SetLocalPosition(DirectX::SimpleMath::Vector3(0.f, 10.f, -10.f));
	camera->GetComponent<Transform>()->LookAt(DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f));
	AddGameObject(camera);

	//---------------------------------------------------------------------------------------------------------
	// SkyBox
	//---------------------------------------------------------------------------------------------------------
	shared_ptr<GameObject> skyBox = make_shared<GameObject>();
	skyBox->SetName("SkyBox");
	skyBox->AddComponent<Transform>();
	skyBox->AddComponent<MeshRenderer>();
	skyBox->GetComponent<MeshRenderer>()->SetMeshID(1);
	skyBox->GetComponent<MeshRenderer>()->GetObjectInfo()->type = OBJECT_TYPE::SKY_BOX;
	skyBox->GetComponent<MeshRenderer>()->GetObjectInfo()->isShadow = false;
	shared_ptr<Material> skyBoxMaterial = make_shared<Material>();
	skyBoxMaterial->name = "SkyBoxMaterial";
	skyBoxMaterial->isLight = false;
	skyBoxMaterial->cubeMap = TEXT("daylight.dds");
	GraphicsManager::Get()->SendMaterialData(skyBoxMaterial);					// 머터리얼 등록
	skyBox->GetComponent<MeshRenderer>()->SetMaterial(skyBoxMaterial->name);
	AddGameObject(skyBox);

	//---------------------------------------------------------------------------------------------------------
	// Directional Light
	//---------------------------------------------------------------------------------------------------------
	shared_ptr<GameObject> dirLight = make_shared<GameObject>();
	dirLight->SetName("dirLight");
	dirLight->AddComponent<Transform>();
	dirLight->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
	dirLight->AddComponent<Light>();
	dirLight->GetComponent<Light>()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
	dirLight->GetComponent<Light>()->SetLightDirection(Vector3(0.5f, -0.5f, 0.5f));
	dirLight->GetComponent<Light>()->SetColor(Vector3(1.0f, 1.0f, 1.0f));
	dirLight->GetComponent<Light>()->SetPower(1.f);
	dirLight->GetComponent<Light>()->SendLightInfo();
	AddGameObject(dirLight);

	//---------------------------------------------------------------------------------------------------------
	// Axis
	//---------------------------------------------------------------------------------------------------------
	shared_ptr<GameObject> axis = make_shared<GameObject>();
	axis->SetName("axis");
	axis->AddComponent<Transform>();
	axis->AddComponent<MeshRenderer>();
	axis->GetComponent<MeshRenderer>()->SetMeshID(2);				// axis는 2					
	shared_ptr<Material> axisMaterial = make_shared<Material>();
	axisMaterial->name = "axisMaterial";
	axisMaterial->isLight = false;
	GraphicsManager::Get()->SendMaterialData(axisMaterial);
	axis->GetComponent<MeshRenderer>()->SetMaterial(axisMaterial->name);
	axis->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_PS");
	axis->GetComponent<MeshRenderer>()->IsShadow(false);			// 이녀석은 섀도우 안할건뎅~
	AddGameObject(axis);

	//shared_ptr<GameObject> box = make_shared<GameObject>();
	//box->SetName("box");
	//box->AddComponent<Transform>();
	//box->GetComponent<Transform>()->SetLocalPosition(DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f));
	//box->GetComponent<Transform>()->SetLocalScale(DirectX::SimpleMath::Vector3(20.f, 1.0f, 20.f));
	//box->AddComponent<MeshRenderer>();
	//box->GetComponent<MeshRenderer>()->SetMeshID(0);				// cube는 0
	//shared_ptr<Material> boxMaterial = make_shared<Material>();
	//boxMaterial->name = "BoxMaterial";
	//boxMaterial->metallic = 0.0f;
	//boxMaterial->roughness = 0.0f;
	//GraphicsManager::Get()->SendMaterialData(boxMaterial);
	//box->GetComponent<MeshRenderer>()->SetMaterial(boxMaterial->name);
	//box->GetComponent<MeshRenderer>()->SetPixelShader(L"Model_PBR_PS");
	//AddGameObject(box);

	/*shared_ptr<GenjiPrefab> genji = make_shared<GenjiPrefab>(Vector3(0.f, 1.f, 0.f));
	AddGameObject(genji->gameObject);*/

	shared_ptr<TestFBXPrefab> crate = make_shared<TestFBXPrefab>(Vector3(0.f, 0.f, 0.f));
	crate->gameObject->GetTransform()->SetLocalRotation(Vector3(0.f, 180.f, 0.f));
	AddGameObject(crate->gameObject);

	shared_ptr<TestAnimPrefab> wolf = make_shared<TestAnimPrefab>(Vector3(-2.f, 0.f, 0.f));
	wolf->gameObject->GetTransform()->SetLocalRotation(Vector3(0.f, 180.f, 0.f));
	wolf->gameObject->AddComponent<Player>();
	AddGameObject(wolf->gameObject);
}
