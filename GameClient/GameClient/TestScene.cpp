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
	shared_ptr<GameObject> camera = make_shared<GameObject>();
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
	shared_ptr<GameObject> dirLight = make_shared<GameObject>();
	dirLight->SetName("dirLight");
	dirLight->AddComponent<Transform>();
	dirLight->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
	dirLight->AddComponent<Light>();
	dirLight->GetComponent<Light>()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
	dirLight->GetComponent<Light>()->SetLightDirection(Vector3(0.5f, -0.5f, 0.5f));
	dirLight->GetComponent<Light>()->SetDiffuse(Vector4(0.6f, 0.6f, 0.6f, 1.0f));	// ±âº»0.5 
	dirLight->GetComponent<Light>()->SetAmbient(Vector4(0.4f, 0.4f, 0.4f, 1.0f));	// ±âº»0.2
	dirLight->GetComponent<Light>()->SetSpecular(Vector4(0.2f, 0.2f, 0.2f, 1.0f));  // ±âº»0.5
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
	shared_ptr<GameObject> axis = make_shared<GameObject>();
	axis->SetName("axis");
	axis->AddComponent<Transform>();
	axis->AddComponent<MeshRenderer>();
	axis->GetComponent<MeshRenderer>()->SetMeshID(2);				// axis´Â 2					
	shared_ptr<Material> axisMaterial = make_shared<Material>();
	axisMaterial->isLight = false;
	axis->GetComponent<MeshRenderer>()->SetMaterial(axisMaterial);
	axis->GetComponent<MeshRenderer>()->IsShadow(false);			// ÀÌ³à¼®Àº ¼¨µµ¿ì ¾ÈÇÒ°Çµ­~
	AddGameObject(axis);

	shared_ptr<GameObject> box = make_shared<GameObject>();
	box->SetName("box");
	box->AddComponent<Transform>();
	box->GetComponent<Transform>()->SetLocalPosition(DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f));
	box->GetComponent<Transform>()->SetLocalScale(DirectX::SimpleMath::Vector3(5.f, 1.0f, 5.f));
	box->AddComponent<MeshRenderer>();
	box->GetComponent<MeshRenderer>()->SetMeshID(0);				// cube´Â 0
	shared_ptr<Material> boxMaterial = make_shared<Material>();
	boxMaterial->ambient = Vector4(0.6f, 0.6f, 0.6f, 1.0f);
	boxMaterial->diffuse = Vector4(0.6f, 0.6f, 0.6f, 1.0f);
	boxMaterial->specular = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	box->GetComponent<MeshRenderer>()->SetMaterial(boxMaterial);
	//box->GetComponent<MeshRenderer>()->IsShadow(false);			// ÀÌ³à¼®Àº ¼¨µµ¿ì ¾ÈÇÒ°Çµ­~
	AddGameObject(box);

	shared_ptr<GenjiPrefab> genji = make_shared<GenjiPrefab>(Vector3(0.f, 1.f, 0.f));
	AddGameObject(genji->gameObject);
}
