#include "pch.h"
#include "TestScene.h"
#include "DataManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "Light.h"
#include "GenjiPrefab.h"

#include "CameraScript.h"

void TestScene::Initialize()
{
	if (!DataManager::Get()->isDataLoad)
		DataManager::Get()->DataLoad();

	//---------------------------------------------------------------------------------------------------------
	// Main Camera
	//---------------------------------------------------------------------------------------------------------
	GameObject* camera = new GameObject();
	camera->SetName("MainCamera");
	camera->AddComponent<Transform>();
	camera->AddComponent<Camera>();
	camera->AddComponent<CameraScript>();
	camera->GetComponent<Transform>()->SetLocalPosition(DirectX::SimpleMath::Vector3(0.f, 10.f, -10.f));
	camera->GetComponent<Transform>()->LookAt(DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f));
	AddGameObject(camera);

	//---------------------------------------------------------------------------------------------------------
	// Directional Light
	//---------------------------------------------------------------------------------------------------------
	GameObject* dirLight = new GameObject();
	dirLight->SetName("dirLight");
	dirLight->AddComponent<Transform>();
	dirLight->AddComponent<Light>();
	dirLight->GetComponent<Light>()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
	dirLight->GetComponent<Light>()->SetLightDirection(Vector3(0.57735f, -0.57735f, 0.57735f));
	dirLight->GetComponent<Light>()->SetDiffuse(Vector4(0.6f, 0.6f, 0.6f, 1.0f));	// 기본0.5 
	dirLight->GetComponent<Light>()->SetAmbient(Vector4(0.4f, 0.4f, 0.4f, 1.0f));	// 기본0.2
	dirLight->GetComponent<Light>()->SetSpecular(Vector4(0.2f, 0.2f, 0.2f, 1.0f));  // 기본0.5
	dirLight->GetComponent<Light>()->SendLightInfo();
	AddGameObject(dirLight);

	//---------------------------------------------------------------------------------------------------------
	// SkyBox
	//---------------------------------------------------------------------------------------------------------
	/*shared_ptr<GameObject> skyBox = make_shared<GameObject>();
	skyBox->SetName("SkyBox");
	skyBox->AddComponent<Transform>();
	skyBox->AddComponent<MeshRenderer>();
	skyBox->GetComponent<MeshRenderer>()->SetMesh((int)MeshName::SphereMesh);
	skyBox->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID = DataManager::GetInstance()->GetSkyBoxId();
	skyBox->GetComponent<MeshRenderer>()->GetMeshInfo()->type = OBJECT_TYPE::SKY_BOX;
	Resources::GetInstance()->SetCubeMapTexture(skyBox->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID, L"daylight.dds");
	AddGameObject(skyBox);*/

	//---------------------------------------------------------------------------------------------------------
	// Axis
	//---------------------------------------------------------------------------------------------------------
	GameObject* axis = new GameObject();
	axis->SetName("axis");
	axis->AddComponent<Transform>();
	axis->AddComponent<MeshRenderer>();
	axis->GetComponent<MeshRenderer>()->SetMeshID(2);				// axis는 2					
	Material* axisMaterial = new Material();
	axisMaterial->isLight = false;
	axis->GetComponent<MeshRenderer>()->SetMaterial(axisMaterial);
	axis->GetComponent<MeshRenderer>()->IsShadow(false);			// 이녀석은 섀도우 안할건뎅~
	AddGameObject(axis);

	GameObject* box = new GameObject();
	box->SetName("box");
	box->AddComponent<Transform>();
	box->GetComponent<Transform>()->SetLocalPosition(DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f));
	box->GetComponent<Transform>()->SetLocalScale(DirectX::SimpleMath::Vector3(20.f, 1.0f, 20.f));
	box->AddComponent<MeshRenderer>();
	box->GetComponent<MeshRenderer>()->SetMeshID(0);				// cube는 0
	Material* boxMaterial = new Material();
	boxMaterial->ambient = Vector4(0.6f, 0.6f, 0.6f, 1.0f);
	boxMaterial->diffuse = Vector4(0.6f, 0.6f, 0.6f, 1.0f);
	boxMaterial->specular = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	box->GetComponent<MeshRenderer>()->SetMaterial(boxMaterial);
	AddGameObject(box);

	GenjiPrefab* genji = new GenjiPrefab(Vector3(0.f, 0.5f, 0.f));
	AddGameObject(genji);
}
