#include "pch.h"
#include "TestFBXPrefab.h"
#include "Resources.h"
#include "Transform.h"
#include "MonoBehaviour.h"
#include "GameObject.h"

#include "DataManager.h"

TestFBXPrefab::TestFBXPrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	std::vector<shared_ptr<GameObject>> gameObjects = DataManager::Get()->GetDataResources("Eye");

	gameObject->SetTag(Tag::Default);
	gameObject->SetName("Test");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalScale({ 1.f, 1.f, 1.f });
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);

	for (auto& gameObj : gameObjects)
	{
		if (gameObj->GetTransform()->GetParent() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

TestFBXPrefab::~TestFBXPrefab()
{}
